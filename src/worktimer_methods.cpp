#include "worktimer.h"
#include <QApplication>
#include <QDebug>

// Timer control methods
void WorkTimer::startTimer()
{
    m_isRunning = true;
    m_timer->start(1000); // Update every second
    m_startButton->setEnabled(false);
    m_pauseButton->setEnabled(true);
}

void WorkTimer::pauseTimer()
{
    m_isRunning = false;
    m_timer->stop();
    m_startButton->setEnabled(true);
    m_pauseButton->setEnabled(false);
}

void WorkTimer::resetTimer()
{
    pauseTimer();
    m_currentSession = 1;
    m_isWorkSession = true;
    m_timeRemaining = m_workDuration * 60;
    updateDisplay();
}

void WorkTimer::restartTimer()
{
    // Reset to first session and start immediately
    m_currentSession = 1;
    m_isWorkSession = true;
    m_timeRemaining = m_workDuration * 60;
    updateDisplay();
    startTimer();
}

void WorkTimer::updateTimer()
{
    if (m_timeRemaining > 0) {
        m_timeRemaining--;
        updateDisplay();
    } else {
        timerFinished();
    }
}

void WorkTimer::timerFinished()
{
    pauseTimer();
    playNotificationSound();
    
    if (m_isWorkSession) {
        // Work session finished, start break
        if (m_currentSession % m_sessionsUntilLongBreak == 0) {
            // Long break
            m_timeRemaining = m_longBreakDuration * 60;
            m_sessionLabel->setText(QString("Сессия %1 - Длинный перерыв").arg(m_currentSession));
        } else {
            // Short break
            m_timeRemaining = m_shortBreakDuration * 60;
            m_sessionLabel->setText(QString("Сессия %1 - Короткий перерыв").arg(m_currentSession));
        }
        m_isWorkSession = false;
    } else {
        // Break finished, start next work session
        m_currentSession++;
        m_timeRemaining = m_workDuration * 60;
        m_sessionLabel->setText(QString("Сессия %1 - Рабочее время").arg(m_currentSession));
        m_isWorkSession = true;
    }
    
    updateDisplay();
    
    // Automatically start the next timer after a short delay
    QTimer::singleShot(2000, this, &WorkTimer::startTimer);
}

void WorkTimer::toggleSettings(bool checked)
{
    if (!m_settingsGroup || !m_windowAnimation || !m_settingsAnimation || !m_animationGroup) {
        qDebug() << "Animation components not initialized!";
        return;
    }
    
    // Prevent multiple clicks during animation
    if (m_isAnimating) {
        return;
    }
    
    // Block the button during animation
    m_toggleSettingsButton->setEnabled(false);
    m_isAnimating = true;
    
    if (checked) {
        // Show settings first
        m_settingsGroup->show();
        
        // Animate window expansion
        QRect currentWindowGeometry = geometry();
        QRect expandedWindowGeometry = QRect(currentWindowGeometry.x(), currentWindowGeometry.y(), 
                                           m_baseWindowWidth + m_settingsWidth, currentWindowGeometry.height());
        
        m_windowAnimation->setStartValue(currentWindowGeometry);
        m_windowAnimation->setEndValue(expandedWindowGeometry);
        
        // Animate settings panel sliding in from right
        QRect startSettingsGeometry = QRect(m_baseWindowWidth + m_settingsWidth, 0, m_settingsWidth, height());
        QRect endSettingsGeometry = QRect(m_baseWindowWidth, 0, m_settingsWidth, height());
        
        m_settingsGroup->setGeometry(startSettingsGeometry);
        m_settingsAnimation->setStartValue(startSettingsGeometry);
        m_settingsAnimation->setEndValue(endSettingsGeometry);
        
        // Start both animations simultaneously
        m_animationGroup->start();
        
        // Update UI state
        m_settingsVisible = true;
        
        // Re-enable button when animation finishes
        connect(m_animationGroup, &QParallelAnimationGroup::finished, this, [this]() {
            m_toggleSettingsButton->setEnabled(true);
            m_isAnimating = false;
            updateAllIcons(); // Update icons after animation
        }, Qt::SingleShotConnection);
    } else {
        // Animate window contraction
        QRect currentWindowGeometry = geometry();
        QRect contractedWindowGeometry = QRect(currentWindowGeometry.x(), currentWindowGeometry.y(), 
                                             m_baseWindowWidth, currentWindowGeometry.height());
        
        m_windowAnimation->setStartValue(currentWindowGeometry);
        m_windowAnimation->setEndValue(contractedWindowGeometry);
        
        // Animate settings panel sliding out to right
        QRect startSettingsGeometry = QRect(m_baseWindowWidth, 0, m_settingsWidth, height());
        QRect endSettingsGeometry = QRect(m_baseWindowWidth + m_settingsWidth, 0, m_settingsWidth, height());
        
        m_settingsAnimation->setStartValue(startSettingsGeometry);
        m_settingsAnimation->setEndValue(endSettingsGeometry);
        
        // Connect animation finished signal to hide settings and re-enable button
        connect(m_animationGroup, &QParallelAnimationGroup::finished, this, [this]() {
            if (m_settingsGroup) {
                m_settingsGroup->hide();
            }
            m_toggleSettingsButton->setEnabled(true);
            m_isAnimating = false;
            updateAllIcons(); // Update icons after animation
            m_animationGroup->disconnect(); // Disconnect to avoid multiple calls
        }, Qt::SingleShotConnection);
        
        // Start both animations simultaneously
        m_animationGroup->start();
        
        // Update UI state
        m_settingsVisible = false;
    }
}

void WorkTimer::closeApplication()
{
    saveSettings();
    m_timer->stop();
    QApplication::quit();
}

// Settings update methods
void WorkTimer::updateWorkDuration(int value)
{
    m_workDuration = value;
    if (m_isWorkSession && !m_isRunning) {
        m_timeRemaining = m_workDuration * 60;
        updateDisplay();
    }
    saveSettings();
}

void WorkTimer::updateShortBreakDuration(int value)
{
    m_shortBreakDuration = value;
    saveSettings();
}

void WorkTimer::updateLongBreakDuration(int value)
{
    m_longBreakDuration = value;
    saveSettings();
}

void WorkTimer::updateSessionsUntilLongBreak(int value)
{
    m_sessionsUntilLongBreak = value;
    saveSettings();
}

void WorkTimer::updateTheme(const QString &theme)
{
    m_currentTheme = theme;
    applyStylesheet();
    
    // Update theme button icon
    if (m_currentTheme == "dark") {
        m_themeButton->setIcon(QIcon(":/new/buttons/ui/dark.svg"));
    } else {
        m_themeButton->setIcon(QIcon(":/new/buttons/ui/light.svg"));
    }
    
    // Update all icons based on theme
    updateAllIcons();
    
    saveSettings();
}



void WorkTimer::updatePinOnTop(int state)
{
    m_pinOnTop = (state == Qt::Checked);
    if (m_pinOnTop) {
        setWindowFlags(m_baseFlags | Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlags(m_baseFlags);
    }
    show(); // Recreate window with new flags
    saveSettings();
}



void WorkTimer::updateAllIcons()
{
    if (m_currentTheme == "dark") {
        // Use white icons for dark theme
        if (m_startButton) {
            m_startButton->setIcon(QIcon(":/new/buttons/ui/play_white.svg"));
        }
        if (m_pauseButton) {
            m_pauseButton->setIcon(QIcon(":/new/buttons/ui/pause_white.svg"));
        }
        if (m_resetButton) {
            m_resetButton->setIcon(QIcon(":/new/buttons/ui/stop_white.svg"));
        }
        if (m_restartButton) {
            m_restartButton->setIcon(QIcon(":/new/buttons/ui/restart_white.svg"));
        }
        if (m_toggleSettingsButton) {
            if (m_settingsVisible) {
                m_toggleSettingsButton->setIcon(QIcon(":/new/buttons/ui/settingsON_white.svg"));
            } else {
                m_toggleSettingsButton->setIcon(QIcon(":/new/buttons/ui/settings_white.svg"));
            }
        }
        if (m_closeButton) {
            m_closeButton->setIcon(QIcon(":/new/buttons/ui/exit_white.svg"));
        }
        
        // Update label pixmaps for dark theme
        QLabel* soundLabel = m_uiWidget->findChild<QLabel*>("soundLabel");
        if (soundLabel) {
            soundLabel->setPixmap(QPixmap(":/new/buttons/ui/music_white.svg"));
        }
        
        QLabel* volumeLabel = m_uiWidget->findChild<QLabel*>("volumeLabel");
        if (volumeLabel) {
            volumeLabel->setPixmap(QPixmap(":/new/buttons/ui/volume_white.svg"));
        }
    } else {
        // Use original icons for light theme
        if (m_startButton) {
            m_startButton->setIcon(QIcon(":/new/buttons/ui/play.svg"));
        }
        if (m_pauseButton) {
            m_pauseButton->setIcon(QIcon(":/new/buttons/ui/pause.svg"));
        }
        if (m_resetButton) {
            m_resetButton->setIcon(QIcon(":/new/buttons/ui/stop.svg"));
        }
        if (m_restartButton) {
            m_restartButton->setIcon(QIcon(":/new/buttons/ui/restart.svg"));
        }
        if (m_toggleSettingsButton) {
            if (m_settingsVisible) {
                m_toggleSettingsButton->setIcon(QIcon(":/new/buttons/ui/settingsON.svg"));
            } else {
                m_toggleSettingsButton->setIcon(QIcon(":/new/buttons/ui/settings.svg"));
            }
        }
        if (m_closeButton) {
            m_closeButton->setIcon(QIcon(":/new/buttons/ui/exit.svg"));
        }
        
        // Update label pixmaps for light theme
        QLabel* soundLabel = m_uiWidget->findChild<QLabel*>("soundLabel");
        if (soundLabel) {
            soundLabel->setPixmap(QPixmap(":/new/buttons/ui/music.svg"));
        }
        
        QLabel* volumeLabel = m_uiWidget->findChild<QLabel*>("volumeLabel");
        if (volumeLabel) {
            volumeLabel->setPixmap(QPixmap(":/new/buttons/ui/volume.svg"));
        }
    }
}

void WorkTimer::updateSound(const QString &soundName)
{
    m_selectedSound = soundName;
    saveSettings();
}

void WorkTimer::updateVolume(int value)
{
    m_soundVolume = value / 100.0;
    m_audioOutput->setVolume(m_soundVolume);
    saveSettings();
}

void WorkTimer::playVolumePreview()
{
    // Play preview sound only when slider is released
    playNotificationSound();
}

void WorkTimer::applyStylesheet()
{
    QString styleFile;
    if (m_currentTheme == "dark") {
        styleFile = ":/styles/dark.qss";
    } else {
        styleFile = ":/styles/light.qss";
    }
    
    QFile file(styleFile);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString stylesheet = file.readAll();
        setStyleSheet(stylesheet);
        file.close();
    } else {
        qDebug() << "Failed to load stylesheet:" << styleFile;
        // Fallback to basic styling
        if (m_currentTheme == "dark") {
            setStyleSheet("QMainWindow { background-color: #2b2b2b; color: #ffffff; }");
        } else {
            setStyleSheet("QMainWindow { background-color: #f5f5f5; color: #333333; }");
        }
    }
}

#include "worktimer.h"
#include <QApplication>
#include <QStyleSheet>
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
}

void WorkTimer::toggleSettings()
{
    if (m_settingsVisible) {
        m_settingsGroup->hide();
        m_settingsVisible = false;
        m_toggleSettingsButton->setText("⚙");
        resize(300, 180);
    } else {
        m_settingsGroup->show();
        m_settingsVisible = true;
        m_toggleSettingsButton->setText("▼");
        resize(300, 520);
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
    saveSettings();
}

void WorkTimer::updateOpacity(int value)
{
    m_opacity = value / 100.0;
    setWindowOpacity(m_opacity);
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

void WorkTimer::updateSound(const QString &soundName)
{
    m_selectedSound = soundName;
    saveSettings();
}

void WorkTimer::updateVolume(int value)
{
    m_soundVolume = value / 100.0;
    m_audioOutput->setVolume(m_soundVolume);
    // Play preview sound
    playNotificationSound();
    saveSettings();
}

void WorkTimer::applyStylesheet()
{
    if (m_currentTheme == "dark") {
        setStyleSheet(R"(
            QMainWindow {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 #2c3e50, stop:1 #34495e);
                border: 2px solid #3498db;
            }
            
            QLabel {
                color: #ecf0f1;
                font-weight: bold;
            }
            
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #3498db, stop:1 #2980b9);
                border: 2px solid #2980b9;
                color: white;
                font-weight: bold;
                padding: 6px 12px;
                font-size: 11px;
                min-width: 70px;
            }
            
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #5dade2, stop:1 #3498db);
                border: 2px solid #5dade2;
            }
            
            QPushButton:pressed {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #2980b9, stop:1 #21618c);
            }
            
            QPushButton:disabled {
                background: #7f8c8d;
                border: 2px solid #95a5a6;
                color: #bdc3c7;
            }
            
            QGroupBox {
                font-weight: bold;
                font-size: 12px;
                color: #ecf0f1;
                border: 2px solid #7f8c8d;
                margin-top: 8px;
                padding-top: 8px;
                background: rgba(52, 73, 94, 0.8);
            }
            
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 8px;
                padding: 0 6px 0 6px;
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                    stop:0 #34495e, stop:1 #2c3e50);
            }
            
            QSpinBox {
                border: 2px solid #7f8c8d;
                padding: 3px;
                background: #34495e;
                font-weight: bold;
                color: #ecf0f1;
            }
            
            QSpinBox:focus {
                border: 2px solid #3498db;
            }
            
            QSpinBox::up-button, QSpinBox::down-button {
                background: #2c3e50;
                border: 1px solid #7f8c8d;
                width: 14px;
            }
            
            QSpinBox::up-button:hover, QSpinBox::down-button:hover {
                background: #34495e;
            }
            
            QPushButton#toggle_settings {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #7f8c8d, stop:1 #95a5a6);
                border: 1px solid #95a5a6;
                color: white;
                font-size: 12px;
                padding: 2px;
                min-width: 25px;
                max-width: 25px;
                min-height: 25px;
                max-height: 25px;
            }
            
            QPushButton#toggle_settings:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #95a5a6, stop:1 #bdc3c7);
            }
            
            QPushButton#close_button {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #34495e, stop:1 #2c3e50);
                border: 1px solid #7f8c8d;
                color: #bdc3c7;
                font-size: 10px;
                font-weight: bold;
                padding: 1px;
                min-width: 20px;
                max-width: 20px;
                min-height: 20px;
                max-height: 20px;
            }
            
            QPushButton#close_button:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #7f8c8d, stop:1 #95a5a6);
                border: 1px solid #95a5a6;
                color: #ecf0f1;
            }
            
            QComboBox {
                border: 2px solid #7f8c8d;
                padding: 3px;
                background: #34495e;
                color: #ecf0f1;
            }
            
            QComboBox:focus {
                border: 2px solid #3498db;
            }
            
            QSlider::groove:horizontal {
                border: 1px solid #7f8c8d;
                height: 8px;
                background: #2c3e50;
            }
            
            QSlider::handle:horizontal {
                background: #3498db;
                border: 1px solid #2980b9;
                width: 18px;
                margin: -2px 0;
            }
            
            QCheckBox {
                color: #ecf0f1;
                font-weight: bold;
            }
            
            QCheckBox::indicator {
                width: 16px;
                height: 16px;
                border: 2px solid #7f8c8d;
                background: #34495e;
            }
            
            QCheckBox::indicator:checked {
                background: #3498db;
                border: 2px solid #2980b9;
            }
            
            QPushButton#dark_theme_button {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 #2c3e50, stop:1 #34495e);
                border: 2px solid #3498db;
                border-radius: 0px;
                padding: 0px;
                margin: 0px;
                min-width: 18px;
                max-width: 18px;
                min-height: 18px;
                max-height: 18px;
                font-size: 0px;
            }
            
            QPushButton#light_theme_button {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 #ecf0f1, stop:1 #bdc3c7);
                border: 2px solid #3498db;
                border-radius: 0px;
                padding: 0px;
                margin: 0px;
                min-width: 18px;
                max-width: 18px;
                min-height: 18px;
                max-height: 18px;
                font-size: 0px;
            }
        )");
    } else { // light theme
        setStyleSheet(R"(
            QMainWindow {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 #ecf0f1, stop:1 #bdc3c7);
                border: 2px solid #3498db;
            }
            
            QLabel {
                color: #2c3e50;
                font-weight: bold;
            }
            
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #3498db, stop:1 #2980b9);
                border: 2px solid #2980b9;
                color: white;
                font-weight: bold;
                padding: 6px 12px;
                font-size: 11px;
                min-width: 70px;
            }
            
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #5dade2, stop:1 #3498db);
                border: 2px solid #5dade2;
            }
            
            QPushButton:pressed {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #2980b9, stop:1 #21618c);
            }
            
            QPushButton:disabled {
                background: #bdc3c7;
                border: 2px solid #95a5a6;
                color: #7f8c8d;
            }
            
            QGroupBox {
                font-weight: bold;
                font-size: 12px;
                color: #2c3e50;
                border: 2px solid #bdc3c7;
                margin-top: 8px;
                padding-top: 8px;
                background: rgba(255, 255, 255, 0.8);
            }
            
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 8px;
                padding: 0 6px 0 6px;
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                    stop:0 #ecf0f1, stop:1 #bdc3c7);
            }
            
            QSpinBox {
                border: 2px solid #bdc3c7;
                padding: 3px;
                background: white;
                font-weight: bold;
                color: #2c3e50;
            }
            
            QSpinBox:focus {
                border: 2px solid #3498db;
            }
            
            QSpinBox::up-button, QSpinBox::down-button {
                background: #ecf0f1;
                border: 1px solid #bdc3c7;
                width: 14px;
            }
            
            QSpinBox::up-button:hover, QSpinBox::down-button:hover {
                background: #d5dbdb;
            }
            
            QComboBox {
                border: 2px solid #bdc3c7;
                padding: 3px;
                background: white;
                color: #2c3e50;
            }
            
            QComboBox:focus {
                border: 2px solid #3498db;
            }
            
            QSlider::groove:horizontal {
                border: 1px solid #bdc3c7;
                height: 8px;
                background: #ecf0f1;
            }
            
            QSlider::handle:horizontal {
                background: #3498db;
                border: 1px solid #2980b9;
                width: 18px;
                margin: -2px 0;
            }
            
            QCheckBox {
                color: #2c3e50;
                font-weight: bold;
            }
            
            QCheckBox::indicator {
                width: 16px;
                height: 16px;
                border: 2px solid #bdc3c7;
                background: white;
            }
            
            QCheckBox::indicator:checked {
                background: #3498db;
                border: 2px solid #2980b9;
            }
            
            QPushButton#toggle_settings {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #95a5a6, stop:1 #7f8c8d);
                border: 1px solid #7f8c8d;
                color: white;
                font-size: 12px;
                padding: 2px;
                min-width: 25px;
                max-width: 25px;
                min-height: 25px;
                max-height: 25px;
            }
            
            QPushButton#toggle_settings:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #bdc3c7, stop:1 #95a5a6);
            }
            
            QPushButton#close_button {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #ecf0f1, stop:1 #bdc3c7);
                border: 1px solid #bdc3c7;
                color: #7f8c8d;
                font-size: 10px;
                font-weight: bold;
                padding: 1px;
                min-width: 20px;
                max-width: 20px;
                min-height: 20px;
                max-height: 20px;
            }
            
            QPushButton#close_button:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #bdc3c7, stop:1 #95a5a6);
                border: 1px solid #95a5a6;
                color: #2c3e50;
            }
            
            QPushButton#dark_theme_button {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 #2c3e50, stop:1 #34495e);
                border: 2px solid #3498db;
                border-radius: 0px;
                padding: 0px;
                margin: 0px;
                min-width: 18px;
                max-width: 18px;
                min-height: 18px;
                max-height: 18px;
                font-size: 0px;
            }
            
            QPushButton#light_theme_button {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 #ecf0f1, stop:1 #bdc3c7);
                border: 2px solid #3498db;
                border-radius: 0px;
                padding: 0px;
                margin: 0px;
                min-width: 18px;
                max-width: 18px;
                min-height: 18px;
                max-height: 18px;
                font-size: 0px;
            }
        )");
    }
} 
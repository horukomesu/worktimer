#include "worktimer.h"
#include "settings.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFont>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QMessageBox>
#include <QDebug>

WorkTimer::WorkTimer(QWidget *parent)
    : QMainWindow(parent)
{
    // Load UI from file
    QUiLoader loader;
    QFile uiFile(":/ui/worktimer.ui");
    
    if (!uiFile.open(QFile::ReadOnly)) {
        qDebug() << "Failed to open UI file:" << uiFile.errorString();
        return;
    }
    
    QWidget *uiWidget = loader.load(&uiFile, this);
    uiFile.close();
    
    if (!uiWidget) {
        qDebug() << "Failed to load UI file:" << loader.errorString();
        return;
    }
    
    // Set the loaded UI as central widget
    setCentralWidget(uiWidget);
    
    // Store reference to the loaded UI widget
    m_uiWidget = uiWidget;
    
    // Make window a desktop widget
    m_baseFlags = Qt::FramelessWindowHint | Qt::Tool;
    setWindowFlags(m_baseFlags | Qt::WindowStaysOnTopHint);
    
    // Set initial size
    resize(280, 200);
    
    // Initialize animations (will be set up after UI is loaded)
    m_windowAnimation = new QPropertyAnimation(this, "geometry", this);
    m_windowAnimation->setDuration(500); // 0.5 seconds
    m_windowAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    m_settingsAnimation = new QPropertyAnimation(this);
    m_settingsAnimation->setDuration(500); // 0.5 seconds
    m_settingsAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    // Create animation group for simultaneous animations
    m_animationGroup = new QParallelAnimationGroup(this);
    m_animationGroup->addAnimation(m_windowAnimation);
    m_animationGroup->addAnimation(m_settingsAnimation);
    
    // Initialize timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &WorkTimer::updateTimer);
    
    // Initialize media player
    m_mediaPlayer = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(m_soundVolume);
    
    // Load settings and setup UI connections
    loadSettings();
    loadSounds();
    setupUI();
    setupTrayIcon();
    applyStylesheet();
    updateAllIcons();
}

WorkTimer::~WorkTimer()
{
    saveSettings();
}

void WorkTimer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void WorkTimer::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && m_dragging) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void WorkTimer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
    }
}

void WorkTimer::closeEvent(QCloseEvent *event)
{
    // Instead of closing, minimize to tray
    minimizeToTray();
    event->ignore(); // Prevent the window from closing
}

void WorkTimer::loadSettings()
{
    QJsonObject settings;
    if (Settings::loadSettings(m_settingsFile, settings)) {
        // Load timer settings (convert from minutes to seconds for backward compatibility)
        int workMinutes = settings.value("work_duration").toInt(25);
        int shortBreakMinutes = settings.value("short_break_duration").toInt(5);
        int longBreakMinutes = settings.value("long_break_duration").toInt(15);
        
        // Check if settings are in new format (seconds) or old format (minutes)
        if (workMinutes > 60) {
            // New format - already in seconds
            m_workDuration = workMinutes;
            m_shortBreakDuration = shortBreakMinutes;
            m_longBreakDuration = longBreakMinutes;
        } else {
            // Old format - convert minutes to seconds
            m_workDuration = workMinutes * 60;
            m_shortBreakDuration = shortBreakMinutes * 60;
            m_longBreakDuration = longBreakMinutes * 60;
        }
        m_sessionsUntilLongBreak = settings.value("sessions_until_long_break").toInt(4);
        
        // Load appearance settings
        m_currentTheme = settings.value("current_theme").toString("dark");
        m_pinOnTop = settings.value("pin_on_top").toBool(true);
        
        // Load sound settings
        m_breakSound = settings.value("break_sound").toString("Happy bells notification.mp3");
        m_workSound = settings.value("work_sound").toString("Happy bells notification.mp3");
        m_soundVolume = settings.value("sound_volume").toDouble(0.5);
        
        // Apply settings
        m_audioOutput->setVolume(m_soundVolume);
        if (m_pinOnTop) {
            setWindowFlags(m_baseFlags | Qt::WindowStaysOnTopHint);
        } else {
            setWindowFlags(m_baseFlags);
        }
    }
}

void WorkTimer::saveSettings()
{
    QJsonObject settings;
    
    // Timer settings
    settings["work_duration"] = m_workDuration;
    settings["short_break_duration"] = m_shortBreakDuration;
    settings["long_break_duration"] = m_longBreakDuration;
    settings["sessions_until_long_break"] = m_sessionsUntilLongBreak;
    
    // Appearance settings
    settings["current_theme"] = m_currentTheme;
    settings["pin_on_top"] = m_pinOnTop;
    
    // Sound settings
    settings["break_sound"] = m_breakSound;
    settings["work_sound"] = m_workSound;
    settings["sound_volume"] = m_soundVolume;
    
    Settings::saveSettings(m_settingsFile, settings);
}

void WorkTimer::loadSounds()
{
    m_availableSounds = Settings::loadSounds(m_soundsFolder);
}

void WorkTimer::playNotificationSound(bool isBreakNotification)
{
    QString selectedSound = isBreakNotification ? m_breakSound : m_workSound;
    QString soundPath = m_soundsFolder + "/" + selectedSound;
    QFile file(soundPath);
    
    if (file.exists()) {
        m_mediaPlayer->setSource(QUrl::fromLocalFile(soundPath));
        m_mediaPlayer->play();
    } else {
        // Fallback to system sound
        QApplication::beep();
    }
}

QString WorkTimer::formatTime(int seconds)
{
    int minutes = seconds / 60;
    int secs = seconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0'));
}

void WorkTimer::updateDisplay()
{
    m_timeLabel->setText(formatTime(m_timeRemaining));
    
    if (m_isWorkSession) {
        m_sessionLabel->setText(QString("Сессия %1 - Рабочее время").arg(m_currentSession));
    } else {
        if (m_currentSession % m_sessionsUntilLongBreak == 0) {
            m_sessionLabel->setText(QString("Сессия %1 - Длинный перерыв").arg(m_currentSession));
        } else {
            m_sessionLabel->setText(QString("Сессия %1 - Короткий перерыв").arg(m_currentSession));
        }
    }
}

void WorkTimer::setupUI()
{
    if (!m_uiWidget) {
        return;
    }
    
    // Find UI elements by object names in the loaded UI widget
    // The UI file now creates a QWidget, so we can find elements directly in it
    m_timeLabel = m_uiWidget->findChild<QLabel*>("timeLabel");
    m_sessionLabel = m_uiWidget->findChild<QLabel*>("sessionLabel");
    m_startButton = m_uiWidget->findChild<QPushButton*>("startButton");
    m_pauseButton = m_uiWidget->findChild<QPushButton*>("pauseButton");
    m_resetButton = m_uiWidget->findChild<QPushButton*>("resetButton");
    m_restartButton = m_uiWidget->findChild<QPushButton*>("restartButton");
    m_toggleSettingsButton = m_uiWidget->findChild<QPushButton*>("toggle_settings");
    m_minimizeButton = m_uiWidget->findChild<QPushButton*>("minimize_button");
    m_closeButton = m_uiWidget->findChild<QPushButton*>("close_button");
    m_themeButton = m_uiWidget->findChild<QPushButton*>("theme_button");
    
    m_settingsGroup = m_uiWidget->findChild<QGroupBox*>("settingsGroup");
    m_workTimeEdit = m_uiWidget->findChild<QTimeEdit*>("workTimeEdit");
    m_shortBreakTimeEdit = m_uiWidget->findChild<QTimeEdit*>("shortBreakTimeEdit");
    m_longBreakTimeEdit = m_uiWidget->findChild<QTimeEdit*>("longBreakTimeEdit");
    m_sessionsSpinbox = m_uiWidget->findChild<QSpinBox*>("sessionsSpinbox");
    m_pinCheckbox = m_uiWidget->findChild<QCheckBox*>("pinCheckbox");
    m_breakSoundCombo = m_uiWidget->findChild<QComboBox*>("breakSoundCombo");
    m_workSoundCombo = m_uiWidget->findChild<QComboBox*>("workSoundCombo");
    m_volumeSlider = m_uiWidget->findChild<QSlider*>("volumeSlider");
    
    
    
    // Set initial values
    m_timeLabel->setText(formatTime(m_timeRemaining));
    m_sessionLabel->setText(QString("Сессия %1 - Рабочее время").arg(m_currentSession));
    
    // Set time edit values (convert seconds to minutes and seconds)
    m_workTimeEdit->setTime(QTime(0, m_workDuration / 60, m_workDuration % 60));
    m_shortBreakTimeEdit->setTime(QTime(0, m_shortBreakDuration / 60, m_shortBreakDuration % 60));
    m_longBreakTimeEdit->setTime(QTime(0, m_longBreakDuration / 60, m_longBreakDuration % 60));
    m_sessionsSpinbox->setValue(m_sessionsUntilLongBreak);
    
    // Set slider values
    m_volumeSlider->setValue(int(m_soundVolume * 100));
    
    // Set checkbox state
    m_pinCheckbox->setChecked(m_pinOnTop);
    
    // Set combo box items
    m_breakSoundCombo->addItems(m_availableSounds);
    m_workSoundCombo->addItems(m_availableSounds);
    
    if (m_availableSounds.contains(m_breakSound)) {
        m_breakSoundCombo->setCurrentText(m_breakSound);
    }
    if (m_availableSounds.contains(m_workSound)) {
        m_workSoundCombo->setCurrentText(m_workSound);
    }
    
    // Set initial icons
    m_toggleSettingsButton->setIcon(QIcon(":/new/buttons/ui/settings.svg"));
    m_toggleSettingsButton->setChecked(false);
    if (m_currentTheme == "dark") {
        m_themeButton->setIcon(QIcon(":/new/buttons/ui/dark.svg"));
        m_minimizeButton->setIcon(QIcon(":/new/buttons/ui/minimize_white.svg"));
    } else {
        m_themeButton->setIcon(QIcon(":/new/buttons/ui/light.svg"));
        m_minimizeButton->setIcon(QIcon(":/new/buttons/ui/minimize.svg"));
    }
    
    // Connect signals
    connect(m_startButton, &QPushButton::clicked, this, &WorkTimer::startTimer);
    connect(m_pauseButton, &QPushButton::clicked, this, &WorkTimer::pauseTimer);
    connect(m_resetButton, &QPushButton::clicked, this, &WorkTimer::resetTimer);
    connect(m_restartButton, &QPushButton::clicked, this, &WorkTimer::restartTimer);
    connect(m_toggleSettingsButton, &QPushButton::toggled, this, &WorkTimer::toggleSettings);
    connect(m_minimizeButton, &QPushButton::clicked, this, &WorkTimer::minimizeToTray);
    connect(m_closeButton, &QPushButton::clicked, this, &WorkTimer::closeApplication);
    
    connect(m_themeButton, &QPushButton::clicked, this, [this]() { 
        updateTheme(m_currentTheme == "dark" ? "light" : "dark"); 
    });
    
    connect(m_workTimeEdit, &QTimeEdit::timeChanged, this, &WorkTimer::updateWorkDuration);
    connect(m_shortBreakTimeEdit, &QTimeEdit::timeChanged, this, &WorkTimer::updateShortBreakDuration);
    connect(m_longBreakTimeEdit, &QTimeEdit::timeChanged, this, &WorkTimer::updateLongBreakDuration);
    connect(m_sessionsSpinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &WorkTimer::updateSessionsUntilLongBreak);
    
    connect(m_pinCheckbox, &QCheckBox::stateChanged, this, &WorkTimer::updatePinOnTop);
    connect(m_breakSoundCombo, &QComboBox::currentTextChanged, this, &WorkTimer::updateBreakSound);
    connect(m_workSoundCombo, &QComboBox::currentTextChanged, this, &WorkTimer::updateWorkSound);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &WorkTimer::updateVolume);
    connect(m_volumeSlider, &QSlider::sliderReleased, this, &WorkTimer::playVolumePreview);
    
    // Hide settings by default and set initial position
    m_settingsVisible = false;
    m_settingsGroup->hide();
    m_settingsGroup->setGeometry(QRect(m_baseWindowWidth, 0, m_settingsWidth, height()));
    
    // Setup animation targets after UI is loaded
    m_settingsAnimation->setTargetObject(m_settingsGroup);
    m_settingsAnimation->setPropertyName("geometry");
} 

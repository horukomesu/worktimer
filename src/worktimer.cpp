#include "worktimer.h"
#include "settings.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFont>
#include <QMouseEvent>
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
    setWindowTitle("WorkTimer");
    setGeometry(100, 100, 300, 280);
    setMinimumSize(280, 250);
    
    // Set window icon
    setWindowIcon(QIcon(":/worktimer_icon.ico"));
    
    // Make window a desktop widget
    m_baseFlags = Qt::FramelessWindowHint | Qt::Tool;
    setWindowFlags(m_baseFlags | Qt::WindowStaysOnTopHint);
    
    // Initialize timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &WorkTimer::updateTimer);
    
    // Initialize media player
    m_mediaPlayer = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(m_soundVolume);
    
    // Load settings and initialize UI
    loadSettings();
    loadSounds();
    initUI();
    applyStylesheet();
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

void WorkTimer::loadSettings()
{
    QJsonObject settings;
    if (Settings::loadSettings(m_settingsFile, settings)) {
        // Load timer settings
        m_workDuration = settings.value("work_duration").toInt(25);
        m_shortBreakDuration = settings.value("short_break_duration").toInt(5);
        m_longBreakDuration = settings.value("long_break_duration").toInt(15);
        m_sessionsUntilLongBreak = settings.value("sessions_until_long_break").toInt(4);
        
        // Load appearance settings
        m_currentTheme = settings.value("current_theme").toString("dark");
        m_opacity = settings.value("opacity").toDouble(1.0);
        m_pinOnTop = settings.value("pin_on_top").toBool(true);
        
        // Load sound settings
        m_selectedSound = settings.value("selected_sound").toString("Happy bells notification.mp3");
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
    settings["opacity"] = m_opacity;
    settings["pin_on_top"] = m_pinOnTop;
    
    // Sound settings
    settings["selected_sound"] = m_selectedSound;
    settings["sound_volume"] = m_soundVolume;
    
    Settings::saveSettings(m_settingsFile, settings);
}

void WorkTimer::loadSounds()
{
    m_availableSounds = Settings::loadSounds(m_soundsFolder);
}

void WorkTimer::playNotificationSound()
{
    QString soundPath = m_soundsFolder + "/" + m_selectedSound;
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

void WorkTimer::initUI()
{
    QWidget *centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(8);
    
    // Top buttons layout
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addStretch();
    
    // Theme buttons
    m_darkThemeButton = new QPushButton();
    m_darkThemeButton->setObjectName("dark_theme_button");
    m_darkThemeButton->setFixedSize(18, 18);
    connect(m_darkThemeButton, &QPushButton::clicked, this, [this]() { updateTheme("dark"); });
    topLayout->addWidget(m_darkThemeButton);
    
    m_lightThemeButton = new QPushButton();
    m_lightThemeButton->setObjectName("light_theme_button");
    m_lightThemeButton->setFixedSize(18, 18);
    connect(m_lightThemeButton, &QPushButton::clicked, this, [this]() { updateTheme("light"); });
    topLayout->addWidget(m_lightThemeButton);
    
    // Close button
    m_closeButton = new QPushButton("✕");
    m_closeButton->setObjectName("close_button");
    m_closeButton->setMaximumWidth(20);
    m_closeButton->setMaximumHeight(20);
    connect(m_closeButton, &QPushButton::clicked, this, &WorkTimer::closeApplication);
    topLayout->addWidget(m_closeButton);
    
    layout->addLayout(topLayout);
    
    // Timer display
    m_timeLabel = new QLabel(formatTime(m_timeRemaining));
    m_timeLabel->setAlignment(Qt::AlignCenter);
    QFont timeFont;
    timeFont.setPointSize(28);
    timeFont.setBold(true);
    m_timeLabel->setFont(timeFont);
    layout->addWidget(m_timeLabel);
    
    // Session info
    m_sessionLabel = new QLabel(QString("Сессия %1 - Рабочее время").arg(m_currentSession));
    m_sessionLabel->setAlignment(Qt::AlignCenter);
    QFont sessionFont;
    sessionFont.setPointSize(11);
    m_sessionLabel->setFont(sessionFont);
    layout->addWidget(m_sessionLabel);
    
    // Control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_startButton = new QPushButton("Старт");
    connect(m_startButton, &QPushButton::clicked, this, &WorkTimer::startTimer);
    buttonLayout->addWidget(m_startButton);
    
    m_pauseButton = new QPushButton("Пауза");
    connect(m_pauseButton, &QPushButton::clicked, this, &WorkTimer::pauseTimer);
    m_pauseButton->setEnabled(false);
    buttonLayout->addWidget(m_pauseButton);
    
    m_resetButton = new QPushButton("Сброс");
    connect(m_resetButton, &QPushButton::clicked, this, &WorkTimer::resetTimer);
    buttonLayout->addWidget(m_resetButton);
    
    layout->addLayout(buttonLayout);
    
    // Settings toggle button
    m_toggleSettingsButton = new QPushButton("⚙");
    m_toggleSettingsButton->setObjectName("toggle_settings");
    m_toggleSettingsButton->setMaximumWidth(25);
    m_toggleSettingsButton->setMaximumHeight(25);
    connect(m_toggleSettingsButton, &QPushButton::clicked, this, &WorkTimer::toggleSettings);
    layout->addWidget(m_toggleSettingsButton);
    
    // Settings group
    m_settingsGroup = new QGroupBox("Настройки");
    QGridLayout *settingsLayout = new QGridLayout(m_settingsGroup);
    
    // Work duration
    settingsLayout->addWidget(new QLabel("Рабочее время (мин):"), 0, 0);
    m_workSpinbox = new QSpinBox();
    m_workSpinbox->setRange(1, 60);
    m_workSpinbox->setValue(m_workDuration);
    connect(m_workSpinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &WorkTimer::updateWorkDuration);
    settingsLayout->addWidget(m_workSpinbox, 0, 1);
    
    // Short break duration
    settingsLayout->addWidget(new QLabel("Короткий перерыв (мин):"), 1, 0);
    m_shortBreakSpinbox = new QSpinBox();
    m_shortBreakSpinbox->setRange(1, 30);
    m_shortBreakSpinbox->setValue(m_shortBreakDuration);
    connect(m_shortBreakSpinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &WorkTimer::updateShortBreakDuration);
    settingsLayout->addWidget(m_shortBreakSpinbox, 1, 1);
    
    // Long break duration
    settingsLayout->addWidget(new QLabel("Длинный перерыв (мин):"), 2, 0);
    m_longBreakSpinbox = new QSpinBox();
    m_longBreakSpinbox->setRange(1, 60);
    m_longBreakSpinbox->setValue(m_longBreakDuration);
    connect(m_longBreakSpinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &WorkTimer::updateLongBreakDuration);
    settingsLayout->addWidget(m_longBreakSpinbox, 2, 1);
    
    // Sessions until long break
    settingsLayout->addWidget(new QLabel("Сессий до длинного перерыва:"), 3, 0);
    m_sessionsSpinbox = new QSpinBox();
    m_sessionsSpinbox->setRange(2, 10);
    m_sessionsSpinbox->setValue(m_sessionsUntilLongBreak);
    connect(m_sessionsSpinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &WorkTimer::updateSessionsUntilLongBreak);
    settingsLayout->addWidget(m_sessionsSpinbox, 3, 1);
    
    // Opacity slider
    settingsLayout->addWidget(new QLabel("Прозрачность:"), 4, 0);
    m_opacitySlider = new QSlider(Qt::Horizontal);
    m_opacitySlider->setRange(20, 100);
    m_opacitySlider->setValue(int(m_opacity * 100));
    connect(m_opacitySlider, &QSlider::valueChanged, this, &WorkTimer::updateOpacity);
    settingsLayout->addWidget(m_opacitySlider, 4, 1);
    
    // Pin on top checkbox
    m_pinCheckbox = new QCheckBox("Поверх всех окон");
    m_pinCheckbox->setChecked(m_pinOnTop);
    connect(m_pinCheckbox, &QCheckBox::stateChanged, this, &WorkTimer::updatePinOnTop);
    settingsLayout->addWidget(m_pinCheckbox, 5, 0, 1, 2);
    
    // Sound selection
    settingsLayout->addWidget(new QLabel("Звук уведомления:"), 6, 0, 1, 2);
    m_soundCombo = new QComboBox();
    m_soundCombo->addItems(m_availableSounds);
    if (m_availableSounds.contains(m_selectedSound)) {
        m_soundCombo->setCurrentText(m_selectedSound);
    }
    connect(m_soundCombo, &QComboBox::currentTextChanged, this, &WorkTimer::updateSound);
    settingsLayout->addWidget(m_soundCombo, 7, 0, 1, 2);
    
    // Volume slider
    settingsLayout->addWidget(new QLabel("Громкость звука:"), 8, 0);
    m_volumeSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(int(m_soundVolume * 100));
    connect(m_volumeSlider, &QSlider::valueChanged, this, &WorkTimer::updateVolume);
    connect(m_volumeSlider, &QSlider::sliderReleased, this, &WorkTimer::playVolumePreview);
    settingsLayout->addWidget(m_volumeSlider, 8, 1);
    
    layout->addWidget(m_settingsGroup);
    
    // Hide settings by default
    m_settingsVisible = false;
    m_settingsGroup->hide();
} 
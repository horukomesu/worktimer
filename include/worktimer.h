#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QTimeEdit>
#include <QTime>
#include <QGroupBox>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPoint>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QFont>
#include <QMouseEvent>
#include <QUiLoader>
#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class WorkTimer : public QMainWindow
{
    Q_OBJECT

public:
    WorkTimer(QWidget *parent = nullptr);
    ~WorkTimer();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void startTimer();
    void pauseTimer();
    void resetTimer();
    void restartTimer();
    void updateTimer();
    void timerFinished();
    void toggleSettings(bool checked = false);
    void closeApplication();
    void minimizeToTray();
    void restoreFromTray();
    
    void updateWorkDuration(const QTime &time);
    void updateShortBreakDuration(const QTime &time);
    void updateLongBreakDuration(const QTime &time);
    void updateSessionsUntilLongBreak(int value);
    void updateTheme(const QString &theme);
    void updatePinOnTop(int state);
    void updateAllIcons();
    void updateBreakSound(const QString &soundName);
    void updateWorkSound(const QString &soundName);
    void updateVolume(int value);
    void playVolumePreview();

private:
    void setupUI();
    void applyStylesheet();
    void loadSettings();
    void saveSettings();
    void loadSounds();
    void playNotificationSound(bool isBreakNotification = true);
    QString formatTime(int seconds);
    void updateDisplay();
    void setupTrayIcon();

    // Window properties
    bool m_dragging = false;
    QPoint m_dragPosition;
    Qt::WindowFlags m_baseFlags;
    
    // Timer settings (in seconds)
    int m_workDuration = 25 * 60;
    int m_shortBreakDuration = 5 * 60;
    int m_longBreakDuration = 15 * 60;
    int m_sessionsUntilLongBreak = 4;
    
    // Appearance settings
    QString m_currentTheme = "dark";
    bool m_pinOnTop = true;
    
    // Sound settings
    QString m_soundsFolder = "sounds";
    QStringList m_availableSounds;
    QString m_breakSound = "Happy bells notification.mp3";
    QString m_workSound = "Happy bells notification.mp3";
    double m_soundVolume = 0.5;
    
    // Settings file
    QString m_settingsFile = "worktimer_settings.json";
    
    // Timer state
    int m_currentSession = 1;
    bool m_isRunning = false;
    bool m_isWorkSession = true;
    int m_timeRemaining = 25 * 60; // Convert to seconds
    bool m_settingsVisible = false;
    
    // UI elements
    QTimer *m_timer;
    QMediaPlayer *m_mediaPlayer;
    QAudioOutput *m_audioOutput;
    
    // UI widget loaded from .ui file
    QWidget *m_uiWidget;
    
    // UI elements loaded from .ui file
    QLabel *m_timeLabel;
    QLabel *m_sessionLabel;
    QPushButton *m_startButton;
    QPushButton *m_pauseButton;
    QPushButton *m_resetButton;
    QPushButton *m_restartButton;
    QPushButton *m_toggleSettingsButton;
    QPushButton *m_minimizeButton;
    QPushButton *m_closeButton;
    QPushButton *m_themeButton;
    
    QGroupBox *m_settingsGroup;
    QTimeEdit *m_workTimeEdit;
    QTimeEdit *m_shortBreakTimeEdit;
    QTimeEdit *m_longBreakTimeEdit;
    QSpinBox *m_sessionsSpinbox;
    QCheckBox *m_pinCheckbox;
    QComboBox *m_breakSoundCombo;
    QComboBox *m_workSoundCombo;
    QSlider *m_volumeSlider;
    
    // Animation
    QPropertyAnimation *m_windowAnimation;
    QPropertyAnimation *m_settingsAnimation;
    QParallelAnimationGroup *m_animationGroup;
    bool m_isAnimating = false;
    int m_settingsWidth = 280;
    int m_baseWindowWidth = 342;
    
    // System tray
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;
    QAction *m_restoreAction;
    QAction *m_quitAction;
}; 

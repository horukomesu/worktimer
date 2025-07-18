import sys
import os
import winsound
import json
from PySide6.QtWidgets import (QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, 
                               QWidget, QPushButton, QLabel, QSpinBox, QGroupBox,
                               QGridLayout, QSlider, QCheckBox, QComboBox)
from PySide6.QtCore import QTimer, Qt, QPoint, QUrl
from PySide6.QtGui import QFont
from PySide6.QtMultimedia import QMediaPlayer, QAudioOutput


class PomodoroTimer(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("WorkTimer")
        self.setGeometry(100, 100, 300, 280)
        self.setMinimumSize(280, 250)
        
        # Делаем окно виджетом рабочего стола
        self.base_flags = Qt.WindowType.FramelessWindowHint | Qt.WindowType.Tool
        self.setWindowFlags(self.base_flags | Qt.WindowType.WindowStaysOnTopHint)
        

        
        # Переменные для перетаскивания окна
        self.dragging = False
        self.drag_position = QPoint()
        
        # Timer settings (in minutes)
        self.work_duration = 25
        self.short_break_duration = 5
        self.long_break_duration = 15
        self.sessions_until_long_break = 4
        
        # Appearance settings
        self.current_theme = "dark"  # "dark" or "light"
        self.opacity = 1.0  # 0.0 to 1.0
        self.pin_on_top = True
        
        # Settings file
        self.settings_file = "worktimer_settings.json"
        
        # Sound settings
        self.sounds_folder = "sounds"
        self.available_sounds = []
        self.selected_sound = "Happy bells notification.mp3"
        self.sound_volume = 0.5  # 0.0 to 1.0
        self.media_player = QMediaPlayer()
        self.audio_output = QAudioOutput()
        self.media_player.setAudioOutput(self.audio_output)
        self.audio_output.setVolume(self.sound_volume)
        
        # Timer state
        self.current_session = 1
        self.is_running = False
        self.is_work_session = True
        self.time_remaining = self.work_duration * 60  # Convert to seconds
        
        # Create timer
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_timer)
        
        self.load_settings()
        self.load_sounds()
        self.init_ui()
        self.apply_stylesheet()
        
    def mousePressEvent(self, event):
        """Обработка нажатия мыши для перетаскивания окна"""
        if event.button() == Qt.MouseButton.LeftButton:
            self.dragging = True
            self.drag_position = event.globalPosition().toPoint() - self.frameGeometry().topLeft()
            event.accept()
            
    def mouseMoveEvent(self, event):
        """Обработка движения мыши для перетаскивания окна"""
        if event.buttons() == Qt.MouseButton.LeftButton and self.dragging:
            self.move(event.globalPosition().toPoint() - self.drag_position)
            event.accept()
            
    def mouseReleaseEvent(self, event):
        """Обработка отпускания мыши"""
        if event.button() == Qt.MouseButton.LeftButton:
            self.dragging = False
            
    def close_application(self):
        """Полностью закрывает приложение"""
        self.save_settings()  # Сохраняем настройки перед закрытием
        self.timer.stop()  # Останавливаем таймер
        QApplication.quit()  # Завершаем приложение
        
    def load_settings(self):
        """Загружает настройки из файла"""
        try:
            if os.path.exists(self.settings_file):
                with open(self.settings_file, 'r', encoding='utf-8') as f:
                    settings = json.load(f)
                    
                # Загружаем настройки таймера
                self.work_duration = settings.get('work_duration', 25)
                self.short_break_duration = settings.get('short_break_duration', 5)
                self.long_break_duration = settings.get('long_break_duration', 15)
                self.sessions_until_long_break = settings.get('sessions_until_long_break', 4)
                
                # Загружаем настройки интерфейса
                self.current_theme = settings.get('current_theme', 'dark')
                self.opacity = settings.get('opacity', 1.0)
                self.pin_on_top = settings.get('pin_on_top', True)
                
                # Загружаем настройки звука
                self.selected_sound = settings.get('selected_sound', 'Happy bells notification.mp3')
                self.sound_volume = settings.get('sound_volume', 0.5)
                
                # Применяем настройки
                self.audio_output.setVolume(self.sound_volume)
                if self.pin_on_top:
                    self.setWindowFlags(self.base_flags | Qt.WindowType.WindowStaysOnTopHint)
                else:
                    self.setWindowFlags(self.base_flags)
        except Exception as e:
            pass  # Если файл не найден или поврежден, используем значения по умолчанию
            
    def save_settings(self):
        """Сохраняет настройки в файл"""
        try:
            settings = {
                # Настройки таймера
                'work_duration': self.work_duration,
                'short_break_duration': self.short_break_duration,
                'long_break_duration': self.long_break_duration,
                'sessions_until_long_break': self.sessions_until_long_break,
                
                # Настройки интерфейса
                'current_theme': self.current_theme,
                'opacity': self.opacity,
                'pin_on_top': self.pin_on_top,
                
                # Настройки звука
                'selected_sound': self.selected_sound,
                'sound_volume': self.sound_volume
            }
            
            with open(self.settings_file, 'w', encoding='utf-8') as f:
                json.dump(settings, f, indent=2, ensure_ascii=False)
        except Exception as e:
            pass  # Если не удалось сохранить, просто игнорируем
        
    def load_sounds(self):
        """Загружает список доступных звуков из папки sounds"""
        self.available_sounds = []
        current_dir = os.getcwd()
        sounds_path = os.path.join(current_dir, self.sounds_folder)
        
        if os.path.exists(sounds_path):
            for file in os.listdir(sounds_path):
                if file.endswith('.mp3'):
                    self.available_sounds.append(file)
        
        
    def play_notification_sound(self):
        """Воспроизводит выбранный звук уведомления"""
        try:
            sound_path = os.path.join(self.sounds_folder, self.selected_sound)
            if os.path.exists(sound_path):
                # Используем QUrl для корректного пути
                self.media_player.setSource(QUrl.fromLocalFile(sound_path))
                self.media_player.play()
            else:
                # Альтернативный способ - системный звук
                winsound.MessageBeep(winsound.MB_ICONEXCLAMATION)
        except Exception as e:
            # Если QMediaPlayer не работает, используем системный звук
            try:
                winsound.MessageBeep(winsound.MB_ICONEXCLAMATION)
            except:
                pass  # Если и это не работает, просто молчим
        
    def init_ui(self):
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        layout = QVBoxLayout(central_widget)
        layout.setContentsMargins(15, 15, 15, 15)
        layout.setSpacing(8)
        
        # Кнопки в верхней части окна
        top_layout = QHBoxLayout()
        top_layout.addStretch()  # Добавляем растяжку слева
        
        # Кнопки тем
        self.dark_theme_button = QPushButton()
        self.dark_theme_button.setObjectName("dark_theme_button")
        self.dark_theme_button.setFixedSize(18, 18)
        self.dark_theme_button.clicked.connect(lambda: self.update_theme("dark"))
        top_layout.addWidget(self.dark_theme_button)
        
        self.light_theme_button = QPushButton()
        self.light_theme_button.setObjectName("light_theme_button")
        self.light_theme_button.setFixedSize(18, 18)
        self.light_theme_button.clicked.connect(lambda: self.update_theme("light"))
        top_layout.addWidget(self.light_theme_button)
        
        # Кнопка закрытия (крестик)
        self.close_button = QPushButton("✕")
        self.close_button.setObjectName("close_button")
        self.close_button.setMaximumWidth(20)
        self.close_button.setMaximumHeight(20)
        self.close_button.clicked.connect(self.close_application)
        top_layout.addWidget(self.close_button)
        
        layout.addLayout(top_layout)
        
        # Timer display
        self.time_label = QLabel(self.format_time(self.time_remaining))
        self.time_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        font = QFont()
        font.setPointSize(28)
        font.setBold(True)
        self.time_label.setFont(font)
        layout.addWidget(self.time_label)
        
        # Session info
        self.session_label = QLabel(f"Сессия {self.current_session} - Рабочее время")
        self.session_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        session_font = QFont()
        session_font.setPointSize(11)
        self.session_label.setFont(session_font)
        layout.addWidget(self.session_label)
        
        # Control buttons
        button_layout = QHBoxLayout()
        
        self.start_button = QPushButton("Старт")
        self.start_button.clicked.connect(self.start_timer)
        button_layout.addWidget(self.start_button)
        
        self.pause_button = QPushButton("Пауза")
        self.pause_button.clicked.connect(self.pause_timer)
        self.pause_button.setEnabled(False)
        button_layout.addWidget(self.pause_button)
        
        self.reset_button = QPushButton("Сброс")
        self.reset_button.clicked.connect(self.reset_timer)
        button_layout.addWidget(self.reset_button)
        
        layout.addLayout(button_layout)
        
        # Кнопка сворачивания/разворачивания настроек
        self.toggle_settings_button = QPushButton("⚙")
        self.toggle_settings_button.setObjectName("toggle_settings")
        self.toggle_settings_button.setMaximumWidth(25)
        self.toggle_settings_button.setMaximumHeight(25)
        self.toggle_settings_button.clicked.connect(self.toggle_settings)
        layout.addWidget(self.toggle_settings_button)
        
        # Settings group
        self.settings_group = QGroupBox("Настройки")
        settings_layout = QGridLayout(self.settings_group)
        
        # Work duration setting
        settings_layout.addWidget(QLabel("Рабочее время (мин):"), 0, 0)
        self.work_spinbox = QSpinBox()
        self.work_spinbox.setRange(1, 60)
        self.work_spinbox.setValue(self.work_duration)
        self.work_spinbox.valueChanged.connect(self.update_work_duration)
        settings_layout.addWidget(self.work_spinbox, 0, 1)
        
        # Short break duration setting
        settings_layout.addWidget(QLabel("Короткий перерыв (мин):"), 1, 0)
        self.short_break_spinbox = QSpinBox()
        self.short_break_spinbox.setRange(1, 30)
        self.short_break_spinbox.setValue(self.short_break_duration)
        self.short_break_spinbox.valueChanged.connect(self.update_short_break_duration)
        settings_layout.addWidget(self.short_break_spinbox, 1, 1)
        
        # Long break duration setting
        settings_layout.addWidget(QLabel("Длинный перерыв (мин):"), 2, 0)
        self.long_break_spinbox = QSpinBox()
        self.long_break_spinbox.setRange(1, 60)
        self.long_break_spinbox.setValue(self.long_break_duration)
        self.long_break_spinbox.valueChanged.connect(self.update_long_break_duration)
        settings_layout.addWidget(self.long_break_spinbox, 2, 1)
        
        # Sessions until long break setting
        settings_layout.addWidget(QLabel("Сессий до длинного перерыва:"), 3, 0)
        self.sessions_spinbox = QSpinBox()
        self.sessions_spinbox.setRange(2, 10)
        self.sessions_spinbox.setValue(self.sessions_until_long_break)
        self.sessions_spinbox.valueChanged.connect(self.update_sessions_until_long_break)
        settings_layout.addWidget(self.sessions_spinbox, 3, 1)
        

        
        # Opacity slider
        settings_layout.addWidget(QLabel("Прозрачность:"), 4, 0)
        self.opacity_slider = QSlider(Qt.Orientation.Horizontal)
        self.opacity_slider.setRange(20, 100)
        self.opacity_slider.setValue(int(self.opacity * 100))
        self.opacity_slider.valueChanged.connect(self.update_opacity)
        settings_layout.addWidget(self.opacity_slider, 4, 1)
        
        # Pin on top checkbox
        self.pin_checkbox = QCheckBox("Поверх всех окон")
        self.pin_checkbox.setChecked(self.pin_on_top)
        self.pin_checkbox.stateChanged.connect(self.update_pin_on_top)
        settings_layout.addWidget(self.pin_checkbox, 5, 0, 1, 2)
        
        # Sound selection
        settings_layout.addWidget(QLabel("Звук уведомления:"), 6, 0, 1, 2)
        self.sound_combo = QComboBox()
        self.sound_combo.addItems(self.available_sounds)
        if self.selected_sound in self.available_sounds:
            self.sound_combo.setCurrentText(self.selected_sound)
        self.sound_combo.currentTextChanged.connect(self.update_sound)
        settings_layout.addWidget(self.sound_combo, 7, 0, 1, 2)
        
        # Volume slider
        settings_layout.addWidget(QLabel("Громкость звука:"), 8, 0)
        self.volume_slider = QSlider(Qt.Orientation.Horizontal)
        self.volume_slider.setRange(0, 100)
        self.volume_slider.setValue(int(self.sound_volume * 100))
        self.volume_slider.valueChanged.connect(self.update_volume)
        settings_layout.addWidget(self.volume_slider, 8, 1)
        
        layout.addWidget(self.settings_group)
        
        # Скрываем настройки по умолчанию
        self.settings_visible = False
        self.settings_group.hide()
        
    def toggle_settings(self):
        """Сворачивает/разворачивает панель настроек"""
        if self.settings_visible:
            self.settings_group.hide()
            self.settings_visible = False
            self.toggle_settings_button.setText("⚙")
            self.resize(300, 180)
        else:
            self.settings_group.show()
            self.settings_visible = True
            self.toggle_settings_button.setText("▼")
            self.resize(300, 520)
        
    def apply_stylesheet(self):
        """Применяет красивый стиль к приложению"""
        if self.current_theme == "dark":
            self.setStyleSheet("""
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
        """)
        else:  # light theme
            self.setStyleSheet("""
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
            """)
        
    def format_time(self, seconds):
        minutes = seconds // 60
        seconds = seconds % 60
        return f"{minutes:02d}:{seconds:02d}"
        
    def start_timer(self):
        self.is_running = True
        self.timer.start(1000)  # Update every second
        self.start_button.setEnabled(False)
        self.pause_button.setEnabled(True)
        
    def pause_timer(self):
        self.is_running = False
        self.timer.stop()
        self.start_button.setEnabled(True)
        self.pause_button.setEnabled(False)
        
    def reset_timer(self):
        self.pause_timer()
        self.current_session = 1
        self.is_work_session = True
        self.time_remaining = self.work_duration * 60
        self.update_display()
        
    def update_timer(self):
        if self.time_remaining > 0:
            self.time_remaining -= 1
            self.update_display()
        else:
            self.timer_finished()
            
    def timer_finished(self):
        self.pause_timer()
        self.play_notification_sound()
        
        if self.is_work_session:
            # Work session finished, start break
            if self.current_session % self.sessions_until_long_break == 0:
                # Long break
                self.time_remaining = self.long_break_duration * 60
                self.session_label.setText(f"Сессия {self.current_session} - Длинный перерыв")
            else:
                # Short break
                self.time_remaining = self.short_break_duration * 60
                self.session_label.setText(f"Сессия {self.current_session} - Короткий перерыв")
            self.is_work_session = False
        else:
            # Break finished, start next work session
            self.current_session += 1
            self.time_remaining = self.work_duration * 60
            self.session_label.setText(f"Сессия {self.current_session} - Рабочее время")
            self.is_work_session = True
            
        self.update_display()
        
    def update_display(self):
        self.time_label.setText(self.format_time(self.time_remaining))
        if self.is_work_session:
            self.session_label.setText(f"Сессия {self.current_session} - Рабочее время")
        else:
            if self.current_session % self.sessions_until_long_break == 0:
                self.session_label.setText(f"Сессия {self.current_session} - Длинный перерыв")
            else:
                self.session_label.setText(f"Сессия {self.current_session} - Короткий перерыв")
                
    def update_work_duration(self, value):
        self.work_duration = value
        if self.is_work_session and not self.is_running:
            self.time_remaining = self.work_duration * 60
            self.update_display()
        self.save_settings()
            
    def update_short_break_duration(self, value):
        self.short_break_duration = value
        self.save_settings()
        
    def update_long_break_duration(self, value):
        self.long_break_duration = value
        self.save_settings()
        
    def update_sessions_until_long_break(self, value):
        self.sessions_until_long_break = value
        self.save_settings()
        
    def update_theme(self, theme):
        """Обновляет цветовую схему"""
        self.current_theme = theme
        self.apply_stylesheet()
        self.save_settings()
        
    def update_opacity(self, value):
        """Обновляет прозрачность окна"""
        self.opacity = value / 100.0
        self.setWindowOpacity(self.opacity)
        self.save_settings()
        
    def update_pin_on_top(self, state):
        """Обновляет настройку 'поверх всех окон'"""
        self.pin_on_top = state == Qt.CheckState.Checked
        if self.pin_on_top:
            self.setWindowFlags(self.base_flags | Qt.WindowType.WindowStaysOnTopHint)
        else:
            self.setWindowFlags(self.base_flags)
        self.show()  # Пересоздаем окно с новыми флагами
        self.save_settings()
        
    def update_sound(self, sound_name):
        """Обновляет выбранный звук уведомления"""
        self.selected_sound = sound_name
        self.save_settings()
        
    def update_volume(self, value):
        """Обновляет громкость звука и проигрывает превью"""
        self.sound_volume = value / 100.0
        self.audio_output.setVolume(self.sound_volume)
        # Проигрываем превью звука
        self.play_notification_sound()
        self.save_settings()


def main():
    app = QApplication(sys.argv)
    window = PomodoroTimer()
    window.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()


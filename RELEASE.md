# WorkTimer v2.1.0

## 🎯 Key Changes

### ✨ New Features
- **Language Toggle Switch**: Added a stylish toggle switch to switch between Russian and English languages
- **Taskbar Integration**: New "Show in taskbar" option to control whether the app appears in the taskbar or system tray
- **Extended Timer Support**: Timer now supports durations longer than 1 hour
- **Improved Settings Loading**: Settings now load before UI creation to ensure correct initial values

### 🔧 Technical Improvements
- **Qt Translation System**: Implemented proper internationalization using Qt's translation framework
- **Enhanced UI Initialization**: Fixed order of initialization to prevent crashes and ensure proper loading
- **Better Tray Management**: Improved system tray behavior and notifications
- **Code Cleanup**: Removed debug messages for cleaner production code

### 🎨 UI/UX Enhancements
- **Compact Language Switcher**: Small, modern toggle switch (40x20px) with RU/EN text
- **Shortened Labels**: Optimized checkbox texts for better UI fit in both languages
- **Responsive Design**: Better handling of window states and animations
- **Theme Consistency**: Updated styles for both dark and light themes

### 🌐 Internationalization
- **Russian Translation**: Complete Russian localization with shortened UI texts
- **English Translation**: Full English localization with optimized labels
- **Dynamic Language Switching**: Real-time language switching without app restart
- **Translation Files**: Compiled .qm files included in resources

## 📦 Installation
1. Download `WorkTimerSetup.exe` from the Releases section
2. Run the installer
3. Follow the installation instructions

## 🎵 Sounds
All sound files are located in the `sounds/` folder. You can add your own sounds in MP3 format.

## 🔧 Settings
Settings are saved in `worktimer_settings.json` and include:
- Work session and break durations
- Selected notification sounds
- Theme preferences
- Language preference
- Taskbar/tray display options
- Other user preferences

---

# WorkTimer v2.0.0

## 🎯 Основные изменения

### ✨ Новые возможности
- **Раздельные звуки уведомлений**: Теперь можно выбрать разные звуки для уведомлений о перерыве и о возвращении к работе
- **Улучшенный ввод времени**: Время теперь вводится в формате мин:сек (например, 0:30, 1:50) вместо только минут
- **Стилизованные QTimeEdit**: Добавлены стили для новых элементов ввода времени в темную и светлую темы

### 🔧 Технические улучшения
- Заменены QSpinBox на QTimeEdit для полей времени
- Обновлена логика работы со временем (теперь в секундах)
- Добавлена обратная совместимость со старыми настройками
- Улучшена структура кода

### 🎨 UI/UX
- Более интуитивный ввод времени
- Лучшая персонализация звуковых уведомлений
- Сохранение единообразия дизайна

## 📦 Установка
1. Скачайте `WorkTimerSetup.exe` из раздела Releases
2. Запустите установщик
3. Следуйте инструкциям установки

## 🎵 Звуки
Все звуковые файлы находятся в папке `sounds/`. Вы можете добавить свои звуки в формате MP3.

## 🔧 Настройки
Настройки сохраняются в файл `worktimer_settings.json` и включают:
- Длительность рабочих сессий и перерывов
- Выбранные звуки для уведомлений
- Тему оформления
- Другие пользовательские настройки 
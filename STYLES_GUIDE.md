# Руководство по стилям QSS

## Обзор

Проект теперь использует отдельные QSS файлы для стилизации интерфейса. Это позволяет:
- Разделить логику и стили
- Легко изменять внешний вид без изменения кода
- Создавать новые темы
- Использовать современные CSS-подобные стили

## Структура стилей

```
styles/
├── dark.qss      # Темная тема
└── light.qss     # Светлая тема
```

## Цветовые схемы

### Темная тема (`dark.qss`)
- **Основной фон**: `#2b2b2b`
- **Вторичный фон**: `#333333`
- **Элементы**: `#404040`
- **Текст**: `#ffffff`
- **Акцент**: `#4CAF50` (зеленый)
- **Кнопки**: `#404040` → `#505050` (hover)
- **Границы**: `#555555`

### Светлая тема (`light.qss`)
- **Основной фон**: `#f5f5f5`
- **Вторичный фон**: `#ffffff`
- **Элементы**: `#ffffff`
- **Текст**: `#333333`
- **Акцент**: `#2196F3` (синий)
- **Кнопки**: `#ffffff` → `#f0f0f0` (hover)
- **Границы**: `#d0d0d0`

## Основные элементы

### QMainWindow
```css
QMainWindow {
    background-color: #2b2b2b;  /* или #f5f5f5 */
    color: #ffffff;             /* или #333333 */
    border: 1px solid #404040;  /* или #d0d0d0 */
    border-radius: 8px;
}
```

### QLabel
```css
QLabel {
    background-color: transparent;
    color: #ffffff;  /* или #333333 */
    border: none;
}

QLabel#timeLabel {
    font-size: 28px;
    font-weight: bold;
    color: #4CAF50;  /* или #2196F3 */
}
```

### QPushButton
```css
QPushButton {
    background-color: #404040;  /* или #ffffff */
    color: #ffffff;             /* или #333333 */
    border: 1px solid #555555;  /* или #d0d0d0 */
    border-radius: 4px;
    padding: 8px 16px;
    font-size: 12px;
    font-weight: bold;
}

QPushButton:hover {
    background-color: #505050;  /* или #f0f0f0 */
    border-color: #666666;      /* или #b0b0b0 */
}
```

### Специальные кнопки
```css
QPushButton#close_button {
    background-color: #e74c3c;
    border-color: #c0392b;
    color: white;
    border-radius: 10px;
}

QPushButton#dark_theme_button {
    background-color: #4CAF50;
    border-color: #45a049;
    border-radius: 9px;
}

QPushButton#light_theme_button {
    background-color: #2196F3;
    border-color: #1976D2;
    border-radius: 9px;
}
```

### QGroupBox
```css
QGroupBox {
    background-color: #333333;  /* или #ffffff */
    border: 1px solid #555555;  /* или #d0d0d0 */
    border-radius: 6px;
    margin-top: 10px;
    padding-top: 10px;
    font-weight: bold;
}
```

### QSpinBox
```css
QSpinBox {
    background-color: #404040;  /* или #ffffff */
    color: #ffffff;             /* или #333333 */
    border: 1px solid #555555;  /* или #d0d0d0 */
    border-radius: 4px;
    padding: 4px;
    selection-background-color: #4CAF50;  /* или #2196F3 */
}
```

### QSlider
```css
QSlider::groove:horizontal {
    border: 1px solid #555555;  /* или #d0d0d0 */
    height: 8px;
    background: #404040;        /* или #f0f0f0 */
    border-radius: 4px;
}

QSlider::handle:horizontal {
    background: #4CAF50;        /* или #2196F3 */
    border: 1px solid #45a049;  /* или #1976D2 */
    width: 18px;
    margin: -2px 0;
    border-radius: 9px;
}
```

### QCheckBox
```css
QCheckBox {
    color: #ffffff;  /* или #333333 */
    spacing: 8px;
}

QCheckBox::indicator {
    width: 16px;
    height: 16px;
    border: 1px solid #555555;  /* или #d0d0d0 */
    border-radius: 3px;
    background-color: #404040;  /* или #ffffff */
}

QCheckBox::indicator:checked {
    background-color: #4CAF50;  /* или #2196F3 */
    border-color: #45a049;      /* или #1976D2 */
}
```

### QComboBox
```css
QComboBox {
    background-color: #404040;  /* или #ffffff */
    color: #ffffff;             /* или #333333 */
    border: 1px solid #555555;  /* или #d0d0d0 */
    border-radius: 4px;
    padding: 4px;
    min-width: 100px;
}

QComboBox QAbstractItemView {
    background-color: #404040;  /* или #ffffff */
    color: #ffffff;             /* или #333333 */
    border: 1px solid #555555;  /* или #d0d0d0 */
    selection-background-color: #4CAF50;  /* или #2196F3 */
    selection-color: #ffffff;
}
```

## Создание новой темы

### 1. Создайте новый QSS файл
```bash
# Создайте файл styles/new_theme.qss
```

### 2. Определите цветовую схему
```css
/* New Theme Stylesheet for WorkTimer */

QMainWindow {
    background-color: #your_color;
    color: #your_text_color;
    border: 1px solid #your_border_color;
    border-radius: 8px;
}

/* Скопируйте остальные стили и измените цвета */
```

### 3. Добавьте в ресурсы
```xml
<!-- В resources.qrc -->
<file>styles/new_theme.qss</file>
```

### 4. Обновите код
```cpp
// В applyStylesheet()
if (m_currentTheme == "new_theme") {
    styleFile = ":/styles/new_theme.qss";
}
```

## Советы по дизайну

### Контрастность
- Убедитесь, что текст читаем на фоне
- Используйте контрастные цвета для важных элементов
- Следуйте принципам доступности

### Согласованность
- Используйте единую цветовую схему
- Поддерживайте одинаковые отступы и размеры
- Следуйте единому стилю для всех элементов

### Современность
- Используйте скругленные углы (`border-radius`)
- Добавьте эффекты при наведении (`:hover`)
- Используйте полупрозрачность для глубины

## Отладка стилей

### Проверка загрузки
```cpp
QFile file(":/styles/dark.qss");
if (file.open(QFile::ReadOnly | QFile::Text)) {
    qDebug() << "Stylesheet loaded successfully";
} else {
    qDebug() << "Failed to load stylesheet:" << file.errorString();
}
```

### Проверка применения
```cpp
qDebug() << "Current stylesheet:" << styleSheet();
```

### Временное отключение
```cpp
// Временно отключить стили для отладки
setStyleSheet("");
```

## Полезные ссылки

- [Qt Style Sheets Reference](https://doc.qt.io/qt-6/stylesheet.html)
- [Qt Style Sheets Examples](https://doc.qt.io/qt-6/stylesheet-examples.html)
- [CSS Color Picker](https://www.w3schools.com/colors/colors_picker.asp)
- [Material Design Colors](https://material.io/design/color/the-color-system.html) 
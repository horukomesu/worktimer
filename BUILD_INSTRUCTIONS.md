# Инструкции по сборке WorkTimer

## Обзор изменений

Теперь `windeployqt` копирует DLL файлы в папку `build\bin\Release` вместо `build\bin`, что обеспечивает правильную организацию файлов.

## Скрипты сборки

### 1. build_cpp.bat
Основной скрипт для полной сборки проекта:
- Собирает проект с помощью CMake
- Автоматически запускает деплой Qt зависимостей в папку Release
- Очищает папку bin от лишних файлов
- Копирует папку sounds в Release

**Использование:**
```bash
build_cpp.bat
```

### 2. deploy_qt.bat
Скрипт для деплоя Qt зависимостей:
- Копирует только необходимые DLL файлы в `build\bin\Release`
- Очищает папку Release от старых файлов перед деплоем
- Использует флаги для исключения ненужных компонентов

**Использование:**
```bash
deploy_qt.bat
```

### 3. clean_bin.bat
Скрипт для очистки папки `build\bin`:
- Удаляет все DLL файлы и папки Qt из `build\bin`
- Оставляет только exe файл и настройки
- Используется автоматически в `build_cpp.bat`

**Использование:**
```bash
clean_bin.bat
```

### 4. build_installer.bat
Скрипт для сборки инсталлятора:
- Проверяет наличие файлов в папке Release
- Собирает инсталлятор с помощью Inno Setup
- Включает все необходимые DLL и папки Qt

**Использование:**
```bash
build_installer.bat
```

## Структура папок после сборки

```
build/
├── bin/
│   ├── WorkTimer.exe          # Основной exe файл (без зависимостей)
│   └── worktimer_settings.json
└── bin/Release/
    ├── WorkTimer.exe          # Exe файл с зависимостями
    ├── Qt6Core.dll           # Qt зависимости
    ├── Qt6Gui.dll
    ├── Qt6Widgets.dll
    ├── Qt6Multimedia.dll
    ├── platforms/            # Qt плагины
    ├── imageformats/
    ├── iconengines/
    ├── styles/
    ├── multimedia/
    ├── networkinformation/
    ├── generic/
    ├── tls/
    └── sounds/               # Звуковые файлы
```

## Порядок сборки

1. **Полная сборка:**
   ```bash
   build_cpp.bat
   ```

2. **Только деплой Qt (если exe уже собран):**
   ```bash
   deploy_qt.bat
   ```

3. **Сборка инсталлятора:**
   ```bash
   build_installer.bat
   ```

## Требования

- Visual Studio 2022
- Qt 6.9.1 (msvc2022_64 или mingw_64)
- CMake 3.20+
- Inno Setup 6 (для сборки инсталлятора)

## Примечания

- Все Qt зависимости теперь находятся в папке `Release`
- Папка `bin` содержит только основной exe файл
- Инсталлятор включает все необходимые зависимости
- Скрипты автоматически проверяют наличие необходимых файлов 
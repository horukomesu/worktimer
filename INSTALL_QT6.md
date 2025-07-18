# Установка Qt6 для сборки WorkTimer

## 🚀 Быстрая установка Qt6

### Вариант 1: Qt Online Installer (Рекомендуется)

1. **Скачайте Qt Online Installer**:
   - Перейдите на [qt.io/download](https://www.qt.io/download)
   - Скачайте "Qt Online Installer for Windows"

2. **Установите Qt6**:
   - Запустите установщик
   - Создайте аккаунт Qt (бесплатно)
   - Выберите компоненты:
     - **Qt 6.6.x** (последняя версия)
     - **MSVC 2019 64-bit** или **MSVC 2022 64-bit**
     - **Qt Creator** (опционально)
     - **CMake** (если не установлен)

3. **Настройте переменные окружения**:
   ```cmd
   set CMAKE_PREFIX_PATH=C:\Qt\6.6.x\msvc2019_64
   ```

### Вариант 2: vcpkg (Альтернативный)

```cmd
# Установите vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Установите Qt6
.\vcpkg install qt6-base qt6-widgets qt6-multimedia

# Интегрируйте с Visual Studio
.\vcpkg integrate install
```

### Вариант 3: Conan (Для опытных разработчиков)

```cmd
# Установите Conan
pip install conan

# Создайте conanfile.txt
echo [requires] > conanfile.txt
echo qt/6.6.0 >> conanfile.txt
echo [generators] >> conanfile.txt
echo CMakeDeps >> conanfile.txt
echo CMakeToolchain >> conanfile.txt

# Установите зависимости
conan install . --build=missing
```

## 🔧 Настройка CMake

После установки Qt6 обновите `CMakeLists.txt`:

```cmake
# Добавьте в начало CMakeLists.txt
if(DEFINED ENV{QTDIR})
    set(CMAKE_PREFIX_PATH $ENV{QTDIR})
endif()

# Или укажите путь напрямую
# set(CMAKE_PREFIX_PATH "C:/Qt/6.6.0/msvc2019_64")
```

## 🛠️ Альтернативная сборка без Qt6

Если Qt6 недоступен, можно использовать:

### Вариант 1: Сборка с Qt5
```cmake
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia)
target_link_libraries(WorkTimer PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Multimedia
)
```

### Вариант 2: Сборка с PySide6 (Python версия)
```cmd
# Вернитесь к Python версии
pyinstaller --onefile --windowed --name "WorkTimer" --strip --noconfirm pomodorotimer.py
```

## 📋 Проверка установки

```cmd
# Проверьте, что Qt6 найден
cmake --find-package -DNAME=Qt6 -DCOMPILER_ID=MSVC -DLANGUAGE=CXX -DMODE=EXIST

# Или проверьте переменную окружения
echo %CMAKE_PREFIX_PATH%
```

## 🚨 Решение проблем

### Ошибка "Could not find Qt6"
1. Убедитесь, что Qt6 установлен
2. Проверьте `CMAKE_PREFIX_PATH`
3. Перезапустите командную строку

### Ошибка компиляции
1. Убедитесь, что установлен MSVC 2019/2022
2. Проверьте версию CMake (>= 3.16)
3. Убедитесь, что архитектура x64

### Ошибка линковки
1. Проверьте, что все Qt6 компоненты установлены
2. Убедитесь, что пути к библиотекам корректны

## 📞 Поддержка

Если возникли проблемы:
1. Проверьте [документацию Qt](https://doc.qt.io/)
2. Обратитесь к [форуму Qt](https://forum.qt.io/)
3. Создайте issue в репозитории проекта 
# WorkTimer

A modern Pomodoro timer application built with Qt6 and C++. Features a clean, customizable interface with dark/light themes and customizable notification sounds.

## Features

- ⏱️ **Pomodoro Technique**: Work sessions, short breaks, and long breaks
- 🎵 **Custom Sounds**: Separate sounds for break and work notifications
- 🎨 **Themes**: Dark and light themes with modern UI
- ⚙️ **Flexible Settings**: Customizable session durations (min:sec format)
- 🖥️ **System Tray**: Minimize to system tray with notifications
- 📌 **Always on Top**: Option to keep window above other applications

## Technical Details

### Built With
- **Qt6**: Modern C++ framework for cross-platform GUI
- **C++17**: Modern C++ features
- **CMake**: Build system
- **QMediaPlayer**: Audio playback for notifications
- **QSystemTrayIcon**: System tray integration

### Architecture
- **Header-only design**: Clean separation of concerns
- **Resource management**: Qt resource system for assets
- **Settings persistence**: JSON-based configuration
- **Event-driven**: Qt signal/slot mechanism

### File Structure
```
worktimer/
├── src/           # Source files
├── include/       # Header files
├── ui/           # Qt Designer UI files
├── styles/       # QSS stylesheets
├── sounds/       # Notification audio files
├── resources/    # Icons and assets
└── installer/    # Installation files
```

## Usage

### Basic Operation
1. **Start Timer**: Click the play button to begin a work session
2. **Pause/Resume**: Use pause button to temporarily stop the timer
3. **Reset**: Reset to the beginning of current session
4. **Restart**: Reset and immediately start a new session

### Settings Panel
- **Work Duration**: Set work session length (e.g., 25:00, 30:30)
- **Short Break**: Set short break duration (e.g., 5:00, 3:30)
- **Long Break**: Set long break duration (e.g., 15:00, 20:00)
- **Sessions**: Number of work sessions before long break
- **Break Sound**: Sound for break notifications
- **Work Sound**: Sound for work session notifications
- **Volume**: Adjust notification volume
- **Always on Top**: Keep window above other applications

### Customization
- **Themes**: Toggle between dark and light themes
- **Sounds**: Add custom MP3 files to `sounds/` folder
- **Settings**: All preferences saved automatically


# -*- mode: python ; coding: utf-8 -*-


a = Analysis(
    ['pomodorotimer.py'],
    pathex=[],
    binaries=[],
    datas=[],
    hiddenimports=[],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=['tkinter', 'test', 'PyQt5', 'matplotlib', 'numpy', 'PIL', 'pandas', 'scipy', 'IPython', 'jupyter', 'notebook', 'qtpy', 'PyQt6', 'PySide2', 'PySide6.QtWebEngineWidgets', 'PySide6.QtWebEngineCore', 'PySide6.QtWebEngine', 'PySide6.Qt3DCore', 'PySide6.Qt3DRender', 'PySide6.Qt3DInput', 'PySide6.Qt3DLogic', 'PySide6.Qt3DAnimation', 'PySide6.Qt3DExtras', 'PySide6.QtCharts', 'PySide6.QtDataVisualization', 'PySide6.QtLocation', 'PySide6.QtPositioning', 'PySide6.QtSensors', 'PySide6.QtSerialPort', 'PySide6.QtSerialBus', 'PySide6.QtBluetooth', 'PySide6.QtNfc', 'PySide6.QtWebSockets', 'PySide6.QtHttpServer', 'PySide6.QtDesigner', 'PySide6.QtHelp', 'PySide6.QtUiTools', 'PySide6.QtXml', 'PySide6.QtSql', 'PySide6.QtTest', 'PySide6.QtConcurrent', 'PySide6.QtOpenGL', 'PySide6.QtOpenGLWidgets', 'PySide6.QtQuick', 'PySide6.QtQuickWidgets', 'PySide6.QtQml', 'PySide6.QtQmlModels', 'PySide6.QtQmlCore', 'PySide6.QtQmlWorkerScript', 'PySide6.QtQmlMeta', 'PySide6.QtSvg', 'PySide6.QtSvgWidgets', 'PySide6.QtPdf', 'PySide6.QtPdfWidgets', 'PySide6.QtVirtualKeyboard', 'PySide6.QtMultimediaWidgets'],
    noarchive=False,
    optimize=0,
)
pyz = PYZ(a.pure)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name='WorkTimer',
    debug=False,
    bootloader_ignore_signals=False,
    strip=True,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
)

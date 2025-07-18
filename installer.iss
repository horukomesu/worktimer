[Setup]
AppName=WorkTimer
AppVersion=1.0
AppPublisher=WorkTimer Team
AppPublisherURL=https://github.com/your-repo
AppSupportURL=https://github.com/your-repo/issues
AppUpdatesURL=https://github.com/your-repo/releases
DefaultDirName={autopf}\WorkTimer
DefaultGroupName=WorkTimer
AllowNoIcons=yes
LicenseFile=
InfoBeforeFile=
InfoAfterFile=
OutputDir=installer
OutputBaseFilename=WorkTimerSetup
SetupIconFile=
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=lowest
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "build\bin\Release\WorkTimer.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\bin\Release\Qt6Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\bin\Release\Qt6Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\bin\Release\Qt6Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\bin\Release\Qt6Multimedia.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\bin\Release\Qt6Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\bin\Release\avcodec-61.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\bin\Release\avformat-61.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\bin\Release\avutil-59.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\bin\Release\swresample-5.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\bin\Release\swscale-8.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\bin\Release\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\bin\Release\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\bin\Release\multimedia\*"; DestDir: "{app}\multimedia"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "build\bin\Release\sounds\*"; DestDir: "{app}\sounds"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\WorkTimer"; Filename: "{app}\WorkTimer.exe"
Name: "{group}\{cm:UninstallProgram,WorkTimer}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\WorkTimer"; Filename: "{app}\WorkTimer.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\WorkTimer.exe"; Description: "{cm:LaunchProgram,WorkTimer}"; Flags: nowait postinstall skipifsilent

[Code]
function InitializeSetup(): Boolean;
begin
  Result := True;
end; 
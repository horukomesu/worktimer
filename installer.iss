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
Source: "dist\WorkTimer.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "sounds\*"; DestDir: "{app}\sounds"; Flags: ignoreversion recursesubdirs createallsubdirs

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
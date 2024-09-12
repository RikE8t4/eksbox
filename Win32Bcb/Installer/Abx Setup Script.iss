; Inno Setup

[Setup]
; Update
AppID=EkszBox-ABX
UsePreviousAppDir=true
UpdateUninstallLogAppName=false

VersionInfoCopyright=Copyright © 2006-2008 Kernel Master
AppName=EkszBox-ABX
AppVerName=EkszBox-ABX v2.1
AppPublisher=EkszBox
AppVersion=2.1
VersionInfoVersion=2.1
DefaultDirName={pf}\EkszBox-XBX
DefaultGroupName=EkszBox-ABX
AllowNoIcons=false
OutputBaseFilename=EkszBox-ABX Setup v2.1
Compression=lzma
SolidCompression=true
SetupIconFile=EkszInstall.ico
WizardImageFile=InstallerBig.bmp
WizardSmallImageFile=InstallerSmall.bmp
;UninstallIconFile=yes
WindowVisible=false
WindowStartMaximized=true
SetupLogging=false
UsePreviousSetupType=false

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Components]
Name: xbadpcm; Description: Xbox ADPCM Codec; Types: full; MinVersion: 0, 1

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: Create a &Quick Launch icon; GroupDescription: Additional icons:; Flags: unchecked

;[Setup]
;InfoAfterFile=XBADPCM\XBADPCM Readme.txt

[Files]
Source: ..\Release_Build\EkszBox-ABX.exe; DestDir: {app}; Flags: ignoreversion
Source: ..\..\Docs\*.chm; DestDir: {app}\\Docs; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Debug_Build\Graphics\*; DestDir: {app}\Graphics; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\Debug_Build\Names\Names.xct; DestDir: {app}\Names; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\..\Read.txt; DestDir: {app}; Flags: ignoreversion

; Codec
Source: XBADPCM\XBADPCM Readme.txt; DestDir: {app}; Components: xbadpcm; Flags: ignoreversion; OnlyBelowVersion: 0,6
Source: XBADPCM\XBADPCM.ACM; DestDir: {tmp}; Components: xbadpcm; Flags: ignoreversion
Source: XBADPCM\XBADPCMinst.inf; DestDir: {tmp}; Components: xbadpcm; Flags: ignoreversion
Source: XBADPCM\XBADUninstall.inf; DestDir: {app}; Components: xbadpcm; Flags: ignoreversion

[Icons]
Name: {group}\EkszBox-ABX; Filename: {app}\EkszBox-ABX.exe
Name: {group}\Read; Filename: {app}\Read.txt
Name: {group}\XBADPCM Readme.txt; Filename: {app}\XBADPCM Readme.txt; Components: xbadpcm; OnlyBelowVersion: 0,6
;Name: "{group}\Docs"; Filename: "{app}\Docs"
;Name: "{group}\Docs\Index.htm"; Filename: "{app}\Docs\Index.htm"
Name: {group}\Docs\KM's Guide To Cracking Game Formats.chm; Filename: {app}\Docs\KM's Guide To Cracking Game Formats.chm
Name: {group}\{cm:UninstallProgram,EkszBox-ABX}; Filename: {uninstallexe}
Name: {commondesktop}\EkszBox-ABX; Filename: {app}\EkszBox-ABX.exe; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\EkszBox-ABX; Filename: {app}\EkszBox-ABX.exe; Tasks: quicklaunchicon

[Run]
; OnlyBelowVersion: 0,6 = no display on vista
; Codec
Filename: {app}\XBADPCM Readme.txt; Description: View Xbox ADPCM Codec Readme; OnlyBelowVersion: 0,6; Flags: postinstall shellexec skipifsilent; Components: xbadpcm
Filename: {win}\rundll.exe; Parameters: setupx.dll, InstallHinfSection Install 132 {tmp}\XBADPCMinst.inf; MinVersion: 1,0; Components: xbadpcm; StatusMsg: Installing Xbox ADPCM codec, please wait...
Filename: {sys}\rundll32.exe; Parameters: setupapi, InstallHinfSection DefaultInstall 132 {tmp}\XBADPCMinst.inf; MinVersion: 0, 1; Components: xbadpcm; StatusMsg: Installing Xbox ADPCM codec, please wait...

Filename: {app}\Read.txt; Description: View EkszBox-ABX Readme; Flags: postinstall shellexec skipifsilent unchecked
Filename: {app}\EkszBox-ABX.exe; Description: {cm:LaunchProgram,EkszBox-ABX}; Flags: unchecked nowait postinstall skipifsilent

;[Dirs]
;Name: "{app}\EkszBox-ABX"; flags: uninsalwaysuninstall

[UninstallRun]
Filename: {win}\rundll.exe; Parameters: setupx.dll, InstallHinfSection Install 132 {app}\XBADUninstall.inf; MinVersion: 1,0; Components: xbadpcm
Filename: {sys}\rundll32.exe; Parameters: setupapi, InstallHinfSection DefaultInstall 132 {app}\XBADUninstall.inf; MinVersion: 0,1; Components: xbadpcm

[UninstallDelete]
Type: files; Name: {app}\EkszBoxSettings.ini
Type: dirifempty; Name: {app}\EkszBox-ABX
;[Registry]
;Root: HKCU; Subkey: "Software\EkszBox-ABX"; Flags: uninsdeletekeyifempty



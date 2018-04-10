!include "MyWriteEnvStr.nsh"
!include "MyAddToPath.nsh"

;--------------------------------
;Include Modern UI

!include "MUI.nsh"
    	
;--------------------------------
;General

!define COMPANY "Module"
!define PRODUCT "VSHELL"
!define NAME "VShell"
!define PRODUCTVERSION "1.0"
!define FILEVERSION "1.0.0.24"
!define ENV_VAR "VSHELL"

OutFile "..\distr\${PRODUCT}_${FILEVERSION}.exe"
!define MUI_ICON "VShell.ico"
!define MUI_UNICON "VShell.ico"


InstallDir "$PROGRAMFILES${PLATFORM}\${COMPANY}\${PRODUCT}"

	;Registry key to check for directory (so if you install again, it will 
	;overwrite the old one automatically)
InstallDirRegKey HKLM "Software\${COMPANY}\${PRODUCT}" "Install_Dir"

Name "${PRODUCT}"
BrandingText "${NAME}"

;--------------------------------
;Interface Settings

!define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings

	;Remember the installer language
!define MUI_LANGDLL_REGISTRY_ROOT "HKLM" 
!define MUI_LANGDLL_REGISTRY_KEY "Software\${COMPANY}\${PRODUCT}" 
!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE  $(myLicenseData)
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
Page custom OnCreateEnvironmentVariables
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
!insertmacro MUI_LANGUAGE "Russian"
!define LANG_NUM ${LANG_RUSSIAN}

LicenseLangString myLicenseData ${LANG_NUM} "license.txt"

LicenseData $(myLicenseData)

VIProductVersion ${FILEVERSION}
VIAddVersionKey "FileVersion" ${FILEVERSION}
VIAddVersionKey "FileDescription" "${PRODUCT} installer"
VIAddVersionKey "ProductVersion" ${PRODUCTVERSION}
VIAddVersionKey "ProductName" ${PRODUCT}
VIAddVersionKey "CompanyName" ${COMPANY}
VIAddVersionKey "LegalCopyright" "© RC Module"

;--------------------------------
;Reserve Files
  
	;These files should be inserted before other files in the data block
	;Keep these lines before any File command
	;Only for solid compression (by default, solid compression is enabled for BZIP2 and LZMA)
  
!insertmacro MUI_RESERVEFILE_LANGDLL
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

;--------------------------------
;Installer Sections

Section "VShell" SecVShell
	SectionIn RO
  
	SetOutPath "$INSTDIR"
	CreateDirectory "$INSTDIR\bin"
	CreateDirectory "$INSTDIR\doc"
	CreateDirectory "$INSTDIR\include"
	CreateDirectory "$INSTDIR\lib"
	CreateDirectory "$INSTDIR\make"
	CreateDirectory "$INSTDIR\make\VS_2005"
	CreateDirectory "$INSTDIR\make\VS_2005\AviPlayer"
	CreateDirectory "$INSTDIR\make\VS_2005\Palette"
	CreateDirectory "$INSTDIR\make\VS_2005\SmoothFilter"
	CreateDirectory "$INSTDIR\make\VS_2005\VectorGraph"
	CreateDirectory "$INSTDIR\src"
	CreateDirectory "$INSTDIR\src\Examples"

	SetOutPath "$INSTDIR\bin"
!if ${PLATFORM} == 32
	 File "..\make\VS_2005\Release\VShell.dll"
	;File "..\make\VS_2005\Release\AviPlayer.exe"
	;File "..\make\VS_2005\Release\Palette.exe"
	;File "..\make\VS_2005\Release\SmoothFilter.exe"
	;File "..\make\VS_2005\Release\VectorGraph.exe"
!else
	;File "..\make\VS_2005\Release\VShell64.dll"
	;File "..\make\VS_2005\Release\AviPlayer64.exe"
	;File "..\make\VS_2005\Release\Palette64.exe"
	;File "..\make\VS_2005\Release\SmoothFilter64.exe"
	;File "..\make\VS_2005\Release\VectorGraph64.exe"
!endif
	File "..\src\Examples\Lena.bmp"


	SetOutPath "$INSTDIR\doc"
	;File "..\doc\VShell.chm"
	;File "..\doc\VShell.pdf"

	SetOutPath "$INSTDIR\include"
	File "..\src\VShell\VShell.h"

	SetOutPath "$INSTDIR\lib"
!if ${PLATFORM} == 32
	File "..\make\VS_2005\Release\VShell.lib"
	File "..\make\VS_2005\Debug\VShelld.lib"
!else
	;File "..\make\VS_2005\Release\VShell64.lib"
!endif

;	SetOutPath "$INSTDIR\make\VS_2005"
;	File "/oname=Examples.props" "..\make\VS_2005\ExamplesDistr.props"
;	
;	SetOutPath "$INSTDIR\make\VS_2005\AviPlayer"
;	File "..\make\VS_2005\AviPlayer\AviPlayer.vcxproj.filters"
;	File "..\make\VS_2005\AviPlayer\AviPlayer.vcxproj"
;
;	SetOutPath "$INSTDIR\make\VS_2005\Palette"
;	File "..\make\VS_2005\Palette\Palette.vcxproj.filters"
;	File "..\make\VS_2005\Palette\Palette.vcxproj"
;
;	SetOutPath "$INSTDIR\make\VS_2005\SmoothFilter"
;	File "..\make\VS_2005\SmoothFilter\SmoothFilter.vcxproj.filters"
;	File "..\make\VS_2005\SmoothFilter\SmoothFilter.vcxproj"
;
;	SetOutPath "$INSTDIR\make\VS_2005\VectorGraph"
;	File "..\make\VS_2005\VectorGraph\VectorGraph.vcxproj.filters"
;	File "..\make\VS_2005\VectorGraph\VectorGraph.vcxproj"
;
;	SetOutPath "$INSTDIR\src\Examples"
;	File "..\src\Examples\AviPlayer.cpp"
;	File "..\src\Examples\Palette.cpp"
;	File "..\src\Examples\SmoothFilter.cpp"
;	File "..\src\Examples\VectorGraph.cpp"

	WriteRegStr HKLM SOFTWARE\${COMPANY}\${PRODUCT} "Install_Dir" "$INSTDIR"
  
		;Write the uninstall keys for Windows
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayName" "${PRODUCT}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayIcon" "$INSTDIR\uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "UninstallString" '"$INSTDIR\uninstall.exe"'
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "NoRepair" 1
	WriteUninstaller "uninstall.exe"

	SetShellVarContext all
	CreateDirectory "$SMPROGRAMS\${COMPANY}\${PRODUCT}"
	CreateShortCut "$SMPROGRAMS\${COMPANY}\${PRODUCT}\${PRODUCT}.lnk" "$INSTDIR\" "" "$INSTDIR\"
	CreateShortCut "$SMPROGRAMS\${COMPANY}\${PRODUCT}\Руководство.lnk" "$INSTDIR\doc\VShell.chm"
	CreateShortCut "$SMPROGRAMS\${COMPANY}\${PRODUCT}\Удалить.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe"

SectionEnd

;--------------------------------
;Envir Section

Section "Environment variable" SecEnvir

!insertmacro MUI_INSTALLOPTIONS_READ $R7 "EnvironmentVariables.ini" "Field 3" "State"
	IntCmp $R7 0 lab_current
        StrCpy $R6 "all"
        SetShellVarContext all
        goto lab_skip_current
lab_current:
        StrCpy $R6 "current"
        SetShellVarContext current
lab_skip_current:
	!insertmacro MUI_INSTALLOPTIONS_READ $R7 "EnvironmentVariables.ini" "Field 1" "State"
	IntCmp $R7 0 lab_no_env_var
	Push $R6
	Push ${ENV_VAR}
	Push $INSTDIR
	Call MyWriteEnvStr
lab_no_env_var:
	!insertmacro MUI_INSTALLOPTIONS_READ $R7 "EnvironmentVariables.ini" "Field 2" "State"
	IntCmp $R7 0 lab_no_path
	Push $R6
	Push "$INSTDIR\bin"
	Call MyAddToPath
lab_no_path:

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"
  
		;Remove registry keys
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}"
	DeleteRegKey HKLM SOFTWARE\${COMPANY}\${PRODUCT}
	DeleteRegKey /ifempty HKLM SOFTWARE\${COMPANY}

		;Remove files and directories
!if ${PLATFORM} == 32
	Delete "$INSTDIR\bin\VShell.dll"
;	Delete "$INSTDIR\bin\AviPlayer.exe"
;	Delete "$INSTDIR\bin\Palette.exe"
;	Delete "$INSTDIR\bin\SmoothFilter.exe"
;	Delete "$INSTDIR\bin\VectorGraph.exe"
!else
;	Delete "$INSTDIR\bin\VShell64.dll"
;	Delete "$INSTDIR\bin\AviPlayer64.exe"
;	Delete "$INSTDIR\bin\Palette64.exe"
;	Delete "$INSTDIR\bin\SmoothFilter64.exe"
;	Delete "$INSTDIR\bin\VectorGraph64.exe"
!endif
	Delete "$INSTDIR\bin\Lena.bmp"
        RMDir "$INSTDIR\bin"

	Delete "$INSTDIR\doc\VShell.chm"
	Delete "$INSTDIR\doc\VShell.pdf"
	RMDir "$INSTDIR\doc"

	Delete "$INSTDIR\include\VShell.h"
	RMDir "$INSTDIR\include"

!if ${PLATFORM} == 32
	Delete "$INSTDIR\lib\VShell.lib"
!else
	Delete "$INSTDIR\lib\VShell64.lib"
!endif
	RMDir "$INSTDIR\lib"

	Delete "$INSTDIR\make\VS_2005\AviPlayer\AviPlayer.vcxproj.filters"
	Delete "$INSTDIR\make\VS_2005\AviPlayer\AviPlayer.vcxproj"
	RMDir "$INSTDIR\make\VS_2005\AviPlayer"

	Delete "$INSTDIR\make\VS_2005\Palette\Palette.vcxproj.filters"
	Delete "$INSTDIR\make\VS_2005\Palette\Palette.vcxproj"
	RMDir "$INSTDIR\make\VS_2005\Palette"

	Delete "$INSTDIR\make\VS_2005\SmoothFilter\SmoothFilter.vcxproj.filters"
	Delete "$INSTDIR\make\VS_2005\SmoothFilter\SmoothFilter.vcxproj"
	RMDir "$INSTDIR\make\VS_2005\SmoothFilter"

	Delete "$INSTDIR\make\VS_2005\VectorGraph\VectorGraph.vcxproj.filters"
	Delete "$INSTDIR\make\VS_2005\VectorGraph\VectorGraph.vcxproj"
	RMDir "$INSTDIR\make\VS_2005\VectorGraph"

	Delete "$INSTDIR\make\VS_2005\Examples.props"
	RMDir "$INSTDIR\make\VS_2005"
	RMDir "$INSTDIR\make"

	Delete "$INSTDIR\src\Examples\AviPlayer.cpp"
	Delete "$INSTDIR\src\Examples\Palette.cpp"
	Delete "$INSTDIR\src\Examples\SmoothFilter.cpp"
	Delete "$INSTDIR\src\Examples\VectorGraph.cpp"
	RMDir "$INSTDIR\src\Examples"
	RMDir "$INSTDIR\src"
	
	Delete $INSTDIR\uninstall.exe
	RMDir "$INSTDIR"

	RMDir "$PROGRAMFILES\${COMPANY}"

	SetShellVarContext all
	RMDir /r "$SMPROGRAMS\${COMPANY}\${PRODUCT}"	
        RMDir "$SMPROGRAMS\${COMPANY}"

		; Remove the envir. variable
	Push ${ENV_VAR}
  	Call un.MyDeleteEnvStr
	Push "PATH"
	Push "$INSTDIR\bin"
  	Call un.MyRemoveFromPath

SectionEnd

;--------------------------------
;Descriptions

LangString DESC_SecVShell ${LANG_RUSSIAN} "Установка программного модуля VShell и его компонентов."
LangString DESC_SecEnvir ${LANG_RUSSIAN} "Создание переменной среды окружения ${ENV_VAR} и добаление пути к модулям VShell в переменную Path."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SecVShell} $(DESC_SecVShell)
!insertmacro MUI_DESCRIPTION_TEXT ${SecEnvir} $(DESC_SecEnvir)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function .onInit
FunctionEnd

Function OnCreateEnvironmentVariables
        ${If} ${SectionIsSelected} ${SecEnvir}
                ReserveFile "EnvironmentVariables.ini"
                !insertmacro MUI_INSTALLOPTIONS_EXTRACT "EnvironmentVariables.ini"
                !insertmacro MUI_HEADER_TEXT "Environment Variables" ""
                !insertmacro MUI_INSTALLOPTIONS_DISPLAY "EnvironmentVariables.ini"
        ${EndIf}
FunctionEnd


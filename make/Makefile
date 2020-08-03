SHELL = cmd
PROJECT=vshell



default: vs2015
	
	
vs2005: 
	premake5 vs2005
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).sln 

#"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).vcproj "Debug"

vs2015:	
	premake5 vs2015
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release
	
#	"$(VS140COMNTOOLS)vsvars32" && msbuild.exe $(PROJECT).sln /t:Build
#/p:Configuration="Release;Platform=Win32
#	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release
	
$(PROJECT).sln:	premake5.lua 
	premake5 $(MAKECMDGOALS) 

	
install: setenv setpath
	
setenv: 
	setenv -ua VSHELL $(realpath ..)
	
setpath:	
	setenv -ua PATH ;%VSHELL%\bin
#	setx  VSHELL $(realpath ..)
	
premake:
	wget https://github.com/premake/premake-core/releases/download/v5.0.0-alpha12/premake-5.0.0-alpha12-windows.zip -Opremake-5.0.0-alpha12-windows.zip
	
CLEAN_DIRS = ../bin ../lib

include ../clean.mk	
	
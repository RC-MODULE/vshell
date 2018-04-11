SHELL = cmd
PROJECT=vshell



default:
	$(MAKE) vs2005
	
	
vs2005: $(PROJECT).sln
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).sln 

#"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).vcproj "Debug"

vs2015:	$(PROJECT).sln
	"$(VS140COMNTOOLS)vsvars32" && msbuild.exe $(PROJECT).sln /t:Build
	
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
	

	
CLEAN_DIRS = ../bin ../lib

include ../clean.mk	
	
SHELL = cmd
PROJECT=vshell-examples

VS   ?=vs2015

.PHONY  = vs2005 vs2015

all:
	$(MAKE) vs2015
	
vs2005: 
	premake5 vs2005
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).sln 

#"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).vcproj "Debug"

vs2015:	
	premake5 vs2015 
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).sln /t:Build
	 
#/p:Configuration="Release;Platform=Win32
#	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release
	
#$(PROJECT).sln:	premake5.lua 
#	premake5 $(MAKECMDGOALS) 

install: 
	setenv -a VSHELL $(realpath ..)
	
#	setx  VSHELL $(realpath ..)
	

	


include ../clean.mk	
	
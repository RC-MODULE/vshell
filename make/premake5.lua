#!lua
ROOT = "../.."
-- A solution contains projects, and defines the available configurations
solution "vshell"
   	
	configurations { "Release","Debug" }
	platforms { "Win32","x64"}
	systemversion ="latest"

	project "vshell"
      kind "SharedLib"
      files { 
		"../include/*.h",
	  	"../src/*.cpp",
		"../src/*.rc",
		"../src/*.def"
	}
	characterset ("MBCS") --  Multi-byte Character Set; currently Visual Studio only
	includedirs { "../include","../src"}
	links { "Vfw32.lib","comctl32.lib" }
	targetdir ("../bin")
	implibdir ("../lib")  
	  
	 
	configuration {"Debug","Win32"}
		targetsuffix ("-x86")
		architecture "x86"
		defines { "DEBUG"}
		symbols  ="On" 
		objdir "1"
	
	configuration {"Release","Win32"}
		targetsuffix ("-x86")
		architecture "x86"
		defines { "NDEBUG"}
		symbols  ="Off" 
		objdir "2"
	
	configuration {"Debug","x64"}
		targetsuffix ("-x64")
		architecture "x86_64"
		defines { "DEBUG"}
		symbols  ="On" 
		objdir "3"
	
	configuration {"Release","x64"}
		targetsuffix ("-x64")
		architecture "x86_64"
		defines { "NDEBUG"}
		symbols  ="Off" 
		objdir "4"
	 
	 
 

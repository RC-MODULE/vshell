#!lua
ROOT = "../.."
-- A solution contains projects, and defines the available configurations
solution "vshell"
   	
	configurations { "Debug", "Release" }
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
	  
	configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 
		 

    configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		
	 
 

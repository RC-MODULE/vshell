#!lua
ROOT = ".."
-- A solution contains projects, and defines the available configurations
solution "vshell-examples"
	configurations { "Debug", "Release" }
	platforms { "Win32","x64"}
	----------------------------------------------
	
	project "AviPlayer"
      kind "ConsoleApp"
      files {"AviPlayer.cpp"}
	characterset ("MBCS") --  Multi-byte Character Set; currently Visual Studio only
	includedirs { "../include"}
	libdirs { "../lib"} 
	links { "vshell.lib" }
	debugenvs { "PATH=%PATH%;$(ProjectDir)/../bin" }
	configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
	configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
	----------------------------------------------
	--configurations { "Debug", "Release" }
	project "Palette"
      kind "ConsoleApp"
      files {"Palette.cpp"}
	characterset ("MBCS") --  Multi-byte Character Set; currently Visual Studio only
	includedirs { "../include"}
	libdirs { "../lib"} 
	links { "vshell.lib" }
	debugenvs { "PATH=%PATH%;$(ProjectDir)/../bin" }
	configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
	configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
	----------------------------------------------
	--configurations { "Debug", "Release" }
	project "SmoothFilter"
      kind "ConsoleApp"
      files {"SmoothFilter.cpp"}
	characterset ("MBCS") --  Multi-byte Character Set; currently Visual Studio only
	includedirs { "../include"}
	libdirs { "../lib"} 
	links { "vshell.lib" }
	debugenvs { "PATH=%PATH%;$(ProjectDir)/../bin" }
	configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
	configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
	----------------------------------------------
	--configurations { "Debug", "Release" }
	project "VectorGraph"
    kind "ConsoleApp"
    files {"VectorGraph.cpp"}
	characterset ("MBCS") --  Multi-byte Character Set; currently Visual Studio only
	includedirs { "../include"}
	libdirs { "../lib"} 
	links { "vshell.lib" }
	debugenvs { "PATH=%PATH%;$(ProjectDir)/../bin" }
	configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
	configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
	
	--configurations { "Debug", "Release" }
	project "SignalPlot"
    kind "ConsoleApp"
    files {"SignalPlot.cpp"}
	characterset ("MBCS") --  Multi-byte Character Set; currently Visual Studio only
	includedirs { "../include","$(SIGNALPLOT64)/include"}
	libdirs { "../lib","$(SIGNALPLOT64)/lib"} 
	links { "vshell.lib","signalplot64.lib" }
	debugenvs { "PATH=%PATH%;$(ProjectDir)/../bin" }
	configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
	configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		
	 
 	
	 
 

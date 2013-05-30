-- Change this table with the paths of your Garry's Mod module base include folders (for different platforms)
local GARRYSMOD_INCLUDES_PATH = {
windows = "D:/garrysmod stuff/gmod-module-base/include",
linux = "/home/daniel/gmod-module-base/include",
macosx = ""
}

-- Change this table with the paths of your SourceSDK folders (for different platforms)
local SOURCE_SDK_PATH = {
windows = "D:/garrysmod stuff/SourceSDK",
linux = "/home/daniel/SourceSDK",
macosx = ""
}

-- Do not change anything else below this line.

GARRYSMOD_INCLUDES_PATH = GARRYSMOD_INCLUDES_PATH[os.get()]
SOURCE_SDK_PATH = SOURCE_SDK_PATH[os.get()]

if not GARRYSMOD_INCLUDES_PATH or not SOURCE_SDK_PATH then
	error("GARRYSMOD_INCLUDES_PATH or SOURCE_SDK_PATH not found! (could also be unsupported platform)")
end

local translation = {windows = "Windows", linux = "Linux", macosx = "MacOSX"}
local upperos = translation[os.get()]

if not _ACTION then
	error("_ACTION variable is missing! (usually because you didn't pass a compiler name to use)")
elseif not upperos then
	error("OS is unsupported!")
end

solution("gm_matrix")
	language("C++")
	location("Projects/" .. upperos .. "/" .. _ACTION)
	flags({"NoPCH", "StaticRuntime", "EnableSSE", "Symbols"})

	if os.is("macosx") then
		platforms({"Universal32"})
	elseif os.is("linux") then
		defines({"_LINUX"}) -- lolhackedsourcesdk
		platforms({"x32"})
	elseif os.is("windows") then
		defines({"WIN32", "_WIN32_WINNT=0x0501"})
		platforms({"x32"})
	end

	configurations({"Release"})

	configuration("Release")
		defines({"NDEBUG"})
		flags({"Optimize"})
		targetdir("Projects/" .. upperos .. "/Release")
		objdir("Projects/" .. upperos .. "/Intermediate")

	project("gmsv_matrix")
		kind("SharedLib")
		defines({"MATRIX_SERVER", "GAME_DLL", "GMMODULE"})
		includedirs({SOURCE_SDK_PATH .. "/public", SOURCE_SDK_PATH .. "/public/tier0", SOURCE_SDK_PATH .. "/public/tier1", GARRYSMOD_INCLUDES_PATH})
		files({"*.c", "*.cxx", "*.cpp", "*.h", "*.hxx", "*.hpp"})
		vpaths({["Header files"] = {"**.h", "**.hxx", "**.hpp"}, ["Source files"] = {"**.c", "**.cxx", "**.cpp"}})
		targetprefix("gmsv_") -- Just to remove prefixes like lib from Linux
		targetname("matrix")
		if os.is("windows") then
			libdirs({SOURCE_SDK_PATH .. "/lib/public"})
			links({"tier0", "tier1", "mathlib"})
			targetsuffix("_win32")
		elseif os.is("linux") then
			libdirs({"Projects/" .. upperos .. "/" .. _ACTION})
			linkoptions({"-l:libtier0_srv.so", "-l:tier1_i486.a", "-l:mathlib_i486.a"})
			targetsuffix("_linux")
			targetextension(".dll") -- Derp Garry, WHY
		elseif os.is("macosx") then
			libdirs({"Projects/" .. upperos .. "/" .. _ACTION})
			linkoptions({"-l:libtier0.dylib", "-l:tier1_i486.a", "-l:mathlib_i486.a"})
			targetsuffix("_mac")
			targetextension(".dll") -- Derp Garry, WHY
		end

	project("gmcl_matrix")
		kind("SharedLib")
		defines({"MATRIX_CLIENT", "GAME_DLL", "GMMODULE"})
		includedirs({SOURCE_SDK_PATH .. "/public", SOURCE_SDK_PATH .. "/public/tier0", SOURCE_SDK_PATH .. "/public/tier1", GARRYSMOD_INCLUDES_PATH})
		files({"*.c", "*.cxx", "*.cpp", "*.h", "*.hxx", "*.hpp"})
		vpaths({["Header files"] = {"**.h", "**.hxx", "**.hpp"}, ["Source files"] = {"**.c", "**.cxx", "**.cpp"}})
		targetprefix("gmcl_") -- Just to remove prefixes like lib from Linux
		targetname("matrix")
		if os.is("windows") then
			libdirs({SOURCE_SDK_PATH .. "/lib/public"})
			links({"tier0", "tier1", "mathlib"})
			targetsuffix("_win32")
		elseif os.is("linux") then
			libdirs({"Projects/" .. upperos .. "/" .. _ACTION})
			linkoptions({"-l:libtier0_srv.so", "-l:tier1_i486.a", "-l:mathlib_i486.a"})
			targetsuffix("_linux")
			targetextension(".dll") -- Derp Garry, WHY
		elseif os.is("macosx") then
			libdirs({"Projects/" .. upperos .. "/" .. _ACTION})
			linkoptions({"-l:libtier0.dylib", "-l:tier1_i486.a", "-l:mathlib_i486.a"})
			targetsuffix("_mac")
			targetextension(".dll") -- Derp Garry, WHY
		end
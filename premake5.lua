local workspaceName   = "onvd"
local projectName     = "onvd"
local parentDir       = "build/"

workspace (workspaceName)
  configurations { "Debug", "Release" }
  location (parentDir)

  filter "system:windows"
    architecture "x86"

  filter "system:linux"
    architecture "x64"

  project (projectName)
    cppdialect "c++20"
    files { "src/**.cpp" }
    kind "ConsoleApp"
    language "c++"
    location (parentDir .. projectName)

    filter "system:windows"
      defines { "WIN32_LEAN_AND_MEAN", "NOMINMAX" }

    filter "system:linux"
      linkoptions { "-ldl", "-Wl,-rpath='$$ORIGIN/bin/linux64'", "-Wl,--disable-new-dtags" }

    filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "on"
      targetsuffix "-D"

    filter "configurations:Release"
      defines { "NDEBUG" }
      flags { "LinkTimeOptimization", "NoIncrementalLink" }
      functionlevellinking "on"
      intrinsics "on"
      optimize "speed"
      staticruntime "on"
      symbols "off"

if _ACTION == "rm" then
  os.rmdir(parentDir)
end
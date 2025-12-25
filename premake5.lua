workspace "sim-mode-ews"
    configurations { "Release" }

project "sim-mode-epr"
    kind "consoleApp"
    language "c++"
    cppdialect "c++11"

    files { "./main.cpp" }

    includedirs 
    { 
        "./",
        "./src",
        "./includes",
    }
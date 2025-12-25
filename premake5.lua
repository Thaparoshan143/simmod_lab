workspace "sim-mod-ws"
    configurations { "Release" }

project "sim-mod-pr"
    kind "consoleApp"
    toolset "clang"
    language "c++"
    cppdialect "c++17" -- other cpp version gives error due to latest packages dependencies

    files { "./main.cpp" }

    includedirs 
    { 
        "./",
        "./src",
        "./includes",
        "/opt/homebrew/include"
    }
    configurations { "macosx" }
        libdirs 
        { 
            "/opt/homebrew/lib", 
            "/opt/local/lib", 
            "/usr/local/lib" 
        }

        links 
        { 
            "matplot" -- seems already other stuffs are included in it.. like glfw.., opengl..
        }

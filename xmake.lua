set_xmakever("2.7.2")
includes("packages/**.lua")
set_project("bgfxtest")

set_arch("x64")
set_languages("cxx20", "cxx2a")
set_runtimes(is_mode("debug") and "MTd" or "MT")
set_symbols("debug")

add_requires("bgfx_custom", "rmlui_custom", "eigen", "stb")

target("bgfxtest")
    set_default(true)
    set_kind("binary")
    set_prefixname("")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h", "include/**.h")
    add_includedirs("src/", "include/", { public = true })
    add_packages("bgfx_custom", "rmlui_custom", "eigen", "stb")
    --add_ldflags("/subsystem:windows")

add_rules("plugin.vsxmake.autoupdate")
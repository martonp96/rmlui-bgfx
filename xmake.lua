set_xmakever("2.7.2")
includes("packages/**.lua")
set_project("rmlui-bgfx")

set_languages("cxx20", "cxx2a")
set_runtimes(is_mode("debug") and "MTd" or "MT")
set_symbols("debug")

add_requires("bgfx_custom", "rmlui_custom", "eigen", "stb", "libsdl")
add_requires("libsdl", { configs = { wayland = true, x11 = false, with_x = false } })

local shaders = {
    { "vert.sc",           "vert.bin",          "common", "vertex",   "varying.def.sc" },
    { "textured_frag.sc",  "textured_frag.bin", "common", "fragment", "" },
}

local shadersPath = "data/shaders/"
local shaderType = is_host("windows") and "s_5_0" or "spirv"

target("rmlui-bgfx")
    set_arch(os.arch())
    set_plat(os.host())
    set_default(true)
    set_kind("binary")
    set_prefixname("")
    add_files("src/**.cpp", shadersPath .. "*.bin")
    add_headerfiles("src/**.h", "include/**.h")
    add_includedirs("src/", "include/", { public = true })
    add_packages("bgfx_custom", "rmlui_custom", "eigen", "stb", "libsdl")
    add_rules("utils.bin2c")
    on_config(function (target)
        local shadercPath = path.join(target:pkg("bgfx_custom"):installdir(), "bin/shadercRelease" .. (is_host("windows") and ".exe" or ""))
        for _, shader in pairs(shaders) do
            local cmd = shadercPath ..
                " -f " .. path.join(target:scriptdir(), shadersPath, shader[1]) ..
                " -o " .. path.join(target:scriptdir(), shadersPath, shader[2]) .. 
                " -i " .. path.join(target:scriptdir(), shadersPath, shader[3]) ..
                " -p " .. shaderType ..
                " --type " .. shader[4] ..
                " --platform " .. os.host() ..
                " --verbose"
    
            if shader[5] ~= nil and shader[5] ~= "" then
                cmd = cmd .. " --varyingdef " .. path.join(target:scriptdir(), shadersPath, shader[5])
            end
    
            os.exec(cmd)
    
            --cmd = shadercPath .. cmd;
    
            print(cmd)
        end
    end)
    after_build(function (target)
        os.cp(path.join(target:scriptdir(), "data/rml/*"), path.join(target:scriptdir(), target:targetdir()))

        --[[print(target:pkgs())
        for pkg, pkg_details in pairs(target:pkgs()) do
            print(pkg_details._INFO)
            for i, dir in ipairs(pkg_details._INFO.sysincludedirs) do
                --os.cp(dir, target:targetdir())
            end
            for i, dir in ipairs(pkg_details._INFO.libfiles) do
                print(dir)
                --os.cp(dir, path.join(target:targetdir(), dir:match("([^/\\]+)$")))
            end
        end]]
    end)

add_rules("plugin.vsxmake.autoupdate")
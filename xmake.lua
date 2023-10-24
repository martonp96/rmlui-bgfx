set_xmakever("2.7.2")
includes("packages/**.lua")
set_project("rmlui-bgfx")

set_languages("cxx20", "cxx2a")
set_runtimes(is_mode("debug") and "MTd" or "MT")
set_symbols("debug")

add_requires("bgfx_custom", "rmlui_custom", "eigen", "stb", "libsdl")
add_requires("libsdl", { configs = { wayland = true, x11 = false, with_x = false } })

local shaders = {
    { "vert.sc", "vert.bin", "common", "vertex",   "varying.def.sc" },
    { "frag.sc", "frag.bin", "common", "fragment", "" },
}

local shadersPath = "data/shaders/"
local shaderType = "s_5_0"
local shaderPlat = "windows"
if is_host("macosx") then 
    shaderType = "metal"
    shaderPlat = "osx"
elseif is_host("linux") then
    shaderType = "spirv"
    shaderPlat = "linux"
end

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
    after_load(function (target)
        local shadercPath = path.join(target:pkg("bgfx_custom"):installdir(), "bin/shadercRelease" .. (is_host("windows") and ".exe" or ""))
        for _, shader in pairs(shaders) do
            local cmd = shadercPath ..
                " -f " .. path.join(target:scriptdir(), shadersPath, shader[1]) ..
                " -o " .. path.join(target:scriptdir(), shadersPath, shader[2]) .. 
                " -i " .. path.join(target:scriptdir(), shadersPath, shader[3]) ..
                " -p " .. shaderType ..
                " --type " .. shader[4] ..
                " --platform " .. shaderPlat ..
                " --verbose"
    
            if shader[5] ~= nil and shader[5] ~= "" then
                cmd = cmd .. " --varyingdef " .. path.join(target:scriptdir(), shadersPath, shader[5])
            end
    
            os.exec(cmd)    
            print(cmd)
        end
    end)
    after_build(function (target)
        os.cp(path.join(target:scriptdir(), "data/rml/*"), path.join(target:scriptdir(), target:targetdir()))
    end)

add_rules("plugin.vsxmake.autoupdate")
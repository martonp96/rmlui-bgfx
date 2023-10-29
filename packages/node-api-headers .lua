package("node-api-headers")
    set_homepage("https://nodejs.org")
    set_license("MIT")

    add_urls("https://github.com/nodejs/node-api-headers/archive/refs/tags/v$(version).tar.gz")
    add_versions("1.1.0", "70608bc1e6dddce280285f3462f18a106f687c0720a4b90893e1ecd86e5a8bbf")

    on_install("macosx", "linux", "windows", function (package)
        os.trycp("include/*.h", package:installdir("include"))
        if is_plat("windows") then
            --https://github.com/xfangfang/wiliwili/blob/7541334784c96991d6d391a5acda5410a875f1af/xmake.lua#L161C22-L161C22
            import("detect.sdks.find_vstudio")
            local vs = find_vstudio()["2022"]["vcvarsall"]["x64"]
            local libExec = path.join(
                vs["VSInstallDir"],
                "VC",
                "Tools",
                "MSVC",
                vs["VCToolsVersion"],
                "bin",
                "HostX64",
                "x64",
                "lib.exe"
            )
            os.execv(libExec, {"/name:libnode.dll", "/def:def/node_api.def", "/out:libnode.lib", "/MACHINE:X64"})

            os.trycp("def/*.def", package:installdir("def"))
            os.trycp("libnode.lib", package:installdir("lib"))
        end
    end)
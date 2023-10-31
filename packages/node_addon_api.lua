package("node_addon_api")
    set_homepage("https://nodejs.org")
    set_license("MIT")

    add_urls("https://github.com/nodejs/node-addon-api/archive/refs/tags/v$(version).tar.gz")
    add_versions("7.0.0", "e0cf2ef9bed9965ac8f498b4a7007b9f0002fa022c1ff665f256ab54260d6305")

    on_install("macosx", "linux", "windows", function (package)
        os.trycp("**.h", package:installdir("include"), { rootdir="." })
    end)
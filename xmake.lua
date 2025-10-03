add_rules("mode.debug", "mode.release")

target("biocenozzz")
    set_kind("binary")
    add_files("src/*.cpp")
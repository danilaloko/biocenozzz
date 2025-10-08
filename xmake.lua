add_rules("mode.debug", "mode.release")
add_requires("toml++", "plog")

target("biocenozzz")
    set_kind("binary")
    add_files("src/*.cpp")
    set_languages("c++17")
    add_packages("toml++", "plog")
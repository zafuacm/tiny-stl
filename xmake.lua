add_rules("mode.debug", "mode.release")

target("src")
    set_kind("library")
    add_files("src/**.cpp")

target("test")
    set_kind("binary")
    add_files("test/*.cpp")
    add_deps("src")

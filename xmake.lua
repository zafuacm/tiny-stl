add_rules("mode.debug", "mode.release")

target("src")
    set_kind("library")
    add_headerfiles("src/**.hpp")

target("test")
    set_kind("binary")
    add_files("test/test.cpp")
    add_deps("src")
    add_cxxflags("-g", "-Wall", "-Wextra", "-Wshadow")

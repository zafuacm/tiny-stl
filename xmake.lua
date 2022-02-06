add_rules("mode.debug", "mode.release")

add_requires("gtest")

target("test")
    set_kind("binary")
    add_headerfiles("src/**.hpp")
    add_files("test/test.cpp")
    add_cxxflags("-g", "-Wall", "-Wextra", "-Wshadow", "-fsanitize=address")
    add_ldflags("-fsanitize=address")
    add_packages("gtest")

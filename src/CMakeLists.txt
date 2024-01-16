cmake_minimum_required(VERSION 3.20)

find_package(Boost REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(exo "main.cpp")
target_include_directories(exo PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(exo PRIVATE fmt::fmt-header-only)

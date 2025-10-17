cmake_minimum_required(VERSION 4.0)

set(CMAKE_C_STANDARD 17)

project(Phusikos)

add_library(Phusikos STATIC
    src/core.c
    src/particle.c
    src/pfgen.c
    src/pcontacts.c
    src/plinks.c
    src/pworld.c
)
target_link_libraries(Phusikos PRIVATE m)

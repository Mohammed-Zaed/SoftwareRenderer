cmake_minimum_required(VERSION 3.2.6)

project(software_renderer VERSION 0.0 LANGUAGES C)
set(CMAKE_C_STANDARD 99)

set(SRC 
    main.c
)

add_library(SDL2 SHARED IMPORTED)
set_target_properties(SDL2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /usr/local/include/SDL2)
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libSDL2.dylib)

add_executable(renderer ${SRC})
target_link_libraries(renderer
    PRIVATE SDL2
)


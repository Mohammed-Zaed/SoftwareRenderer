cmake_minimum_required(VERSION 3.2.6)

project(software_renderer VERSION 0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -Werror")


set(SRC 
	main.c
)

add_executable(renderer ${SRC})

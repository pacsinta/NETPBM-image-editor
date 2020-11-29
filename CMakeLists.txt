cmake_minimum_required(VERSION 3.17)
project(kepszerkesztes C)

set(CMAKE_C_STANDARD 99)

add_executable(kepszerkesztes main.c image.c imageEditing.c imageReading.c)
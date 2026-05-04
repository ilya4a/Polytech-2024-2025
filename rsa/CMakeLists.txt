cmake_minimum_required(VERSION 3.20)
project(rsa C)

set(CMAKE_C_STANDARD 11)

find_library(GMP_LIB gmp REQUIRED)

include_directories(include)

add_executable(rsa src/main.c src/rsa.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
target_link_libraries(rsa ${GMP_LIB})
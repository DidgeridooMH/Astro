cmake_minimum_required(VERSION 3.12)
project(Astro)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES src/main.cpp src/Tokenizer.cpp)

add_executable(Astro ${SOURCES})
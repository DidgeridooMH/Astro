cmake_minimum_required(VERSION 3.10)
project(Astro)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/main.cpp
    src/build_system/Tokenizer.cpp
    src/build_system/Program.cpp
    src/build_system/FileBlock.cpp
    src/build_system/FunctionBlock.cpp
    src/build_system/Variable.cpp
    src/utils/Logger.cpp
    )

add_executable(Astro ${SOURCES})
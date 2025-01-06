cmake_minimum_required(VERSION 3.15)
project(Dragonfly C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/lexer.cpp
    src/parser.cpp
    src/codegen.cpp
    src/semantics.cpp
    src/main.cpp
)

add_executable(dfc src/main.cpp ${SOURCES})

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
    DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

include(CTest)
enable_testing()

add_subdirectory(tests)

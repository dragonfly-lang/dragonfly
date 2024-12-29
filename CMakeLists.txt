cmake_minimum_required(VERSION 3.15)
project(MyLanguage C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/lexer.c
    src/token.c
    src/parser.c
    src/vm.c
    src/codegen.c
    src/semantics.c
)

add_executable(pebble src/main.c ${SOURCES})

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

include(CTest)
enable_testing()

add_subdirectory(tests)

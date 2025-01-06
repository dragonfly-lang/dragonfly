file(GLOB_RECURSE PARSER_TESTS "parser/*.cpp")

add_executable(run_tests lexer_test.cpp ${PARSER_TESTS} semantics_test.cpp)

target_link_libraries(run_tests gtest gtest_main pthread)
target_include_directories(run_tests PRIVATE ../include)

target_sources(run_tests PRIVATE
    ../src/lexer.cpp
    ../src/parser.cpp
    ../src/semantics.cpp
)

include(GoogleTest)
gtest_discover_tests(run_tests)

# Valgrind test integration
find_program(VALGRIND_EXECUTABLE valgrind)
if(VALGRIND_EXECUTABLE)
    add_test(NAME ValgrindTests
        COMMAND ${VALGRIND_EXECUTABLE} --leak-check=full --track-origins=yes --error-exitcode=1 --log-file=valgrind.log $<TARGET_FILE:run_tests>)
    set_tests_properties(ValgrindTests PROPERTIES TIMEOUT 30)
endif()

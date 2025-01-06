#include <gtest/gtest.h>

#include "dragon/parser.h"
#include "dragon/token.h"
#include "dragon/lexer.h"

struct WhileLoopTestParam {
    std::string code;   
    std::string to_string_result;
};

class WhileLoopTests : public ::testing::TestWithParam<WhileLoopTestParam> {};

TEST_P(WhileLoopTests, HandlesWhileLoops) {
    WhileLoopTestParam param = GetParam(); 

    Parser parser(Lexer(param.code).lex());
    auto nodes = parser.parse();

    EXPECT_EQ(nodes[0].to_string(), param.to_string_result) << "Expected " << param.to_string_result << ", got " << nodes[0].to_string();
}

INSTANTIATE_TEST_SUITE_P(
    WhileLoopVariations,
    WhileLoopTests,
    ::testing::Values(
        WhileLoopTestParam{"while true { print(5) }", "WhileLoopNode"},
        WhileLoopTestParam{"while true == true { print(5) }", "WhileLoopNode"},
        WhileLoopTestParam{"while true { print(5) }", "WhileLoopNode"},
        WhileLoopTestParam{"while true { print(5) }", "WhileLoopNode"}
    )
);
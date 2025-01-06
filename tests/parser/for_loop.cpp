#include <gtest/gtest.h>

#include "dragon/parser.h"
#include "dragon/token.h"
#include "dragon/lexer.h"

struct ForLoopTestParam {
    std::string code;   
    std::string to_string_result;
};

class ForLoopTests : public ::testing::TestWithParam<ForLoopTestParam> {};

TEST_P(ForLoopTests, HandlesForLoops) {
    ForLoopTestParam param = GetParam();

    Parser parser(Lexer(param.code).lex());
    auto nodes = parser.parse();

    EXPECT_EQ(nodes[0].to_string(), param.to_string_result) << "Expected " << param.to_string_result << ", got " << nodes[0].to_string();
}

INSTANTIATE_TEST_SUITE_P(
    ForLoopVariations,
    ForLoopTests,
    ::testing::Values(
        ForLoopTestParam{"for i in message { print(i) }", "ForLoopNode"},
        ForLoopTestParam{"for i in 0..10 { print(i) }", "ForLoopNode"},
        ForLoopTestParam{"for i in (10 + 20) {}", "ForLoopNode"}
    )
);
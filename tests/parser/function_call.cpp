#include <gtest/gtest.h>

#include "dragon/parser.h"
#include "dragon/token.h"
#include "dragon/lexer.h"

struct FunctionCallTestParam {
    std::string code;   
    std::string to_string_result;
};

class FunctionCallTests : public ::testing::TestWithParam<FunctionCallTestParam> {};

TEST_P(FunctionCallTests, HandlesFunctionCalls) {
    FunctionCallTestParam param = GetParam();

    Parser parser(Lexer(param.code).lex());
    auto nodes = parser.parse();

    EXPECT_EQ(nodes[0].to_string(), param.to_string_result) << "Expected " << param.to_string_result << ", got " << nodes[0].to_string();
}

INSTANTIATE_TEST_SUITE_P(
    FunctionCallVariations,
    FunctionCallTests,
    ::testing::Values(
        FunctionCallTestParam{"print()", "FunctionCallNode"},
        FunctionCallTestParam{"print(5)", "FunctionCallNode"},
        FunctionCallTestParam{"print(5 + 5)", "FunctionCallNode"},
        FunctionCallTestParam{"print(5, 10)", "FunctionCallNode"},
        FunctionCallTestParam{"print(5, 10, 15)", "FunctionCallNode"},
        FunctionCallTestParam{"print(5 + 2, 10 * 30, print(100), 20)", "FunctionCallNode"}
    )
);
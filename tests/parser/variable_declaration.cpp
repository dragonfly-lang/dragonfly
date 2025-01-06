#include <gtest/gtest.h>

#include "dragon/parser.h"
#include "dragon/token.h"
#include "dragon/lexer.h"

struct VariableTestParam {
    std::string code;   
    std::string to_string_result;
};

class VariableDeclarationTests : public ::testing::TestWithParam<VariableTestParam> {};

TEST_P(VariableDeclarationTests, HandlesVariableDeclarations) {
    VariableTestParam param = GetParam();

    Parser parser(Lexer(param.code).lex());
    auto nodes = parser.parse();

    EXPECT_EQ(nodes.size(), 1) << "Expected 1 node, got " << nodes.size() << " nodes";
    EXPECT_EQ(nodes[0].to_string(), param.to_string_result) << "Expected " << param.to_string_result << ", got " << nodes[0].to_string();
}

INSTANTIATE_TEST_SUITE_P(
    VariableDeclarationVariations,
    VariableDeclarationTests,
    ::testing::Values(
        VariableTestParam{"let x int = 5 + 5", "true"},
        VariableTestParam{"let x int", "true"},
        VariableTestParam{"let x = 5 + 5", "true"},
        VariableTestParam{"let mut x int = 5 + 5", "true"},
        VariableTestParam{"let mut x int", "true"},
        VariableTestParam{"let mut x = 5 + 5", "true"}
    )
);

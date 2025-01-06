#include <gtest/gtest.h>

#include "dragon/lexer.h"
#include "dragon/token.h"
#include "dragon/parser.h"

struct VariableTestParam {
    std::string code;   
    std::string to_string_result;
};

class VariableAssignmentTests : public ::testing::TestWithParam<VariableTestParam> {};

TEST_P(VariableAssignmentTests, HandlesVariableAssignments) {
    VariableTestParam param = GetParam();

    Parser parser(Lexer(param.code).lex());
    auto nodes = parser.parse();

    EXPECT_EQ(nodes.size(), 1) << "Expected 1 node, got " << nodes.size() << " nodes";
    EXPECT_EQ(nodes[0].to_string(), param.to_string_result) << "Expected " << param.to_string_result << ", got " << nodes[0].to_string();
}

INSTANTIATE_TEST_SUITE_P(
    VariableAssignmentVariations,
    VariableAssignmentTests,
    ::testing::Values(
        VariableTestParam{"x = 5 + 5", "true"},
        VariableTestParam{"x=10", "true"},
        VariableTestParam{"x += true", "true"},
        VariableTestParam{"x -= 10", "true"},
        VariableTestParam{"x *= 100 * 10", "true"},
        VariableTestParam{"x /= 5 + 5", "true"}
    )
);
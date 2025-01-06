#include <gtest/gtest.h>

#include "dragon/parser.h"
#include "dragon/token.h"
#include "dragon/lexer.h"

struct ExpressionsTestParam {
    std::string code;   
    std::string to_string_result;
};

class ExpressionsTests : public ::testing::TestWithParam<ExpressionsTestParam> {};

TEST_P(ExpressionsTests, HandlesExpressions) {
    ExpressionsTestParam param = GetParam();

    Parser parser(Lexer(param.code).lex());
    auto nodes = parser.parse_expression();

    EXPECT_EQ(nodes->size(), 1) << "Expected 1 node, got " << nodes->size() << " nodes";
    EXPECT_EQ(nodes->to_string(), param.to_string_result) << "Expected " << param.to_string_result << ", got " << nodes->to_string();
}

INSTANTIATE_TEST_SUITE_P(
    ExpressionsVariations,
    ExpressionsTests,
    ::testing::Values(
        ExpressionsTestParam{"5 + 5 - 10 * 100", "true"},
        ExpressionsTestParam{"10 / 4 + 3 * 4 - 9", "true"},
        ExpressionsTestParam{"(1 == 1) && true == false || !false", "true"},
        ExpressionsTestParam{"5 & 10 | 15 ^ 20", "true"},
        ExpressionsTestParam{"5 << 2 >> 2", "true"},
        ExpressionsTestParam{"5 < 10 > 15 <= 20 >= 25", "true"},
        ExpressionsTestParam{"5 == 5 != 10", "true"},
        ExpressionsTestParam{"5 && 10 || 15", "true"},
        ExpressionsTestParam{"5 ? 10 : 15", "true"}
    )
);

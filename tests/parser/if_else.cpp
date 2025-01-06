#include <gtest/gtest.h>

#include "dragon/parser.h"
#include "dragon/token.h"
#include "dragon/lexer.h"

struct IfElseTestParam {
    std::string code;   
    std::string to_string_result;
};

class IfElseTests : public ::testing::TestWithParam<IfElseTestParam> {};

TEST_P(IfElseTests, HandlesIfElse) {
    IfElseTestParam param = GetParam();

    Parser parser(Lexer(param.code).lex());
    auto nodes = parser.parse();

    EXPECT_EQ(nodes[0].to_string(), param.to_string_result) << "Expected " << param.to_string_result << ", got " << nodes[0].to_string();
}

INSTANTIATE_TEST_SUITE_P(
    IfElseVariations,
    IfElseTests,
    ::testing::Values(
        IfElseTestParam{"if true { print(5) }", "IfElseNode"},
        IfElseTestParam{"if true==true { print(5) } else { print(10) }", "IfElseNode"},
        IfElseTestParam{"if true { print(5) } else if false { print(10) }", "IfElseNode"},
        IfElseTestParam{"if true { print(5) } else if false { print(10) } else { print(15) }", "IfElseNode"}
    )
);
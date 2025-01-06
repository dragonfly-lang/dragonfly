#pragma once

#include "token.h"
#include "ast.h"

#include "ast/variable_declaration_node.h"
#include "ast/variable_assignment_node.h"
#include "ast/while_loop_node.h"
#include "ast/for_loop_node.h"
#include "ast/if_else_node.h"
#include "ast/function_call_node.h"


class Parser {
private:
    std::vector<Token> tokens;
    std::vector<Node> nodes;
    size_t index;

    std::optional<Token> peek() const;
    std::optional<Token> peek_next() const;
    std::optional<Token> advance(TokenType type);

public: 
    explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), index(0) {}
    explicit Parser() : tokens(std::vector<Token>()), index(0) {}

    void reset();

    std::vector<Node> parse();
    std::vector<Node> parse(const std::vector<Token>& tokens);
    
    std::unique_ptr<VariableDeclarationNode> parse_variable_declaration();
    std::unique_ptr<VariableAssignmentNode> parse_variable_assignment();
    std::unique_ptr<WhileLoopNode> parse_while_loop();
    std::unique_ptr<ForLoopNode> parse_for_loop();
    std::unique_ptr<IfElseNode> parse_if_else();
    std::unique_ptr<FunctionCallNode> parse_function_call();
    std::unique_ptr<ExpressionNode> parse_expression();
};
#pragma once 

#include <memory>
#include "../ast.h"

class VariableDeclarationNode : public StatementNode {
public:
    Token identifier;
    Token type;
    std::unique_ptr<ExpressionNode> value;
    bool is_const;

    explicit VariableDeclarationNode(Token identifier, Token type, std::unique_ptr<ExpressionNode> value, bool is_const) 
        : identifier(std::move(identifier)), type(std::move(type)), value(std::move(value)), is_const(is_const) {}

    ~VariableDeclarationNode() override = default;

    std::string to_string() const override {
        return "VariableDeclarationNode(identifier: " + identifier.to_string()
            + ", type: " + type.to_string() 
            + ", is_const: " + (is_const ? "true" : "false") + ")";
    }
};
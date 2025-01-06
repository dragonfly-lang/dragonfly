#pragma once 

#include <string>
#include <memory>
#include <vector>
#include <variant>
#include "token.h"

class Node {
public:
    virtual ~Node() = default;
    virtual std::string to_string() const = 0;
};

class ExpressionNode : public Node {
public:
    Token op;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    ExpressionNode(Token token, std::unique_ptr<Node> left, std::unique_ptr<Node> right) 
        : op(std::move(token)), left(std::move(left)), right(std::move(right)) {}

    ~ExpressionNode() override = default;

    std::string to_string() const override {
        return "ExpressionNode(op: " + op.to_string() + ")";
    }

    virtual std::variant<int, std::string, bool> eval() const = 0;
    virtual size_t size() const = 0;
};

class StatementNode : public Node {
public:
    virtual ~StatementNode() override = default;
    virtual std::string to_string() const override = 0; 
};



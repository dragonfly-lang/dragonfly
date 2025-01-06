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

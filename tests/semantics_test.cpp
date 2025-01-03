#include <gtest/gtest.h>

#include "dragon/lexer.h"
#include "dragon/token.h"
#include "dragon/parser.h"
#include "dragon/semantics.h"

// Duplicate variable/constant identifier
// Duplicate function call identifier
// Non-boolean expression for while loop
// Non-boolean expression for if statement
// For loop expression isn't iterable
// Function call argument type doesn't match parameter type
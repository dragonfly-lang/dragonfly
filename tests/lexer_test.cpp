#include <gtest/gtest.h>


#include "dragon/lexer.h"
#include "dragon/token.h"

// let <identifier> <identifier> 
// let <identifier> <identifier> = <expr>
// let <identifier> = <expr>
// const <identifier> <identifier> = <expr>
// const <identifier> <identifier> 
// const <identifier> = <expr>
// <identifier> = <expr>
// <identifier>(<expr>)
// while <expr> { <statement(s)> }
// for <identifier> in <identifier> { <statement(s)> }
// for <identifier> in <expr> { <statement(s)> }
// if <expr> { <statement(s)> }
// else { <statement(s)> }
// else if <expr> { <statement(s)> }



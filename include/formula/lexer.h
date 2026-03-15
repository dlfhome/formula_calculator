#pragma once

#include "formula/token.h"
#include <string>
#include <vector>

namespace formula {

/**
 * Lexer
 * Converts formula string to token sequence
 */
class Lexer {
public:
    explicit Lexer(const std::string& input);

    // Perform lexical analysis, return token list
    std::vector<Token> tokenize();

private:
    std::string input_;
    size_t position_;

    void skipWhitespace();
    Token readNumber();
    Token readIdentifier();
    Token readOperator();
    char peek() const;
    char advance();
    bool isAtEnd() const;
    bool match(char expected);
};

} // namespace formula

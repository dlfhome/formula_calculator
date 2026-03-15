#pragma once

#include <string>

namespace formula {

/**
 * Token type enumeration
 */
enum class TokenType {
    // Literals
    NUMBER,         // Number
    IDENTIFIER,     // Identifier (variable name, function name)

    // Arithmetic operators
    PLUS,           // +
    MINUS,          // -
    MULTIPLY,       // *
    DIVIDE,         // /
    MODULO,         // %
    POWER,          // ^

    // Comparison operators
    GREATER,        // >
    LESS,           // <
    EQUAL,          // ==
    GREATER_EQUAL,  // >=
    LESS_EQUAL,     // <=
    NOT_EQUAL,      // !=

    // Logical operators
    LOGICAL_AND,    // &&
    LOGICAL_OR,     // ||
    LOGICAL_NOT,    // !

    // Ternary operator
    QUESTION,       // ?
    COLON,          // :

    // Parentheses
    LEFT_PAREN,     // (
    RIGHT_PAREN,    // )

    // Comma (function argument separator)
    COMMA,          // ,

    // End markers
    END,            // End of input
    INVALID         // Invalid token
};

/**
 * Token structure
 */
struct Token {
    TokenType type;
    std::string value;
    size_t position;

    Token(TokenType t, const std::string& v, size_t pos)
        : type(t), value(v), position(pos) {}

    Token() : type(TokenType::INVALID), value(""), position(0) {}
};

/**
 * Convert TokenType to string
 */
const char* tokenTypeToString(TokenType type);

} // namespace formula

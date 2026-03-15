#pragma once

#include <string>

namespace formula {

/**
 * Token 类型枚举
 */
enum class TokenType {
    // 字面量
    NUMBER,         // 数字
    IDENTIFIER,     // 标识符（变量名、函数名）

    // 算术运算符
    PLUS,           // +
    MINUS,          // -
    MULTIPLY,       // *
    DIVIDE,         // /
    MODULO,         // %
    POWER,          // ^

    // 比较运算符
    GREATER,        // >
    LESS,           // <
    EQUAL,          // ==
    GREATER_EQUAL,  // >=
    LESS_EQUAL,     // <=
    NOT_EQUAL,      // !=

    // 逻辑运算符
    LOGICAL_AND,    // &&
    LOGICAL_OR,     // ||
    LOGICAL_NOT,    // !

    // 三元运算符
    QUESTION,       // ?
    COLON,          // :

    // 括号
    LEFT_PAREN,     // (
    RIGHT_PAREN,    // )

    // 逗号（函数参数分隔）
    COMMA,          // ,

    // 结束标记
    END,            // 输入结束
    INVALID         // 无效Token
};

/**
 * Token 结构
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
 * 将 TokenType 转换为字符串
 */
const char* tokenTypeToString(TokenType type);

} // namespace formula

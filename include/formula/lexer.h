#pragma once

#include "formula/token.h"
#include <string>
#include <vector>

namespace formula {

/**
 * 词法分析器
 * 将公式字符串转换为 Token 序列
 */
class Lexer {
public:
    explicit Lexer(const std::string& input);

    // 执行词法分析，返回 Token 列表
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

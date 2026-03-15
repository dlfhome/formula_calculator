#include "formula/lexer.h"
#include <cctype>
#include <stdexcept>

namespace formula {

Lexer::Lexer(const std::string& input) : input_(input), position_(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        skipWhitespace();

        if (isAtEnd()) break;

        char c = peek();

        if (std::isdigit(c) || c == '.') {
            tokens.push_back(readNumber());
        } else if (std::isalpha(c) || c == '_') {
            tokens.push_back(readIdentifier());
        } else {
            tokens.push_back(readOperator());
        }
    }

    tokens.emplace_back(TokenType::END, "", position_);
    return tokens;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(peek())) {
        advance();
    }
}

Token Lexer::readNumber() {
    size_t startPos = position_;
    std::string value;

    while (!isAtEnd() && (std::isdigit(peek()) || peek() == '.')) {
        value += advance();
    }

    // 处理科学计数法
    if (!isAtEnd() && (peek() == 'e' || peek() == 'E')) {
        value += advance();
        if (!isAtEnd() && (peek() == '+' || peek() == '-')) {
            value += advance();
        }
        while (!isAtEnd() && std::isdigit(peek())) {
            value += advance();
        }
    }

    return Token(TokenType::NUMBER, value, startPos);
}

Token Lexer::readIdentifier() {
    size_t startPos = position_;
    std::string value;

    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
        value += advance();
    }

    return Token(TokenType::IDENTIFIER, value, startPos);
}

Token Lexer::readOperator() {
    size_t startPos = position_;
    char c = advance();

    switch (c) {
        case '+': return Token(TokenType::PLUS, "+", startPos);
        case '-': return Token(TokenType::MINUS, "-", startPos);
        case '*': return Token(TokenType::MULTIPLY, "*", startPos);
        case '/': return Token(TokenType::DIVIDE, "/", startPos);
        case '%': return Token(TokenType::MODULO, "%", startPos);
        case '^': return Token(TokenType::POWER, "^", startPos);
        case '(': return Token(TokenType::LEFT_PAREN, "(", startPos);
        case ')': return Token(TokenType::RIGHT_PAREN, ")", startPos);
        case ',': return Token(TokenType::COMMA, ",", startPos);
        case '?': return Token(TokenType::QUESTION, "?", startPos);
        case ':': return Token(TokenType::COLON, ":", startPos);

        // 处理多字符运算符
        case '!':
            if (match('=')) {
                return Token(TokenType::NOT_EQUAL, "!=", startPos);
            }
            return Token(TokenType::LOGICAL_NOT, "!", startPos);

        case '=':
            if (match('=')) {
                return Token(TokenType::EQUAL, "==", startPos);
            }
            throw std::runtime_error("未知的字符: = (是否想使用 == ?)");

        case '>':
            if (match('=')) {
                return Token(TokenType::GREATER_EQUAL, ">=", startPos);
            }
            return Token(TokenType::GREATER, ">", startPos);

        case '<':
            if (match('=')) {
                return Token(TokenType::LESS_EQUAL, "<=", startPos);
            }
            return Token(TokenType::LESS, "<", startPos);

        case '&':
            if (match('&')) {
                return Token(TokenType::LOGICAL_AND, "&&", startPos);
            }
            throw std::runtime_error("未知的字符: & (是否想使用 && ?)");

        case '|':
            if (match('|')) {
                return Token(TokenType::LOGICAL_OR, "||", startPos);
            }
            throw std::runtime_error("未知的字符: | (是否想使用 || ?)");

        default:
            throw std::runtime_error(std::string("未知的字符: ") + c);
    }
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (input_[position_] != expected) return false;
    position_++;
    return true;
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return input_[position_];
}

char Lexer::advance() {
    if (isAtEnd()) return '\0';
    return input_[position_++];
}

bool Lexer::isAtEnd() const {
    return position_ >= input_.size();
}

} // namespace formula

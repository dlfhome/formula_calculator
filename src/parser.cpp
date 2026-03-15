#include "formula/parser.h"
#include <stdexcept>
#include <sstream>

namespace formula {

Parser::Parser(const std::vector<Token>& tokens,
               std::shared_ptr<FunctionRegistry> funcRegistry)
    : tokens_(tokens)
    , current_(0)
    , funcRegistry_(funcRegistry) {}

std::unique_ptr<AstNode> Parser::parse() {
    auto node = expression();
    if (hasError()) {
        throw ParseException(error_);
    }
    if (!isAtEnd()) {
        error("解析未完成");
        throw ParseException(error_);
    }
    return node;
}

bool Parser::hasError() const {
    return !error_.empty();
}

std::string Parser::getError() const {
    return error_;
}

// expression : ternary
std::unique_ptr<AstNode> Parser::expression() {
    return ternary();
}

// ternary : logical_or ('?' ternary ':' ternary)?
std::unique_ptr<AstNode> Parser::ternary() {
    auto node = logicalOr();

    if (match(TokenType::QUESTION)) {
        auto trueExpr = ternary();
        consume(TokenType::COLON, "期望 ':' 在三元运算符中");
        auto falseExpr = ternary();
        node = std::make_unique<TernaryOpNode>(std::move(node), std::move(trueExpr), std::move(falseExpr));
    }

    return node;
}

// logical_or : logical_and ('||' logical_and)*
std::unique_ptr<AstNode> Parser::logicalOr() {
    auto node = logicalAnd();

    while (match(TokenType::LOGICAL_OR)) {
        TokenType op = previous().type;
        auto right = logicalAnd();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(right));
    }

    return node;
}

// logical_and : equality ('&&' equality)*
std::unique_ptr<AstNode> Parser::logicalAnd() {
    auto node = equality();

    while (match(TokenType::LOGICAL_AND)) {
        TokenType op = previous().type;
        auto right = equality();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(right));
    }

    return node;
}

// equality : comparison (('==' | '!=') comparison)*
std::unique_ptr<AstNode> Parser::equality() {
    auto node = comparison();

    while (match({TokenType::EQUAL, TokenType::NOT_EQUAL})) {
        TokenType op = previous().type;
        auto right = comparison();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(right));
    }

    return node;
}

// comparison : additive (('>' | '<' | '>=' | '<=') additive)*
std::unique_ptr<AstNode> Parser::comparison() {
    auto node = additive();

    while (match({TokenType::GREATER, TokenType::LESS, TokenType::GREATER_EQUAL, TokenType::LESS_EQUAL})) {
        TokenType op = previous().type;
        auto right = additive();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(right));
    }

    return node;
}

// additive : multiplicative (('+' | '-') multiplicative)*
std::unique_ptr<AstNode> Parser::additive() {
    auto node = multiplicative();

    while (match({TokenType::PLUS, TokenType::MINUS})) {
        TokenType op = previous().type;
        auto right = multiplicative();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(right));
    }

    return node;
}

// multiplicative : power (('*' | '/' | '%') power)*
std::unique_ptr<AstNode> Parser::multiplicative() {
    auto node = power();

    while (match({TokenType::MULTIPLY, TokenType::DIVIDE, TokenType::MODULO})) {
        TokenType op = previous().type;
        auto right = power();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(right));
    }

    return node;
}

// power : unary ('^' power)?
std::unique_ptr<AstNode> Parser::power() {
    auto node = unary();

    if (match(TokenType::POWER)) {
        auto right = power();
        node = std::make_unique<BinaryOpNode>(TokenType::POWER, std::move(node), std::move(right));
    }

    return node;
}

// unary : ('!' | '+' | '-') unary | primary
std::unique_ptr<AstNode> Parser::unary() {
    if (match({TokenType::LOGICAL_NOT, TokenType::PLUS, TokenType::MINUS})) {
        TokenType op = previous().type;
        auto operand = unary();
        return std::make_unique<UnaryOpNode>(op, std::move(operand));
    }

    return primary();
}

// primary : NUMBER | IDENTIFIER | IDENTIFIER '(' argument_list ')' | '(' expression ')'
std::unique_ptr<AstNode> Parser::primary() {
    if (match(TokenType::NUMBER)) {
        double value = std::stod(previous().value);
        return std::make_unique<NumberNode>(value);
    }

    if (match(TokenType::IDENTIFIER)) {
        std::string name = previous().value;

        // 检查是否是函数调用
        if (match(TokenType::LEFT_PAREN)) {
            std::vector<std::unique_ptr<AstNode>> args;

            if (!check(TokenType::RIGHT_PAREN)) {
                do {
                    args.push_back(expression());
                } while (match(TokenType::COMMA));
            }

            consume(TokenType::RIGHT_PAREN, "期望 ')'");
            return std::make_unique<FunctionCallNode>(name, std::move(args));
        }

        // 变量
        return std::make_unique<VariableNode>(name);
    }

    if (match(TokenType::LEFT_PAREN)) {
        auto node = expression();
        consume(TokenType::RIGHT_PAREN, "期望 ')'");
        return node;
    }

    error("期望表达式");
    return nullptr;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();

    error(message);
    return Token();
}

Token Parser::peek() const {
    if (isAtEnd()) return Token(TokenType::END, "", 0);
    return tokens_[current_];
}

Token Parser::previous() const {
    return tokens_[current_ - 1];
}

bool Parser::isAtEnd() const {
    return current_ >= tokens_.size() || tokens_[current_].type == TokenType::END;
}

Token Parser::advance() {
    if (!isAtEnd()) current_++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

void Parser::error(const std::string& message) {
    if (!error_.empty()) return;

    std::ostringstream oss;
    oss << "解析错误 [位置 " << peek().position << "]: " << message;
    if (peek().type != TokenType::END) {
        oss << "，遇到 '" << peek().value << "'";
    }
    error_ = oss.str();
}

} // namespace formula

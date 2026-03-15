#pragma once

#include "formula/token.h"
#include "formula/ast.h"
#include "formula/function_registry.h"
#include <vector>
#include <memory>
#include <string>

namespace formula {

/**
 * 解析异常
 */
class ParseException : public std::runtime_error {
public:
    explicit ParseException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * 语法分析器
 * 使用递归下降解析算法
 */
class Parser {
public:
    Parser(const std::vector<Token>& tokens,
           std::shared_ptr<FunctionRegistry> funcRegistry);

    // 解析 Token 序列，返回 AST 根节点
    std::unique_ptr<AstNode> parse();

    // 获取解析错误
    bool hasError() const;
    std::string getError() const;

private:
    std::vector<Token> tokens_;
    size_t current_;
    std::shared_ptr<FunctionRegistry> funcRegistry_;
    std::string error_;

    // 递归下降解析函数（按优先级从低到高）
    // expression : ternary
    std::unique_ptr<AstNode> expression();

    // ternary : logical_or ('?' ternary ':' ternary)?
    std::unique_ptr<AstNode> ternary();

    // logical_or : logical_and ('||' logical_and)*
    std::unique_ptr<AstNode> logicalOr();

    // logical_and : equality ('&&' equality)*
    std::unique_ptr<AstNode> logicalAnd();

    // equality : comparison (('==' | '!=') comparison)*
    std::unique_ptr<AstNode> equality();

    // comparison : additive (('>' | '<' | '>=' | '<=') additive)*
    std::unique_ptr<AstNode> comparison();

    // additive : multiplicative (('+' | '-') multiplicative)*
    std::unique_ptr<AstNode> additive();

    // multiplicative : power (('*' | '/' | '%') power)*
    std::unique_ptr<AstNode> multiplicative();

    // power : unary ('^' power)?
    std::unique_ptr<AstNode> power();

    // unary : ('!' | '+' | '-') unary | primary
    std::unique_ptr<AstNode> unary();

    // primary : NUMBER | IDENTIFIER | IDENTIFIER '(' argument_list ')' | '(' expression ')'
    std::unique_ptr<AstNode> primary();

    // 辅助函数
    bool match(TokenType type);
    bool match(std::initializer_list<TokenType> types);
    Token consume(TokenType type, const std::string& message);
    Token peek() const;
    Token previous() const;
    bool isAtEnd() const;
    Token advance();
    bool check(TokenType type) const;

    void error(const std::string& message);
};

} // namespace formula

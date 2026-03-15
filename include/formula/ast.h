#pragma once

#include "formula/token.h"
#include "formula/value.h"
#include <memory>
#include <string>
#include <vector>

namespace formula {

/**
 * AST 节点类型枚举
 */
enum class NodeType {
    BINARY_OP,      // 二元运算
    UNARY_OP,       // 一元运算
    TERNARY_OP,     // 三元运算
    NUMBER,         // 数字字面量
    VARIABLE,       // 变量
    FUNCTION_CALL   // 函数调用
};

/**
 * AST 访问者接口（前向声明）
 */
class AstVisitor;

/**
 * AST 节点基类
 */
class AstNode {
public:
    virtual ~AstNode() = default;

    NodeType getType() const { return type_; }

    // 访问者模式接口 - 返回 Value 类型
    virtual Value accept(AstVisitor* visitor) = 0;

protected:
    AstNode(NodeType type) : type_(type) {}

    NodeType type_;
};

/**
 * AST 访问者接口
 */
class AstVisitor {
public:
    virtual ~AstVisitor() = default;

    virtual Value visitBinaryOp(class BinaryOpNode* node) = 0;
    virtual Value visitUnaryOp(class UnaryOpNode* node) = 0;
    virtual Value visitTernaryOp(class TernaryOpNode* node) = 0;
    virtual Value visitNumber(class NumberNode* node) = 0;
    virtual Value visitVariable(class VariableNode* node) = 0;
    virtual Value visitFunctionCall(class FunctionCallNode* node) = 0;
};

/**
 * 二元运算节点
 */
class BinaryOpNode : public AstNode {
public:
    TokenType op;
    std::unique_ptr<AstNode> left;
    std::unique_ptr<AstNode> right;

    BinaryOpNode(TokenType oper, std::unique_ptr<AstNode> l, std::unique_ptr<AstNode> r)
        : AstNode(NodeType::BINARY_OP), op(oper), left(std::move(l)), right(std::move(r)) {}

    Value accept(AstVisitor* visitor) override;
};

/**
 * 一元运算节点
 */
class UnaryOpNode : public AstNode {
public:
    TokenType op;
    std::unique_ptr<AstNode> operand;

    UnaryOpNode(TokenType oper, std::unique_ptr<AstNode> expr)
        : AstNode(NodeType::UNARY_OP), op(oper), operand(std::move(expr)) {}

    Value accept(AstVisitor* visitor) override;
};

/**
 * 三元运算节点 (condition ? trueExpr : falseExpr)
 */
class TernaryOpNode : public AstNode {
public:
    std::unique_ptr<AstNode> condition;
    std::unique_ptr<AstNode> trueExpr;
    std::unique_ptr<AstNode> falseExpr;

    TernaryOpNode(std::unique_ptr<AstNode> cond,
                  std::unique_ptr<AstNode> trueExp,
                  std::unique_ptr<AstNode> falseExp)
        : AstNode(NodeType::TERNARY_OP),
          condition(std::move(cond)),
          trueExpr(std::move(trueExp)),
          falseExpr(std::move(falseExp)) {}

    Value accept(AstVisitor* visitor) override;
};

/**
 * 数字节点
 */
class NumberNode : public AstNode {
public:
    double value;

    explicit NumberNode(double val)
        : AstNode(NodeType::NUMBER), value(val) {}

    Value accept(AstVisitor* visitor) override;
};

/**
 * 变量节点
 */
class VariableNode : public AstNode {
public:
    std::string name;

    explicit VariableNode(std::string varName)
        : AstNode(NodeType::VARIABLE), name(std::move(varName)) {}

    Value accept(AstVisitor* visitor) override;
};

/**
 * 函数调用节点
 */
class FunctionCallNode : public AstNode {
public:
    std::string name;
    std::vector<std::unique_ptr<AstNode>> arguments;

    FunctionCallNode(std::string funcName, std::vector<std::unique_ptr<AstNode>> args)
        : AstNode(NodeType::FUNCTION_CALL), name(std::move(funcName)), arguments(std::move(args)) {}

    Value accept(AstVisitor* visitor) override;
};

} // namespace formula

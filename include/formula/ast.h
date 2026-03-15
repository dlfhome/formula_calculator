#pragma once

#include "formula/token.h"
#include "formula/value.h"
#include <memory>
#include <string>
#include <vector>

namespace formula {

/**
 * AST Node type enumeration
 */
enum class NodeType {
    BINARY_OP,      // Binary operation
    UNARY_OP,       // Unary operation
    TERNARY_OP,     // Ternary operation
    NUMBER,         // Numeric literal
    VARIABLE,       // Variable
    FUNCTION_CALL   // Function call
};

/**
 * AST Visitor interface (forward declaration)
 */
class AstVisitor;

/**
 * AST Node base class
 */
class AstNode {
public:
    virtual ~AstNode() = default;

    NodeType getType() const { return type_; }

    // Visitor pattern interface - returns Value type
    virtual Value accept(AstVisitor* visitor) = 0;

protected:
    AstNode(NodeType type) : type_(type) {}

    NodeType type_;
};

/**
 * AST Visitor interface
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
 * Binary operation node
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
 * Unary operation node
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
 * Ternary operation node (condition ? trueExpr : falseExpr)
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
 * Number node
 */
class NumberNode : public AstNode {
public:
    double value;

    explicit NumberNode(double val)
        : AstNode(NodeType::NUMBER), value(val) {}

    Value accept(AstVisitor* visitor) override;
};

/**
 * Variable node
 */
class VariableNode : public AstNode {
public:
    std::string name;

    explicit VariableNode(std::string varName)
        : AstNode(NodeType::VARIABLE), name(std::move(varName)) {}

    Value accept(AstVisitor* visitor) override;
};

/**
 * Function call node
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

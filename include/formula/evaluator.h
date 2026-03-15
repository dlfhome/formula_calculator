#pragma once

#include "formula/ast.h"
#include "formula/variable_manager.h"
#include "formula/function_registry.h"
#include "formula/constant_registry.h"
#include <memory>

namespace formula {

/**
 * AST evaluator (visitor pattern implementation)
 */
class Evaluator : public AstVisitor {
public:
    Evaluator(std::shared_ptr<VariableManager> varManager,
              std::shared_ptr<FunctionRegistry> funcRegistry,
              std::shared_ptr<ConstantRegistry> constRegistry);

    // Evaluation entry point
    Value evaluate(AstNode* node);

    // Visitor method implementations
    Value visitBinaryOp(BinaryOpNode* node) override;
    Value visitUnaryOp(UnaryOpNode* node) override;
    Value visitTernaryOp(TernaryOpNode* node) override;
    Value visitNumber(NumberNode* node) override;
    Value visitVariable(VariableNode* node) override;
    Value visitFunctionCall(FunctionCallNode* node) override;

private:
    std::shared_ptr<VariableManager> varManager_;
    std::shared_ptr<FunctionRegistry> funcRegistry_;
    std::shared_ptr<ConstantRegistry> constRegistry_;

    // Helper function: perform arithmetic operations
    Value performArithmetic(Value left, Value right, TokenType op);

    // Helper function: perform comparison operations
    Value performComparison(Value left, Value right, TokenType op);

    // Helper function: perform equality comparison
    Value performEquality(Value left, Value right, TokenType op);

    // Helper function: perform logical operations
    Value performLogical(Value left, Value right, TokenType op);
};

} // namespace formula

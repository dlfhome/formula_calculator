#pragma once

#include "formula/ast.h"
#include "formula/variable_manager.h"
#include "formula/function_registry.h"
#include "formula/constant_registry.h"
#include <memory>

namespace formula {

/**
 * AST 求值器（访问者模式实现）
 */
class Evaluator : public AstVisitor {
public:
    Evaluator(std::shared_ptr<VariableManager> varManager,
              std::shared_ptr<FunctionRegistry> funcRegistry,
              std::shared_ptr<ConstantRegistry> constRegistry);

    // 求值入口
    Value evaluate(AstNode* node);

    // 访问者方法实现
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

    // 辅助函数：执行算术运算
    Value performArithmetic(Value left, Value right, TokenType op);

    // 辅助函数：执行比较运算
    Value performComparison(Value left, Value right, TokenType op);

    // 辅助函数：执行相等性比较
    Value performEquality(Value left, Value right, TokenType op);

    // 辅助函数：执行逻辑运算
    Value performLogical(Value left, Value right, TokenType op);
};

} // namespace formula

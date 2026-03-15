#include "formula/evaluator.h"
#include <cmath>
#include <stdexcept>

namespace formula {

Evaluator::Evaluator(std::shared_ptr<VariableManager> varManager,
                     std::shared_ptr<FunctionRegistry> funcRegistry,
                     std::shared_ptr<ConstantRegistry> constRegistry)
    : varManager_(varManager)
    , funcRegistry_(funcRegistry)
    , constRegistry_(constRegistry) {}

Value Evaluator::evaluate(AstNode* node) {
    if (!node) {
        throw std::runtime_error("空节点");
    }
    return node->accept(this);
}

Value Evaluator::visitBinaryOp(BinaryOpNode* node) {
    Value left = evaluate(node->left.get());
    Value right = evaluate(node->right.get());

    switch (node->op) {
        // 算术运算符
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MULTIPLY:
        case TokenType::DIVIDE:
        case TokenType::MODULO:
        case TokenType::POWER:
            return performArithmetic(left, right, node->op);

        // 比较运算符
        case TokenType::GREATER:
        case TokenType::LESS:
        case TokenType::GREATER_EQUAL:
        case TokenType::LESS_EQUAL:
            return performComparison(left, right, node->op);

        // 相等性运算符
        case TokenType::EQUAL:
        case TokenType::NOT_EQUAL:
            return performEquality(left, right, node->op);

        // 逻辑运算符
        case TokenType::LOGICAL_AND:
        case TokenType::LOGICAL_OR:
            return performLogical(left, right, node->op);

        default:
            throw std::runtime_error("未知的二元运算符");
    }
}

Value Evaluator::visitUnaryOp(UnaryOpNode* node) {
    Value operand = evaluate(node->operand.get());

    switch (node->op) {
        case TokenType::PLUS:
            return Value(operand.asDouble());
        case TokenType::MINUS:
            return Value(-operand.asDouble());
        case TokenType::LOGICAL_NOT:
            return Value(!operand.asBool());
        default:
            throw std::runtime_error("未知的一元运算符");
    }
}

Value Evaluator::visitTernaryOp(TernaryOpNode* node) {
    Value condition = evaluate(node->condition.get());

    // 根据条件选择分支
    if (condition.asBool()) {
        return evaluate(node->trueExpr.get());
    } else {
        return evaluate(node->falseExpr.get());
    }
}

Value Evaluator::visitNumber(NumberNode* node) {
    return Value(node->value);
}

Value Evaluator::visitVariable(VariableNode* node) {
    // 先检查是否是常量
    if (constRegistry_->hasConstant(node->name)) {
        return Value(constRegistry_->getConstant(node->name));
    }
    // 再检查变量
    return varManager_->getVariable(node->name);
}

Value Evaluator::visitFunctionCall(FunctionCallNode* node) {
    auto funcInfo = funcRegistry_->getFunction(node->name);

    // 计算参数值
    std::vector<Value> args;
    args.reserve(node->arguments.size());
    for (auto& arg : node->arguments) {
        args.push_back(evaluate(arg.get()));
    }

    // 检查参数个数
    if (funcInfo.arity != -1 && static_cast<int>(args.size()) != funcInfo.arity) {
        throw std::runtime_error("函数 " + node->name + " 参数个数错误");
    }

    // 将 Value 转换为 double 调用函数
    std::vector<double> doubleArgs;
    doubleArgs.reserve(args.size());
    for (const auto& arg : args) {
        doubleArgs.push_back(arg.asDouble());
    }

    return Value(funcInfo.function(doubleArgs));
}

Value Evaluator::performArithmetic(Value left, Value right, TokenType op) {
    double l = left.asDouble();
    double r = right.asDouble();

    switch (op) {
        case TokenType::PLUS:
            return Value(l + r);
        case TokenType::MINUS:
            return Value(l - r);
        case TokenType::MULTIPLY:
            return Value(l * r);
        case TokenType::DIVIDE:
            if (r == 0.0) {
                throw std::runtime_error("除零错误");
            }
            return Value(l / r);
        case TokenType::MODULO:
            if (r == 0.0) {
                throw std::runtime_error("除零错误");
            }
            return Value(std::fmod(l, r));
        case TokenType::POWER:
            return Value(std::pow(l, r));
        default:
            throw std::runtime_error("未知的算术运算符");
    }
}

Value Evaluator::performComparison(Value left, Value right, TokenType op) {
    double l = left.asDouble();
    double r = right.asDouble();

    switch (op) {
        case TokenType::GREATER:
            return Value(l > r);
        case TokenType::LESS:
            return Value(l < r);
        case TokenType::GREATER_EQUAL:
            return Value(l >= r);
        case TokenType::LESS_EQUAL:
            return Value(l <= r);
        default:
            throw std::runtime_error("未知的比较运算符");
    }
}

Value Evaluator::performEquality(Value left, Value right, TokenType op) {
    bool result = (left == right);

    switch (op) {
        case TokenType::EQUAL:
            return Value(result);
        case TokenType::NOT_EQUAL:
            return Value(!result);
        default:
            throw std::runtime_error("未知的相等性运算符");
    }
}

Value Evaluator::performLogical(Value left, Value right, TokenType op) {
    bool l = left.asBool();
    bool r = right.asBool();

    switch (op) {
        case TokenType::LOGICAL_AND:
            return Value(l && r);
        case TokenType::LOGICAL_OR:
            return Value(l || r);
        default:
            throw std::runtime_error("未知的逻辑运算符");
    }
}

} // namespace formula

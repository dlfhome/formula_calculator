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
        throw std::runtime_error("Null node");
    }
    return node->accept(this);
}

Value Evaluator::visitBinaryOp(BinaryOpNode* node) {
    Value left = evaluate(node->left.get());
    Value right = evaluate(node->right.get());

    switch (node->op) {
        // Arithmetic operators
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MULTIPLY:
        case TokenType::DIVIDE:
        case TokenType::MODULO:
        case TokenType::POWER:
            return performArithmetic(left, right, node->op);

        // Comparison operators
        case TokenType::GREATER:
        case TokenType::LESS:
        case TokenType::GREATER_EQUAL:
        case TokenType::LESS_EQUAL:
            return performComparison(left, right, node->op);

        // Equality operators
        case TokenType::EQUAL:
        case TokenType::NOT_EQUAL:
            return performEquality(left, right, node->op);

        // Logical operators
        case TokenType::LOGICAL_AND:
        case TokenType::LOGICAL_OR:
            return performLogical(left, right, node->op);

        default:
            throw std::runtime_error("Unknown binary operator");
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
            throw std::runtime_error("Unknown unary operator");
    }
}

Value Evaluator::visitTernaryOp(TernaryOpNode* node) {
    Value condition = evaluate(node->condition.get());

    // Select branch based on condition
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
    // First check if it is a constant
    if (constRegistry_->hasConstant(node->name)) {
        return Value(constRegistry_->getConstant(node->name));
    }
    // Then check variable
    return varManager_->getVariable(node->name);
}

Value Evaluator::visitFunctionCall(FunctionCallNode* node) {
    auto funcInfo = funcRegistry_->getFunction(node->name);

    // Calculate argument values
    std::vector<Value> args;
    args.reserve(node->arguments.size());
    for (auto& arg : node->arguments) {
        args.push_back(evaluate(arg.get()));
    }

    // Check argument count
    if (funcInfo.arity != -1 && static_cast<int>(args.size()) != funcInfo.arity) {
        throw std::runtime_error("Function " + node->name + ": invalid argument count");
    }

    // Convert Value to double and call function
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
                throw std::runtime_error("Division by zero");
            }
            return Value(l / r);
        case TokenType::MODULO:
            if (r == 0.0) {
                throw std::runtime_error("Division by zero");
            }
            return Value(std::fmod(l, r));
        case TokenType::POWER:
            return Value(std::pow(l, r));
        default:
            throw std::runtime_error("Unknown arithmetic operator");
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
            throw std::runtime_error("Unknown comparison operator");
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
            throw std::runtime_error("Unknown equality operator");
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
            throw std::runtime_error("Unknown logical operator");
    }
}

} // namespace formula

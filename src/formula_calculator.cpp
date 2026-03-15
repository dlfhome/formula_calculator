#include "formula/formula_calculator.h"
#include "formula/lexer.h"
#include "formula/parser.h"
#include "formula/evaluator.h"
#include "formula/variable_manager.h"
#include "formula/function_registry.h"
#include "formula/constant_registry.h"

namespace formula {

FormulaCalculator::FormulaCalculator()
    : parser_(nullptr)
    , evaluator_(nullptr)
    , varManager_(std::make_shared<VariableManager>())
    , funcRegistry_(std::make_shared<FunctionRegistry>())
    , constRegistry_(std::make_shared<ConstantRegistry>())
    , ast_(nullptr)
    , isCompiled_(false) {}

FormulaCalculator::~FormulaCalculator() = default;

FormulaCalculator::FormulaCalculator(FormulaCalculator&&) noexcept = default;
FormulaCalculator& FormulaCalculator::operator=(FormulaCalculator&&) noexcept = default;

bool FormulaCalculator::compile(const std::string& formula) {
    clear();

    try {
        // 词法分析
        Lexer lexer(formula);
        auto tokens = lexer.tokenize();

        // 语法分析
        parser_ = std::make_unique<Parser>(tokens, funcRegistry_);
        ast_ = parser_->parse();

        // 创建求值器
        evaluator_ = std::make_unique<Evaluator>(varManager_, funcRegistry_, constRegistry_);

        isCompiled_ = true;
        return true;
    } catch (const std::exception& e) {
        lastError_ = e.what();
        return false;
    }
}

Value FormulaCalculator::evaluate() {
    if (!isCompiled_) {
        throw std::runtime_error("公式未编译");
    }

    try {
        return evaluator_->evaluate(ast_.get());
    } catch (const std::exception& e) {
        lastError_ = e.what();
        throw;
    }
}

void FormulaCalculator::setVariable(const std::string& name, const Value& value) {
    varManager_->setVariable(name, value);
}

Value FormulaCalculator::getVariable(const std::string& name) const {
    try {
        return varManager_->getVariable(name);
    } catch (const std::exception& e) {
        throw std::runtime_error(e.what());
    }
}

bool FormulaCalculator::hasVariable(const std::string& name) const {
    return varManager_->hasVariable(name);
}

bool FormulaCalculator::registerFunction(const std::string& name, FunctionPtr func, int arity) {
    // 包装 Value 函数为 FunctionInfo
    auto wrapper = [func](const std::vector<double>& args) -> double {
        std::vector<Value> valueArgs;
        valueArgs.reserve(args.size());
        for (double arg : args) {
            valueArgs.push_back(Value(arg));
        }
        return func(valueArgs).asDouble();
    };
    FunctionInfo info(wrapper, arity);
    return funcRegistry_->registerFunction(name, info);
}

bool FormulaCalculator::registerFunction(const std::string& name, LegacyFunctionPtr func, int arity) {
    FunctionInfo info(func, arity);
    return funcRegistry_->registerFunction(name, info);
}

std::string FormulaCalculator::getLastError() const {
    return lastError_;
}

void FormulaCalculator::clear() {
    ast_.reset();
    parser_.reset();
    evaluator_.reset();
    lastError_.clear();
    isCompiled_ = false;
}

std::vector<std::string> FormulaCalculator::getSupportedFunctions() const {
    return funcRegistry_->getFunctionNames();
}

std::vector<std::string> FormulaCalculator::getSupportedConstants() const {
    return constRegistry_->getConstantNames();
}

bool FormulaCalculator::isCompiled() const {
    return isCompiled_;
}

} // namespace formula

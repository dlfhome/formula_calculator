#pragma once

#include "formula/value.h"
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace formula {

// Forward declarations
class AstNode;
class Parser;
class Evaluator;
class VariableManager;
class FunctionRegistry;
class ConstantRegistry;

// 函数类型定义 - 使用 Value 类型
using FunctionPtr = std::function<Value(const std::vector<Value>&)>;
using LegacyFunctionPtr = std::function<double(const std::vector<double>&)>;

/**
 * Formula Calculator class
 * Main entry class providing formula compilation, evaluation, and variable management
 */
class FormulaCalculator {
public:
    FormulaCalculator();
    ~FormulaCalculator();

    // Disable copy, allow move
    FormulaCalculator(const FormulaCalculator&) = delete;
    FormulaCalculator& operator=(const FormulaCalculator&) = delete;
    FormulaCalculator(FormulaCalculator&&) noexcept;
    FormulaCalculator& operator=(FormulaCalculator&&) noexcept;

    /**
     * Compile formula
     * @param formula Formula string
     * @return Returns true if compilation succeeds
     */
    bool compile(const std::string& formula);

    /**
     * Calculate formula result
     * @return Calculation result (Value type)
     */
    Value evaluate();


    /**
     * Set variable value
     * @param name Variable name
     * @param value Variable value (Value type)
     */
    void setVariable(const std::string& name, const Value& value);

    /**
     * Get variable value
     * @param name Variable name
     * @return Variable value (Value type)
     */
    Value getVariable(const std::string& name) const;

    /**
     * Check if variable exists
     * @param name Variable name
     * @return Returns true if exists
     */
    bool hasVariable(const std::string& name) const;

    /**
     * Register custom function (Value version)
     * @param name Function name
     * @param func Function implementation
     * @param arity Number of arguments, -1 for variadic
     * @return Returns true if registration succeeds
     */
    bool registerFunction(const std::string& name, FunctionPtr func, int arity = -1);

    /**
     * Register custom function (double version, for backward compatibility)
     * @param name Function name
     * @param func Function implementation
     * @param arity Number of arguments, -1 for variadic
     * @return Returns true if registration succeeds
     */
    bool registerFunction(const std::string& name, LegacyFunctionPtr func, int arity = -1);

    /**
     * Get last error message
     * @return Error message string
     */
    std::string getLastError() const;

    /**
     * Clear all state (variables, AST, etc.)
     */
    void clear();

    /**
     * Get list of supported functions
     * @return Function name list
     */
    std::vector<std::string> getSupportedFunctions() const;

    /**
     * Get list of supported constants
     * @return Constant name list
     */
    std::vector<std::string> getSupportedConstants() const;

    /**
     * Check if formula is compiled
     * @return Returns true if compiled
     */
    bool isCompiled() const;

private:
    std::unique_ptr<Parser> parser_;
    std::unique_ptr<Evaluator> evaluator_;
    std::shared_ptr<VariableManager> varManager_;
    std::shared_ptr<FunctionRegistry> funcRegistry_;
    std::shared_ptr<ConstantRegistry> constRegistry_;
    std::shared_ptr<AstNode> ast_;
    std::string lastError_;
    bool isCompiled_;
};

} // namespace formula

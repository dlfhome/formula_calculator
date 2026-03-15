#pragma once

#include "formula/value.h"
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace formula {

// 前向声明
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
 * 公式计算器类
 * 提供公式编译、求值、变量管理等功能的主入口类
 */
class FormulaCalculator {
public:
    FormulaCalculator();
    ~FormulaCalculator();

    // 禁止拷贝，允许移动
    FormulaCalculator(const FormulaCalculator&) = delete;
    FormulaCalculator& operator=(const FormulaCalculator&) = delete;
    FormulaCalculator(FormulaCalculator&&) noexcept;
    FormulaCalculator& operator=(FormulaCalculator&&) noexcept;

    /**
     * 编译公式
     * @param formula 公式字符串
     * @return 编译成功返回 true
     */
    bool compile(const std::string& formula);

    /**
     * 计算公式结果
     * @return 计算结果（Value 类型）
     */
    Value evaluate();


    /**
     * 设置变量值
     * @param name 变量名
     * @param value 变量值（Value 类型）
     */
    void setVariable(const std::string& name, const Value& value);

    /**
     * 获取变量值
     * @param name 变量名
     * @return 变量值（Value 类型）
     */
    Value getVariable(const std::string& name) const;

    /**
     * 检查变量是否存在
     * @param name 变量名
     * @return 存在返回 true
     */
    bool hasVariable(const std::string& name) const;

    /**
     * 注册自定义函数（Value 版本）
     * @param name 函数名
     * @param func 函数实现
     * @param arity 参数个数，-1 表示可变参数
     * @return 注册成功返回 true
     */
    bool registerFunction(const std::string& name, FunctionPtr func, int arity = -1);

    /**
     * 注册自定义函数（double 版本，保持向后兼容）
     * @param name 函数名
     * @param func 函数实现
     * @param arity 参数个数，-1 表示可变参数
     * @return 注册成功返回 true
     */
    bool registerFunction(const std::string& name, LegacyFunctionPtr func, int arity = -1);

    /**
     * 获取最后一次错误信息
     * @return 错误信息字符串
     */
    std::string getLastError() const;

    /**
     * 清空所有状态（变量、AST等）
     */
    void clear();

    /**
     * 获取支持的函数列表
     * @return 函数名列表
     */
    std::vector<std::string> getSupportedFunctions() const;

    /**
     * 获取支持的常量列表
     * @return 常量名列表
     */
    std::vector<std::string> getSupportedConstants() const;

    /**
     * 检查公式是否已编译
     * @return 已编译返回 true
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

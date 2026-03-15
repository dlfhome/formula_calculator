#pragma once

#include "formula/value.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <shared_mutex>
#include <stdexcept>

namespace formula {

/**
 * 变量异常
 */
class VariableException : public std::runtime_error {
public:
    explicit VariableException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * 变量管理器
 * 管理公式中使用的变量及其值
 */
class VariableManager {
public:
    VariableManager() = default;

    // 设置变量值（double 版本，保持向后兼容）
    void setVariable(const std::string& name, double value);

    // 设置变量值（Value 版本）
    void setVariable(const std::string& name, const Value& value);

    // 获取变量值，不存在则抛出异常
    Value getVariable(const std::string& name) const;

    // 获取变量值（double 版本，保持向后兼容）
    double getVariableAsDouble(const std::string& name) const;

    // 检查变量是否存在
    bool hasVariable(const std::string& name) const;

    // 删除变量
    bool removeVariable(const std::string& name);

    // 清空所有变量
    void clear();

    // 获取所有变量名
    std::vector<std::string> getVariableNames() const;

    // 获取变量数量
    size_t getVariableCount() const;

private:
    std::unordered_map<std::string, Value> variables_;
    mutable std::shared_mutex mutex_;
};

} // namespace formula

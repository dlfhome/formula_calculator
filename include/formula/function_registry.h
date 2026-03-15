#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <shared_mutex>
#include <stdexcept>

namespace formula {

/**
 * 函数异常
 */
class FunctionException : public std::runtime_error {
public:
    explicit FunctionException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * 函数信息结构
 */
struct FunctionInfo {
    std::function<double(const std::vector<double>&)> function;
    int arity;  // 参数个数，-1 表示可变参数
    std::string description;

    FunctionInfo() : arity(0) {}
    FunctionInfo(std::function<double(const std::vector<double>&)> func, int ar, const std::string& desc = "")
        : function(func), arity(ar), description(desc) {}
};

/**
 * 函数注册表
 * 管理所有可用的数学函数
 */
class FunctionRegistry {
public:
    FunctionRegistry();

    // 注册函数
    bool registerFunction(const std::string& name, const FunctionInfo& info);

    // 获取函数信息
    FunctionInfo getFunction(const std::string& name) const;

    // 检查函数是否存在
    bool hasFunction(const std::string& name) const;

    // 注销函数
    bool unregisterFunction(const std::string& name);

    // 获取所有函数名
    std::vector<std::string> getFunctionNames() const;

private:
    std::unordered_map<std::string, FunctionInfo> functions_;
    mutable std::shared_mutex mutex_;

    // 注册默认数学函数
    void registerDefaultFunctions();
};

} // namespace formula

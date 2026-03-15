#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <shared_mutex>
#include <stdexcept>

namespace formula {

/**
 * 常量异常
 */
class ConstantException : public std::runtime_error {
public:
    explicit ConstantException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * 常量注册表
 * 管理预定义的数学常量
 */
class ConstantRegistry {
public:
    ConstantRegistry();

    // 注册常量
    bool registerConstant(const std::string& name, double value);

    // 获取常量值
    double getConstant(const std::string& name) const;

    // 检查常量是否存在
    bool hasConstant(const std::string& name) const;

    // 注销常量
    bool unregisterConstant(const std::string& name);

    // 获取所有常量名
    std::vector<std::string> getConstantNames() const;

private:
    std::unordered_map<std::string, double> constants_;
    mutable std::shared_mutex mutex_;

    // 注册默认常量
    void registerDefaultConstants();
};

} // namespace formula

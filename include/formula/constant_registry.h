#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <shared_mutex>
#include <stdexcept>

namespace formula {

/**
 * Constant exception
 */
class ConstantException : public std::runtime_error {
public:
    explicit ConstantException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * Constant registry
 * Manages predefined mathematical constants
 */
class ConstantRegistry {
public:
    ConstantRegistry();

    // Register constant
    bool registerConstant(const std::string& name, double value);

    // Get constant value
    double getConstant(const std::string& name) const;

    // Check if constant exists
    bool hasConstant(const std::string& name) const;

    // Unregister constant
    bool unregisterConstant(const std::string& name);

    // Get all constant names
    std::vector<std::string> getConstantNames() const;

private:
    std::unordered_map<std::string, double> constants_;
    mutable std::shared_mutex mutex_;

    // Register default constants
    void registerDefaultConstants();
};

} // namespace formula

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <shared_mutex>
#include <stdexcept>

namespace formula {

/**
 * Function exception
 */
class FunctionException : public std::runtime_error {
public:
    explicit FunctionException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * Function info structure
 */
struct FunctionInfo {
    std::function<double(const std::vector<double>&)> function;
    int arity;  // Number of parameters, -1 means variadic
    std::string description;

    FunctionInfo() : arity(0) {}
    FunctionInfo(std::function<double(const std::vector<double>&)> func, int ar, const std::string& desc = "")
        : function(func), arity(ar), description(desc) {}
};

/**
 * Function registry
 * Manages all available mathematical functions
 */
class FunctionRegistry {
public:
    FunctionRegistry();

    // Register function
    bool registerFunction(const std::string& name, const FunctionInfo& info);

    // Get function info
    FunctionInfo getFunction(const std::string& name) const;

    // Check if function exists
    bool hasFunction(const std::string& name) const;

    // Unregister function
    bool unregisterFunction(const std::string& name);

    // Get all function names
    std::vector<std::string> getFunctionNames() const;

private:
    std::unordered_map<std::string, FunctionInfo> functions_;
    mutable std::shared_mutex mutex_;

    // Register default mathematical functions
    void registerDefaultFunctions();
};

} // namespace formula

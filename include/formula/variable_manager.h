#pragma once

#include "formula/value.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <shared_mutex>
#include <stdexcept>

namespace formula {

/**
 * Variable exception
 */
class VariableException : public std::runtime_error {
public:
    explicit VariableException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * Variable manager
 * Manages variables and their values used in formulas
 */
class VariableManager {
public:
    VariableManager() = default;

    // Set variable value (double version, for backward compatibility)
    void setVariable(const std::string& name, double value);

    // Set variable value (Value version)
    void setVariable(const std::string& name, const Value& value);

    // Get variable value, throws exception if not exists
    Value getVariable(const std::string& name) const;

    // Get variable value (double version, for backward compatibility)
    double getVariableAsDouble(const std::string& name) const;

    // Check if variable exists
    bool hasVariable(const std::string& name) const;

    // Remove variable
    bool removeVariable(const std::string& name);

    // Clear all variables
    void clear();

    // Get all variable names
    std::vector<std::string> getVariableNames() const;

    // Get variable count
    size_t getVariableCount() const;

private:
    std::unordered_map<std::string, Value> variables_;
    mutable std::shared_mutex mutex_;
};

} // namespace formula

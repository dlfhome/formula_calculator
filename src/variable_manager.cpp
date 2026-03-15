#include "formula/variable_manager.h"
#include <mutex>

namespace formula {

void VariableManager::setVariable(const std::string& name, double value) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    variables_[name] = Value(value);
}

void VariableManager::setVariable(const std::string& name, const Value& value) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    variables_[name] = value;
}

Value VariableManager::getVariable(const std::string& name) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    auto it = variables_.find(name);
    if (it == variables_.end()) {
        throw VariableException("Variable does not exist: " + name);
    }
    return it->second;
}

double VariableManager::getVariableAsDouble(const std::string& name) const {
    return getVariable(name).asDouble();
}

bool VariableManager::hasVariable(const std::string& name) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return variables_.find(name) != variables_.end();
}

bool VariableManager::removeVariable(const std::string& name) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    auto it = variables_.find(name);
    if (it == variables_.end()) {
        return false;
    }
    variables_.erase(it);
    return true;
}

void VariableManager::clear() {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    variables_.clear();
}

std::vector<std::string> VariableManager::getVariableNames() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    std::vector<std::string> names;
    names.reserve(variables_.size());
    for (const auto& pair : variables_) {
        names.push_back(pair.first);
    }
    return names;
}

size_t VariableManager::getVariableCount() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return variables_.size();
}

} // namespace formula

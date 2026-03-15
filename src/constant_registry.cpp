#include "formula/constant_registry.h"
#include <mutex>

namespace formula {

ConstantRegistry::ConstantRegistry() {
    registerDefaultConstants();
}

bool ConstantRegistry::registerConstant(const std::string& name, double value) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    if (constants_.find(name) != constants_.end()) {
        return false;
    }
    constants_[name] = value;
    return true;
}

double ConstantRegistry::getConstant(const std::string& name) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    auto it = constants_.find(name);
    if (it == constants_.end()) {
        throw ConstantException("常量不存在: " + name);
    }
    return it->second;
}

bool ConstantRegistry::hasConstant(const std::string& name) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return constants_.find(name) != constants_.end();
}

bool ConstantRegistry::unregisterConstant(const std::string& name) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    auto it = constants_.find(name);
    if (it == constants_.end()) {
        return false;
    }
    constants_.erase(it);
    return true;
}

std::vector<std::string> ConstantRegistry::getConstantNames() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    std::vector<std::string> names;
    names.reserve(constants_.size());
    for (const auto& pair : constants_) {
        names.push_back(pair.first);
    }
    return names;
}

void ConstantRegistry::registerDefaultConstants() {
    // 圆周率
    constants_["PI"] = 3.14159265358979323846;
    // 自然对数的底
    constants_["E"] = 2.71828182845904523536;
}

} // namespace formula

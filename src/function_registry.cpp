#include "formula/function_registry.h"
#include <algorithm>
#include <limits>
#include <mutex>
#include <cmath>

namespace formula {

FunctionRegistry::FunctionRegistry() {
    registerDefaultFunctions();
}

bool FunctionRegistry::registerFunction(const std::string& name, const FunctionInfo& info) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    if (functions_.find(name) != functions_.end()) {
        return false; // 函数已存在
    }
    functions_[name] = info;
    return true;
}

FunctionInfo FunctionRegistry::getFunction(const std::string& name) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    auto it = functions_.find(name);
    if (it == functions_.end()) {
        throw FunctionException("函数不存在: " + name);
    }
    return it->second;
}

bool FunctionRegistry::hasFunction(const std::string& name) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    return functions_.find(name) != functions_.end();
}

bool FunctionRegistry::unregisterFunction(const std::string& name) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    auto it = functions_.find(name);
    if (it == functions_.end()) {
        return false;
    }
    functions_.erase(it);
    return true;
}

std::vector<std::string> FunctionRegistry::getFunctionNames() const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    std::vector<std::string> names;
    names.reserve(functions_.size());
    for (const auto& pair : functions_) {
        names.push_back(pair.first);
    }
    return names;
}

void FunctionRegistry::registerDefaultFunctions() {
    // 三角函数
    functions_["sin"] = FunctionInfo([](const std::vector<double>& args) { return std::sin(args[0]); }, 1, "正弦");
    functions_["cos"] = FunctionInfo([](const std::vector<double>& args) { return std::cos(args[0]); }, 1, "余弦");
    functions_["tan"] = FunctionInfo([](const std::vector<double>& args) { return std::tan(args[0]); }, 1, "正切");
    functions_["asin"] = FunctionInfo([](const std::vector<double>& args) { return std::asin(args[0]); }, 1, "反正弦");
    functions_["acos"] = FunctionInfo([](const std::vector<double>& args) { return std::acos(args[0]); }, 1, "反余弦");
    functions_["atan"] = FunctionInfo([](const std::vector<double>& args) { return std::atan(args[0]); }, 1, "反正切");

    // 双曲函数
    functions_["sinh"] = FunctionInfo([](const std::vector<double>& args) { return std::sinh(args[0]); }, 1, "双曲正弦");
    functions_["cosh"] = FunctionInfo([](const std::vector<double>& args) { return std::cosh(args[0]); }, 1, "双曲余弦");
    functions_["tanh"] = FunctionInfo([](const std::vector<double>& args) { return std::tanh(args[0]); }, 1, "双曲正切");

    // 对数和指数
    functions_["log"] = FunctionInfo([](const std::vector<double>& args) { return std::log(args[0]); }, 1, "自然对数");
    functions_["log10"] = FunctionInfo([](const std::vector<double>& args) { return std::log10(args[0]); }, 1, "常用对数");
    functions_["exp"] = FunctionInfo([](const std::vector<double>& args) { return std::exp(args[0]); }, 1, "指数函数");

    // 其他数学函数
    functions_["sqrt"] = FunctionInfo([](const std::vector<double>& args) { return std::sqrt(args[0]); }, 1, "平方根");
    functions_["pow"] = FunctionInfo([](const std::vector<double>& args) { return std::pow(args[0], args[1]); }, 2, "幂函数");
    functions_["abs"] = FunctionInfo([](const std::vector<double>& args) { return std::abs(args[0]); }, 1, "绝对值");
    functions_["floor"] = FunctionInfo([](const std::vector<double>& args) { return std::floor(args[0]); }, 1, "向下取整");
    functions_["ceil"] = FunctionInfo([](const std::vector<double>& args) { return std::ceil(args[0]); }, 1, "向上取整");
    functions_["round"] = FunctionInfo([](const std::vector<double>& args) { return std::round(args[0]); }, 1, "四舍五入");

    // 可变参数函数
    functions_["min"] = FunctionInfo([](const std::vector<double>& args) {
        if (args.empty()) return std::numeric_limits<double>::quiet_NaN();
        return *std::min_element(args.begin(), args.end());
    }, -1, "最小值");

    functions_["max"] = FunctionInfo([](const std::vector<double>& args) {
        if (args.empty()) return std::numeric_limits<double>::quiet_NaN();
        return *std::max_element(args.begin(), args.end());
    }, -1, "最大值");
}

} // namespace formula

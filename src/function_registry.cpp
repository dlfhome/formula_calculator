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
        return false; // Function already exists
    }
    functions_[name] = info;
    return true;
}

FunctionInfo FunctionRegistry::getFunction(const std::string& name) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    auto it = functions_.find(name);
    if (it == functions_.end()) {
        throw FunctionException("Function does not exist: " + name);
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
    // Trigonometric functions
    functions_["sin"] = FunctionInfo([](const std::vector<double>& args) { return std::sin(args[0]); }, 1, "Sine");
    functions_["cos"] = FunctionInfo([](const std::vector<double>& args) { return std::cos(args[0]); }, 1, "Cosine");
    functions_["tan"] = FunctionInfo([](const std::vector<double>& args) { return std::tan(args[0]); }, 1, "Tangent");
    functions_["asin"] = FunctionInfo([](const std::vector<double>& args) { return std::asin(args[0]); }, 1, "Arcsine");
    functions_["acos"] = FunctionInfo([](const std::vector<double>& args) { return std::acos(args[0]); }, 1, "Arccosine");
    functions_["atan"] = FunctionInfo([](const std::vector<double>& args) { return std::atan(args[0]); }, 1, "Arctangent");

    // Hyperbolic functions
    functions_["sinh"] = FunctionInfo([](const std::vector<double>& args) { return std::sinh(args[0]); }, 1, "Hyperbolic sine");
    functions_["cosh"] = FunctionInfo([](const std::vector<double>& args) { return std::cosh(args[0]); }, 1, "Hyperbolic cosine");
    functions_["tanh"] = FunctionInfo([](const std::vector<double>& args) { return std::tanh(args[0]); }, 1, "Hyperbolic tangent");

    // Logarithmic and exponential functions
    functions_["log"] = FunctionInfo([](const std::vector<double>& args) { return std::log(args[0]); }, 1, "Natural logarithm");
    functions_["log10"] = FunctionInfo([](const std::vector<double>& args) { return std::log10(args[0]); }, 1, "Common logarithm");
    functions_["exp"] = FunctionInfo([](const std::vector<double>& args) { return std::exp(args[0]); }, 1, "Exponential function");

    // Other math functions
    functions_["sqrt"] = FunctionInfo([](const std::vector<double>& args) { return std::sqrt(args[0]); }, 1, "Square root");
    functions_["pow"] = FunctionInfo([](const std::vector<double>& args) { return std::pow(args[0], args[1]); }, 2, "Power function");
    functions_["abs"] = FunctionInfo([](const std::vector<double>& args) { return std::abs(args[0]); }, 1, "Absolute value");
    functions_["floor"] = FunctionInfo([](const std::vector<double>& args) { return std::floor(args[0]); }, 1, "Floor");
    functions_["ceil"] = FunctionInfo([](const std::vector<double>& args) { return std::ceil(args[0]); }, 1, "Ceiling");
    functions_["round"] = FunctionInfo([](const std::vector<double>& args) { return std::round(args[0]); }, 1, "Round");

    // Variadic functions
    functions_["min"] = FunctionInfo([](const std::vector<double>& args) {
        if (args.empty()) return std::numeric_limits<double>::quiet_NaN();
        return *std::min_element(args.begin(), args.end());
    }, -1, "Minimum value");

    functions_["max"] = FunctionInfo([](const std::vector<double>& args) {
        if (args.empty()) return std::numeric_limits<double>::quiet_NaN();
        return *std::max_element(args.begin(), args.end());
    }, -1, "Maximum value");
}

} // namespace formula

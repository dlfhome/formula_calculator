#include "formula/formula_calculator.h"
#include <iostream>
#include <cmath>

using namespace formula;

int main() {
    std::cout << "=== Formula Calculator Basic Usage Examples ===" << std::endl << std::endl;

    // Create calculator instance
    FormulaCalculator calc;

    // Example 1: Basic arithmetic operations
    std::cout << "[Example 1] Basic Arithmetic Operations" << std::endl;
    if (calc.compile("2 + 3 * 4")) {
        auto result = calc.evaluate();
        std::cout << "  2 + 3 * 4 = " << result << std::endl;
        std::cout << "  (Expected: 14)" << std::endl;
    }
    std::cout << std::endl;

    // Example 2: Using variables
    std::cout << "[Example 2] Using Variables" << std::endl;
    calc.clear();
    if (calc.compile("x * x + y * y")) {
        calc.setVariable("x", 3);
        calc.setVariable("y", 4);
        auto result = calc.evaluate();
        std::cout << "  x * x + y * y (x=3, y=4) = " << result << std::endl;
        std::cout << "  (Expected: 25)" << std::endl;
    }
    std::cout << std::endl;

    // Example 3: Using mathematical functions
    std::cout << "[Example 3] Using Mathematical Functions" << std::endl;
    calc.clear();
    if (calc.compile("sin(x) + cos(y)")) {
        calc.setVariable("x", 0.0);
        calc.setVariable("y", 0.0);
        auto result = calc.evaluate();
        std::cout << "  sin(x) + cos(y) (x=0, y=0) = " << result << std::endl;
        std::cout << "  (Expected: 1)" << std::endl;
    }
    std::cout << std::endl;

    // Example 4: Using constants
    std::cout << "[Example 4] Using Constants" << std::endl;
    calc.clear();
    if (calc.compile("2 * PI * r")) {
        calc.setVariable("r", 5.0);
        auto result = calc.evaluate();
        std::cout << "  2 * PI * r (r=5) = " << result << std::endl;
        std::cout << "  (Expected: " << 2 * 3.14159265358979323846 * 5 << ")" << std::endl;
    }
    std::cout << std::endl;

    // Example 5: Complex expression
    std::cout << "[Example 5] Complex Expression" << std::endl;
    calc.clear();
    if (calc.compile("sqrt(a^2 + b^2) + sin(PI/2)")) {
        calc.setVariable("a", 3.0);
        calc.setVariable("b", 4.0);
        auto result = calc.evaluate();
        std::cout << "  sqrt(a^2 + b^2) + sin(PI/2) (a=3, b=4) = " << result << std::endl;
        std::cout << "  (Expected: 6)" << std::endl;
    }
    std::cout << std::endl;

    // Example 6: Error handling
    std::cout << "[Example 6] Error Handling" << std::endl;
    calc.clear();
    if (!calc.compile("2 + * 3")) {
        std::cout << "  Compilation error: " << calc.getLastError() << std::endl;
    }
    std::cout << std::endl;

    // Example 7: Get supported functions list
    std::cout << "[Example 7] Supported Functions List" << std::endl;
    auto functions = calc.getSupportedFunctions();
    std::cout << "  Total " << functions.size() << " functions: ";
    for (size_t i = 0; i < functions.size() && i < 10; ++i) {
        std::cout << functions[i] << " ";
    }
    if (functions.size() > 10) {
        std::cout << "...";
    }
    std::cout << std::endl << std::endl;

    // Example 8: Get supported constants list
    std::cout << "[Example 8] Supported Constants List" << std::endl;
    auto constants = calc.getSupportedConstants();
    std::cout << "  Constants: ";
    for (const auto& c : constants) {
        std::cout << c << " ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "=== Examples Complete ===" << std::endl;
    return 0;
}

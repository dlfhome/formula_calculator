#include "formula/formula_calculator.h"
#include <iostream>
#include <cmath>

using namespace formula;

int main() {
    std::cout << "=== Formula Calculator Advanced Usage Examples ===" << std::endl << std::endl;

    FormulaCalculator calc;

    // Example 1: Custom function
    std::cout << "[Example 1] Custom Function" << std::endl;
    calc.registerFunction("custom_add", [](const std::vector<double>& args) {
        return args[0] + args[1] * 2;
    }, 2);

    if (calc.compile("custom_add(x, y)")) {
        calc.setVariable("x", 5.0);
        calc.setVariable("y", 3.0);
        auto result = calc.evaluate();
        std::cout << "  custom_add(5, 3) = 5 + 3*2 = " << result << std::endl;
    }
    std::cout << std::endl;

    // Example 2: Power operation
    std::cout << "[Example 2] Power Operation" << std::endl;
    calc.clear();
    if (calc.compile("2^10")) {
        auto result = calc.evaluate();
        std::cout << "  2^10 = " << result << std::endl;
        std::cout << "  (Expected: 1024)" << std::endl;
    }
    std::cout << std::endl;

    // Example 3: Modulo operation
    std::cout << "[Example 3] Modulo Operation" << std::endl;
    calc.clear();
    if (calc.compile("17 % 5")) {
        auto result = calc.evaluate();
        std::cout << "  17 % 5 = " << result << std::endl;
        std::cout << "  (Expected: 2)" << std::endl;
    }
    std::cout << std::endl;

    // Example 4: Complex mathematical formula
    std::cout << "[Example 4] Complex Mathematical Formula" << std::endl;
    calc.clear();
    if (calc.compile("sin(x)^2 + cos(x)^2")) {
        calc.setVariable("x", 1.5);
        auto result = calc.evaluate();
        std::cout << "  sin(x)^2 + cos(x)^2 (x=1.5) = " << result << std::endl;
        std::cout << "  (Expected approx: 1)" << std::endl;
    }
    std::cout << std::endl;

    // Example 5: Multi-parameter function
    std::cout << "[Example 5] Multi-parameter Functions min/max" << std::endl;
    calc.clear();
    if (calc.compile("max(a, b, c) - min(a, b, c)")) {
        calc.setVariable("a", 10.0);
        calc.setVariable("b", 5.0);
        calc.setVariable("c", 15.0);
        auto result = calc.evaluate();
        std::cout << "  max(10, 5, 15) - min(10, 5, 15) = " << result << std::endl;
        std::cout << "  (Expected: 10)" << std::endl;
    }
    std::cout << std::endl;

    // Example 6: Nested function calls
    std::cout << "[Example 6] Nested Function Calls" << std::endl;
    calc.clear();
    if (calc.compile("sqrt(abs(-25))")) {
        auto result = calc.evaluate();
        std::cout << "  sqrt(abs(-25)) = " << result << std::endl;
        std::cout << "  (Expected: 5)" << std::endl;
    }
    std::cout << std::endl;

    // Example 7: Circle area and circumference
    std::cout << "[Example 7] Circle Area and Circumference" << std::endl;
    calc.clear();
    if (calc.compile("PI * r^2")) {
        calc.setVariable("r", 5.0);
        auto area = calc.evaluate();
        std::cout << "  Circle area with radius 5 = " << area << std::endl;
    }
    calc.clear();
    if (calc.compile("2 * PI * r")) {
        calc.setVariable("r", 5.0);
        auto circumference = calc.evaluate();
        std::cout << "  Circle circumference with radius 5 = " << circumference << std::endl;
    }
    std::cout << std::endl;

    std::cout << "=== Examples Complete ===" << std::endl;
    return 0;
}

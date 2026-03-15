#include "formula/formula_calculator.h"
#include <iostream>
#include <chrono>
#include <cmath>

using namespace formula;
using namespace std::chrono;

void test_repeat_evaluation() {
    std::cout << "[Test 1] Repeat Evaluation Performance (AST Caching)" << std::endl;

    FormulaCalculator calc;
    if (!calc.compile("sin(x) * cos(y) + sqrt(x^2 + y^2)")) {
        std::cout << "Compilation failed: " << calc.getLastError() << std::endl;
        return;
    }

    calc.setVariable("x", 3.0);
    calc.setVariable("y", 4.0);

    const int iterations = 100000;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        volatile double result = calc.evaluate().asDouble();
        (void)result;
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    std::cout << "  Formula: sin(x) * cos(y) + sqrt(x^2 + y^2)" << std::endl;
    std::cout << "  Iterations: " << iterations << std::endl;
    std::cout << "  Total time: " << duration.count() << " ms" << std::endl;
    std::cout << "  Average per iteration: " << (double)duration.count() / iterations << " ms" << std::endl;
    std::cout << std::endl;
}

void test_complex_expression() {
    std::cout << "[Test 2] Complex Expression Performance" << std::endl;

    FormulaCalculator calc;
    std::string formula = "pow(sin(x), 2) + pow(cos(x), 2) + log(exp(y)) + sqrt(pow(a, 2) + pow(b, 2)) + max(min(x, y, z), a, b, c)";

    if (!calc.compile(formula)) {
        std::cout << "Compilation failed: " << calc.getLastError() << std::endl;
        return;
    }

    calc.setVariable("x", 1.0);
    calc.setVariable("y", 2.0);
    calc.setVariable("z", 3.0);
    calc.setVariable("a", 4.0);
    calc.setVariable("b", 5.0);
    calc.setVariable("c", 6.0);

    const int iterations = 10000;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        volatile double result = calc.evaluate().asDouble();
        (void)result;
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    std::cout << "  Formula: " << formula << std::endl;
    std::cout << "  Iterations: " << iterations << std::endl;
    std::cout << "  Total time: " << duration.count() << " ms" << std::endl;
    std::cout << "  Average per iteration: " << (double)duration.count() / iterations << " ms" << std::endl;
    std::cout << std::endl;
}

void test_compile_performance() {
    std::cout << "[Test 3] Compilation Performance" << std::endl;

    std::vector<std::string> formulas = {
        "x + y",
        "sin(x) + cos(y)",
        "sqrt(a^2 + b^2)",
        "log(x) + exp(y)",
        "max(a, b, c) - min(a, b, c)",
        "PI * r^2",
        "2 * PI * r",
        "pow(sin(x), 2) + pow(cos(x), 2)"
    };

    const int rounds = 1000;
    auto start = high_resolution_clock::now();

    for (int r = 0; r < rounds; ++r) {
        for (const auto& formula : formulas) {
            FormulaCalculator calc;
            if (!calc.compile(formula)) {
                std::cout << "Compilation failed: " << formula << std::endl;
            }
        }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    auto total_compiles = rounds * formulas.size();

    std::cout << "  Formula count: " << formulas.size() << std::endl;
    std::cout << "  Compilation rounds: " << rounds << std::endl;
    std::cout << "  Total compilations: " << total_compiles << std::endl;
    std::cout << "  Total time: " << duration.count() << " ms" << std::endl;
    std::cout << "  Average per compile: " << (double)duration.count() / total_compiles << " ms" << std::endl;
    std::cout << std::endl;
}

void test_accuracy() {
    std::cout << "[Test 4] Calculation Accuracy Verification" << std::endl;

    FormulaCalculator calc;

    // Test sin(PI/2) = 1
    calc.clear();
    if (calc.compile("sin(PI/2)")) {
        auto result = calc.evaluate().asDouble();
        std::cout << "  sin(PI/2) = " << result << " (Expected: 1, Error: " << std::abs(result - 1) << ")" << std::endl;
    }

    // Test cos(PI) = -1
    calc.clear();
    if (calc.compile("cos(PI)")) {
        auto result = calc.evaluate().asDouble();
        std::cout << "  cos(PI) = " << result << " (Expected: -1, Error: " << std::abs(result - (-1)) << ")" << std::endl;
    }

    // Test sqrt(2)^2 = 2
    calc.clear();
    if (calc.compile("sqrt(2)^2")) {
        auto result = calc.evaluate().asDouble();
        std::cout << "  sqrt(2)^2 = " << result << " (Expected: 2, Error: " << std::abs(result - 2) << ")" << std::endl;
    }

    // Test log(E) = 1
    calc.clear();
    if (calc.compile("log(E)")) {
        auto result = calc.evaluate().asDouble();
        std::cout << "  log(E) = " << result << " (Expected: 1, Error: " << std::abs(result - 1) << ")" << std::endl;
    }

    std::cout << std::endl;
}

int main() {
    std::cout << "=== Formula Calculator Performance Test ===" << std::endl << std::endl;

    test_repeat_evaluation();
    test_complex_expression();
    test_compile_performance();
    test_accuracy();

    std::cout << "=== Test Complete ===" << std::endl;
    return 0;
}

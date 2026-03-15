/**
 * New Feature Tests - Comparison, Logical, Ternary Operators
 */

#include "formula/formula_calculator.h"
#include <iostream>
#include <cmath>

using namespace formula;

bool test_comparison() {
    std::cout << "[Test] Comparison Operations" << std::endl;
    FormulaCalculator calc;
    bool all_passed = true;

    // 大于 >
    calc.compile("5 > 3");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: 5 > 3 should be true" << std::endl;
        all_passed = false;
    }

    // 小于 <
    calc.compile("3 < 5");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: 3 < 5 should be true" << std::endl;
        all_passed = false;
    }

    // 等于 ==
    calc.compile("5 == 5");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: 5 == 5 should be true" << std::endl;
        all_passed = false;
    }

    // 不等于 !=
    calc.compile("5 != 3");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: 5 != 3 should be true" << std::endl;
        all_passed = false;
    }

    // 大于等于 >=
    calc.compile("5 >= 5");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: 5 >= 5 should be true" << std::endl;
        all_passed = false;
    }

    // 小于等于 <=
    calc.compile("5 <= 5");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: 5 <= 5 should be true" << std::endl;
        all_passed = false;
    }

    // 范围检查
    calc.compile("x >= 0 && x <= 100");
    calc.setVariable("x", 50);
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: 50 should be within [0, 100] range" << std::endl;
        all_passed = false;
    }

    if (all_passed) {
        std::cout << "  All comparison tests passed!" << std::endl;
    }
    return all_passed;
}

bool test_logical() {
    std::cout << "[Test] Logical Operations" << std::endl;
    FormulaCalculator calc;
    bool all_passed = true;

    // 逻辑与 &&
    calc.compile("1 > 0 && 2 > 1");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: true && true should be true" << std::endl;
        all_passed = false;
    }

    calc.compile("1 > 0 && 1 > 2");
    if (calc.evaluate().asBool()) {
        std::cout << "  Failed: true && false should be false" << std::endl;
        all_passed = false;
    }

    // 逻辑或 ||
    calc.compile("1 > 0 || 1 > 2");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: true || false should be true" << std::endl;
        all_passed = false;
    }

    calc.compile("1 > 2 || 3 > 4");
    if (calc.evaluate().asBool()) {
        std::cout << "  Failed: false || false should be false" << std::endl;
        all_passed = false;
    }

    // 逻辑非 !
    calc.compile("!(1 > 2)");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: !false should be true" << std::endl;
        all_passed = false;
    }

    // 复合逻辑表达式
    calc.compile("(a > 0 && b > 0) || (a < 0 && b < 0)");
    calc.setVariable("a", 5);
    calc.setVariable("b", 3);
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: (5>0 && 3>0) || ... should be true" << std::endl;
        all_passed = false;
    }

    if (all_passed) {
        std::cout << "  All logical tests passed!" << std::endl;
    }
    return all_passed;
}

bool test_ternary() {
    std::cout << "[Test] Ternary Operator" << std::endl;
    FormulaCalculator calc;
    bool all_passed = true;

    // 基本三元运算
    calc.compile("5 > 3 ? 10 : 20");
    if (calc.evaluate().asDouble() != 10) {
        std::cout << "  Failed: true ? 10 : 20 should return 10" << std::endl;
        all_passed = false;
    }

    calc.compile("5 < 3 ? 10 : 20");
    if (calc.evaluate().asDouble() != 20) {
        std::cout << "  Failed: false ? 10 : 20 should return 20" << std::endl;
        all_passed = false;
    }

    // 绝对值: x >= 0 ? x : -x
    calc.compile("x >= 0 ? x : -x");
    calc.setVariable("x", -5);
    if (calc.evaluate().asDouble() != 5) {
        std::cout << "  Failed: abs(-5) should return 5" << std::endl;
        all_passed = false;
    }

    // 最大值: a > b ? a : b
    calc.compile("a > b ? a : b");
    calc.setVariable("a", 10);
    calc.setVariable("b", 7);
    if (calc.evaluate().asDouble() != 10) {
        std::cout << "  Failed: max(10, 7) should return 10" << std::endl;
        all_passed = false;
    }

    // 嵌套三元
    calc.compile("score >= 90 ? 1 : (score >= 60 ? 2 : 3)");
    calc.setVariable("score", 85);
    if (calc.evaluate().asDouble() != 2) {
        std::cout << "  Failed: Score 85 should return 2" << std::endl;
        all_passed = false;
    }

    if (all_passed) {
        std::cout << "  All ternary tests passed!" << std::endl;
    }
    return all_passed;
}

bool test_precedence() {
    std::cout << "[Test] Operator Precedence" << std::endl;
    FormulaCalculator calc;
    bool all_passed = true;

    // 算术 > 比较 > 逻辑
    // 1 + 2 > 3 - 1 should parse as (1+2) > (3-1) = 3 > 2 = true
    calc.compile("1 + 2 > 3 - 1");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: 1+2 > 3-1 (=3>2) should be true" << std::endl;
        all_passed = false;
    }

    // 比较 > 逻辑与
    // 5 > 3 && 2 < 4 should parse as (5>3) && (2<4) = true && true = true
    calc.compile("5 > 3 && 2 < 4");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: 5>3 && 2<4 should be true" << std::endl;
        all_passed = false;
    }

    // 逻辑与 > 逻辑或
    // true || false && false should parse as true || (false && false) = true
    calc.compile("1 > 0 || 1 > 2 && 3 > 4");
    if (!calc.evaluate().asBool()) {
        std::cout << "  Failed: true || false && false should be true" << std::endl;
        all_passed = false;
    }

    if (all_passed) {
        std::cout << "  All precedence tests passed!" << std::endl;
    }
    return all_passed;
}

bool test_value_types() {
    std::cout << "[Test] Value Type System" << std::endl;
    bool all_passed = true;

    // bool 类型
    Value v_bool(true);
    if (!v_bool.isBool() || !v_bool.asBool()) {
        std::cout << "  Failed: bool type test" << std::endl;
        all_passed = false;
    }

    // int 类型
    Value v_int(42);
    if (!v_int.isInt() || v_int.asInt() != 42) {
        std::cout << "  Failed: int type test" << std::endl;
        all_passed = false;
    }

    // double 类型
    Value v_double(3.14);
    if (!v_double.isDouble() || std::abs(v_double.asDouble() - 3.14) > 1e-10) {
        std::cout << "  Failed: double type test" << std::endl;
        all_passed = false;
    }

    // 类型转换
    Value v_int2(10);
    if (std::abs(v_int2.asDouble() - 10.0) > 1e-10) {
        std::cout << "  失败: int 转 double Test" << std::endl;
        all_passed = false;
    }

    // bool 转数值
    Value v_true(true);
    if (v_true.asInt() != 1 || v_true.asDouble() != 1.0) {
        std::cout << "  失败: bool 转数值Test" << std::endl;
        all_passed = false;
    }

    if (all_passed) {
        std::cout << "  All Value type tests passed!" << std::endl;
    }
    return all_passed;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Formula Calculator New Feature Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    bool all_passed = true;

    all_passed &= test_value_types();
    std::cout << std::endl;

    all_passed &= test_comparison();
    std::cout << std::endl;

    all_passed &= test_logical();
    std::cout << std::endl;

    all_passed &= test_ternary();
    std::cout << std::endl;

    all_passed &= test_precedence();
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    if (all_passed) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << "Some tests failed!" << std::endl;
    }
    std::cout << "========================================" << std::endl;

    return all_passed ? 0 : 1;
}

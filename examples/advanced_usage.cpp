#include "formula/formula_calculator.h"
#include <iostream>
#include <cmath>

using namespace formula;

int main() {
    std::cout << "=== 公式计算器高级使用示例 ===" << std::endl << std::endl;

    FormulaCalculator calc;

    // 示例 1: 自定义函数
    std::cout << "【示例 1】自定义函数" << std::endl;
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

    // 示例 2: 幂运算
    std::cout << "【示例 2】幂运算" << std::endl;
    calc.clear();
    if (calc.compile("2^10")) {
        auto result = calc.evaluate();
        std::cout << "  2^10 = " << result << std::endl;
        std::cout << "  （期望: 1024）" << std::endl;
    }
    std::cout << std::endl;

    // 示例 3: 取模运算
    std::cout << "【示例 3】取模运算" << std::endl;
    calc.clear();
    if (calc.compile("17 % 5")) {
        auto result = calc.evaluate();
        std::cout << "  17 % 5 = " << result << std::endl;
        std::cout << "  （期望: 2）" << std::endl;
    }
    std::cout << std::endl;

    // 示例 4: 复杂数学公式
    std::cout << "【示例 4】复杂数学公式" << std::endl;
    calc.clear();
    if (calc.compile("sin(x)^2 + cos(x)^2")) {
        calc.setVariable("x", 1.5);
        auto result = calc.evaluate();
        std::cout << "  sin(x)^2 + cos(x)^2 (x=1.5) = " << result << std::endl;
        std::cout << "  （期望约: 1）" << std::endl;
    }
    std::cout << std::endl;

    // 示例 5: 多参数函数
    std::cout << "【示例 5】多参数函数 min/max" << std::endl;
    calc.clear();
    if (calc.compile("max(a, b, c) - min(a, b, c)")) {
        calc.setVariable("a", 10.0);
        calc.setVariable("b", 5.0);
        calc.setVariable("c", 15.0);
        auto result = calc.evaluate();
        std::cout << "  max(10, 5, 15) - min(10, 5, 15) = " << result << std::endl;
        std::cout << "  （期望: 10）" << std::endl;
    }
    std::cout << std::endl;

    // 示例 6: 嵌套函数调用
    std::cout << "【示例 6】嵌套函数调用" << std::endl;
    calc.clear();
    if (calc.compile("sqrt(abs(-25))")) {
        auto result = calc.evaluate();
        std::cout << "  sqrt(abs(-25)) = " << result << std::endl;
        std::cout << "  （期望: 5）" << std::endl;
    }
    std::cout << std::endl;

    // 示例 7: 圆的面积和周长
    std::cout << "【示例 7】圆的面积和周长" << std::endl;
    calc.clear();
    if (calc.compile("PI * r^2")) {
        calc.setVariable("r", 5.0);
        auto area = calc.evaluate();
        std::cout << "  半径 5 的圆面积 = " << area << std::endl;
    }
    calc.clear();
    if (calc.compile("2 * PI * r")) {
        calc.setVariable("r", 5.0);
        auto circumference = calc.evaluate();
        std::cout << "  半径 5 的圆周长 = " << circumference << std::endl;
    }
    std::cout << std::endl;

    std::cout << "=== 示例结束 ===" << std::endl;
    return 0;
}

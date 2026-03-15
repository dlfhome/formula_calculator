#include "formula/formula_calculator.h"
#include <iostream>
#include <cmath>

using namespace formula;

int main() {
    std::cout << "=== 公式计算器基础使用示例 ===" << std::endl << std::endl;

    // 创建计算器实例
    FormulaCalculator calc;

    // 示例 1: 基本算术运算
    std::cout << "【示例 1】基本算术运算" << std::endl;
    if (calc.compile("2 + 3 * 4")) {
        auto result = calc.evaluate();
        std::cout << "  2 + 3 * 4 = " << result << std::endl;
        std::cout << "  （期望: 14）" << std::endl;
    }
    std::cout << std::endl;

    // 示例 2: 使用变量
    std::cout << "【示例 2】使用变量" << std::endl;
    calc.clear();
    if (calc.compile("x * x + y * y")) {
        calc.setVariable("x", 3);
        calc.setVariable("y", 4);
        auto result = calc.evaluate();
        std::cout << "  x * x + y * y (x=3, y=4) = " << result << std::endl;
        std::cout << "  （期望: 25）" << std::endl;
    }
    std::cout << std::endl;

    // 示例 3: 使用数学函数
    std::cout << "【示例 3】使用数学函数" << std::endl;
    calc.clear();
    if (calc.compile("sin(x) + cos(y)")) {
        calc.setVariable("x", 0.0);
        calc.setVariable("y", 0.0);
        auto result = calc.evaluate();
        std::cout << "  sin(x) + cos(y) (x=0, y=0) = " << result << std::endl;
        std::cout << "  （期望: 1）" << std::endl;
    }
    std::cout << std::endl;

    // 示例 4: 使用常量
    std::cout << "【示例 4】使用常量" << std::endl;
    calc.clear();
    if (calc.compile("2 * PI * r")) {
        calc.setVariable("r", 5.0);
        auto result = calc.evaluate();
        std::cout << "  2 * PI * r (r=5) = " << result << std::endl;
        std::cout << "  （期望: " << 2 * 3.14159265358979323846 * 5 << ")" << std::endl;
    }
    std::cout << std::endl;

    // 示例 5: 复杂表达式
    std::cout << "【示例 5】复杂表达式" << std::endl;
    calc.clear();
    if (calc.compile("sqrt(a^2 + b^2) + sin(PI/2)")) {
        calc.setVariable("a", 3.0);
        calc.setVariable("b", 4.0);
        auto result = calc.evaluate();
        std::cout << "  sqrt(a^2 + b^2) + sin(PI/2) (a=3, b=4) = " << result << std::endl;
        std::cout << "  （期望: 6）" << std::endl;
    }
    std::cout << std::endl;

    // 示例 6: 错误处理
    std::cout << "【示例 6】错误处理" << std::endl;
    calc.clear();
    if (!calc.compile("2 + * 3")) {
        std::cout << "  编译错误: " << calc.getLastError() << std::endl;
    }
    std::cout << std::endl;

    // 示例 7: 获取支持的函数列表
    std::cout << "【示例 7】支持的函数列表" << std::endl;
    auto functions = calc.getSupportedFunctions();
    std::cout << "  共 " << functions.size() << " 个函数: ";
    for (size_t i = 0; i < functions.size() && i < 10; ++i) {
        std::cout << functions[i] << " ";
    }
    if (functions.size() > 10) {
        std::cout << "...";
    }
    std::cout << std::endl << std::endl;

    // 示例 8: 获取支持的常量列表
    std::cout << "【示例 8】支持的常量列表" << std::endl;
    auto constants = calc.getSupportedConstants();
    std::cout << "  常量: ";
    for (const auto& c : constants) {
        std::cout << c << " ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "=== 示例结束 ===" << std::endl;
    return 0;
}

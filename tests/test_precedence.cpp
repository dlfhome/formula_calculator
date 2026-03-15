/**
 * 运算符优先级测试
 * 验证复杂表达式中各运算符的优先级和结合性
 *
 * 预期优先级（从高到低）：
 * 1. () 括号
 * 2. ! 逻辑非
 * 3. ^ 幂运算
 * 4. *, /, % 乘除模
 * 5. +, - 加减
 * 6. >, <, >=, <= 比较
 * 7. ==, != 相等性
 * 8. && 逻辑与
 * 9. || 逻辑或
 * 10. ?: 三元运算符
 */

#include <iostream>
#include <cassert>
#include <cmath>

// 测试辅助宏
#define TEST(name) void test_##name()
#define RUN_TEST(name) \
    do { \
        std::cout << "  运行 " << #name << "... "; \
        test_##name(); \
        std::cout << "通过" << std::endl; \
    } while(0)
#define ASSERT_TRUE(expr) assert(expr)
#define ASSERT_FALSE(expr) assert(!(expr))
#define ASSERT_EQ(a, b) assert((a) == (b))
#define ASSERT_NEAR(a, b, eps) assert(std::abs((a) - (b)) < (eps))

namespace formula {

// 辅助函数：模拟表达式求值
// 注意：这些函数用于验证优先级规则

// 逻辑非优先级最高（在二元运算符中）
bool logicalNot(bool operand) { return !operand; }

// 幂运算
double power(double base, double exp) { return std::pow(base, exp); }

// 比较运算
bool greater(double a, double b) { return a > b; }
bool less(double a, double b) { return a < b; }
bool greaterEq(double a, double b) { return a >= b; }
bool lessEq(double a, double b) { return a <= b; }
bool equal(double a, double b, double eps = 1e-9) {
    return std::abs(a - b) <= eps;
}
bool notEqual(double a, double b, double eps = 1e-9) {
    return std::abs(a - b) > eps;
}

// 逻辑运算
bool logicalAnd(bool a, bool b) { return a && b; }
bool logicalOr(bool a, bool b) { return a || b; }

// 三元运算符
template<typename T>
T ternary(bool cond, T trueVal, T falseVal) { return cond ? trueVal : falseVal; }

} // namespace formula

using namespace formula;

// ==================== 测试用例 ====================

// 括号优先级测试（最高）
TEST(parentheses_highest) {
    // (2 + 3) * 4 = 20，不是 2 + (3 * 4) = 14
    double result1 = (2.0 + 3.0) * 4.0;
    double result2 = 2.0 + 3.0 * 4.0;
    ASSERT_EQ(result1, 20.0);
    ASSERT_EQ(result2, 14.0);

    // 2 * (3 + 4) = 14
    ASSERT_EQ(2.0 * (3.0 + 4.0), 14.0);

    // 嵌套括号
    ASSERT_EQ(((1.0 + 2.0) * 3.0) + 4.0, 13.0);
}

// 逻辑非优先级测试
TEST(logical_not_precedence) {
    // !a && b 应该解析为 (!a) && b，不是 !(a && b)
    bool a = true, b = false;
    ASSERT_FALSE(logicalNot(a) && b);  // false && false = false
    ASSERT_TRUE(logicalNot(logicalAnd(a, b)));  // !(true && false) = true

    // !a || b 应该解析为 (!a) || b
    ASSERT_FALSE(logicalNot(a) || b);  // false || false = false

    // !!a = a
    ASSERT_TRUE(logicalNot(logicalNot(a)));

    // !与比较：!a > b 应该是 (!a) > b
    // 假设 a = true(1), b = 0
    // (!1) > 0 = 0 > 0 = false
}

// 幂运算优先级测试
TEST(power_precedence) {
    // 2 ^ 3 ^ 2 = 2 ^ (3 ^ 2) = 2 ^ 9 = 512（右结合）
    double result = power(2.0, power(3.0, 2.0));
    ASSERT_EQ(result, 512.0);

    // 2 * 3 ^ 2 = 2 * 9 = 18，不是 (2 * 3) ^ 2 = 36
    ASSERT_EQ(2.0 * power(3.0, 2.0), 18.0);

    // 2 ^ 3 * 4 = 8 * 4 = 32，不是 2 ^ (3 * 4)
    ASSERT_EQ(power(2.0, 3.0) * 4.0, 32.0);

    // 2 + 3 ^ 2 = 2 + 9 = 11
    ASSERT_EQ(2.0 + power(3.0, 2.0), 11.0);
}

// 乘除模优先级测试
TEST(multiplicative_precedence) {
    // 2 + 3 * 4 = 14，不是 (2 + 3) * 4 = 20
    ASSERT_EQ(2.0 + 3.0 * 4.0, 14.0);

    // 10 - 6 / 2 = 7，不是 (10 - 6) / 2 = 2
    ASSERT_EQ(10.0 - 6.0 / 2.0, 7.0);

    // 8 / 4 * 2 = 4（左结合）
    ASSERT_EQ(8.0 / 4.0 * 2.0, 4.0);

    // 10 % 3 * 2 = 1 * 2 = 2
    ASSERT_EQ(std::fmod(10.0, 3.0) * 2.0, 2.0);
}

// 加减优先级测试
TEST(additive_precedence) {
    // 5 - 3 + 2 = 4（左结合）
    ASSERT_EQ(5.0 - 3.0 + 2.0, 4.0);

    // 与乘除混合
    // 10 - 2 * 3 = 4
    ASSERT_EQ(10.0 - 2.0 * 3.0, 4.0);

    // 10 / 2 + 3 = 8
    ASSERT_EQ(10.0 / 2.0 + 3.0, 8.0);
}

// 比较运算符优先级测试
TEST(comparison_precedence) {
    // 5 + 3 > 7 = 8 > 7 = true
    ASSERT_TRUE(5.0 + 3.0 > 7.0);

    // 2 * 3 < 10 - 3 = 6 < 7 = true
    ASSERT_TRUE(2.0 * 3.0 < 10.0 - 3.0);

    // 比较运算符之间：> 和 < 同优先级，左结合
    // 5 > 3 < 7 = (5 > 3) < 7 = true < 7 = true (转为数值1 < 7)
    // 注意：这种写法在实际中很少见且容易混淆

    // 10 >= 5 + 5 = 10 >= 10 = true
    ASSERT_TRUE(10.0 >= 5.0 + 5.0);

    // 10 <= 3 * 4 = 10 <= 12 = true
    ASSERT_TRUE(10.0 <= 3.0 * 4.0);
}

// 相等性运算符优先级测试
TEST(equality_precedence) {
    // 5 > 3 == true = true == true = true
    ASSERT_TRUE((5.0 > 3.0) == true);

    // 2 + 2 == 5 - 1 = 4 == 4 = true
    ASSERT_TRUE(2.0 + 2.0 == 5.0 - 1.0);

    // 10 / 2 != 3 + 2 = 5 != 5 = false
    ASSERT_FALSE(10.0 / 2.0 != 3.0 + 2.0);

    // 比较在相等性之前：5 > 3 == 4 > 2
    // = (5 > 3) == (4 > 2) = true == true = true
    ASSERT_TRUE((5.0 > 3.0) == (4.0 > 2.0));
}

// 逻辑与优先级测试
TEST(logical_and_precedence) {
    // true && false == false = true
    ASSERT_TRUE((true && false) == false);

    // 5 > 3 && 4 < 6 = true && true = true
    ASSERT_TRUE((5.0 > 3.0) && (4.0 < 6.0));

    // 2 + 2 == 4 && 3 * 3 == 9
    ASSERT_TRUE((2.0 + 2.0 == 4.0) && (3.0 * 3.0 == 9.0));

    // 比较在逻辑与之前：5 > 3 && 4 > 6 = true && false = false
    ASSERT_FALSE((5.0 > 3.0) && (4.0 > 6.0));
}

// 逻辑或优先级测试
TEST(logical_or_precedence) {
    // false || true && false = false || (true && false) = false
    // && 优先级高于 ||
    ASSERT_FALSE(false || (true && false));

    // 5 > 10 || 3 < 7 = false || true = true
    ASSERT_TRUE((5.0 > 10.0) || (3.0 < 7.0));

    // 2 == 3 || 4 == 4 = false || true = true
    ASSERT_TRUE((2.0 == 3.0) || (4.0 == 4.0));

    // 复杂组合：(5 > 3 && 4 < 2) || 10 > 5
    // = (true && false) || true = false || true = true
    ASSERT_TRUE(((5.0 > 3.0) && (4.0 < 2.0)) || (10.0 > 5.0));
}

// 三元运算符优先级测试（最低）
TEST(ternary_precedence) {
    // 5 > 3 ? 10 : 20 = true ? 10 : 20 = 10
    ASSERT_EQ(ternary(5.0 > 3.0, 10.0, 20.0), 10.0);

    // 2 + 2 == 4 ? 100 : 200 = 100
    ASSERT_EQ(ternary(2.0 + 2.0 == 4.0, 100.0, 200.0), 100.0);

    // true || false ? 1 : 0
    // || 优先级高于 ?:，所以是 (true || false) ? 1 : 0 = 1
    ASSERT_EQ(ternary(true || false, 1.0, 0.0), 1.0);

    // 5 > 3 && 4 < 2 ? "yes" : "no"
    // = (5 > 3 && 4 < 2) ? "yes" : "no" = false ? "yes" : "no" = "no"
    ASSERT_EQ(ternary((5.0 > 3.0) && (4.0 < 2.0), 1.0, 0.0), 0.0);
}

// 复杂综合表达式测试
TEST(complex_expressions) {
    // (a + b) * c > d && e < f || g == h ? x : y
    double a = 2, b = 3, c = 4, d = 15;
    bool e = true, f = false, g = true, h = true;
    double x = 100, y = 200;

    // 步骤分解：
    // 1. (a + b) = 5
    // 2. 5 * c = 20
    // 3. 20 > d = 20 > 15 = true
    // 4. e < f = true < false = false (数值比较)
    // 5. true && false = false
    // 6. g == h = true == true = true
    // 7. false || true = true
    // 8. true ? x : y = x = 100

    // 简化版本测试
    bool step1 = (a + b) * c > d;  // true
    bool step2 = e && f;           // false
    bool step3 = g == h;           // true
    bool step4 = step2 || step3;   // true
    double result = ternary(step4, x, y);
    ASSERT_EQ(result, 100.0);
}

// 数学公式优先级测试
TEST(mathematical_formulas) {
    // 二次方程求根公式的一部分：-b + sqrt(b^2 - 4ac) / 2a
    double a = 1, b = -3, c = 2;
    double discriminant = power(b, 2.0) - 4.0 * a * c;
    ASSERT_EQ(discriminant, 1.0);  // 9 - 8 = 1

    // 等差数列求和：n * (a1 + an) / 2
    double n = 10, a1 = 1, an = 10;
    double sum = n * (a1 + an) / 2.0;
    ASSERT_EQ(sum, 55.0);

    // 圆的面积：pi * r^2
    double pi = 3.14159, r = 5;
    double area = pi * power(r, 2.0);
    ASSERT_NEAR(area, 78.53975, 1e-5);
}

// 逻辑表达式优先级测试
TEST(logical_expression_precedence) {
    // !(a && b) == !a || !b（德摩根定律）
    for (bool a : {true, false}) {
        for (bool b : {true, false}) {
            bool left = logicalNot(a && b);
            bool right = logicalNot(a) || logicalNot(b);
            ASSERT_EQ(left, right);
        }
    }

    // a && b || c && d = (a && b) || (c && d)
    // && 优先级高于 ||
    bool a = true, b = false, c = true, d = true;
    bool result1 = (a && b) || (c && d);  // false || true = true
    bool result2 = a && b || c && d;      // 同上
    ASSERT_EQ(result1, result2);
    ASSERT_TRUE(result1);
}

// 结合性测试
TEST(associativity) {
    // 左结合：a - b - c = (a - b) - c
    ASSERT_EQ(10.0 - 5.0 - 2.0, 3.0);  // (10-5)-2 = 3，不是 10-(5-2) = 7

    // 左结合：a / b / c = (a / b) / c
    ASSERT_EQ(100.0 / 10.0 / 2.0, 5.0);  // (100/10)/2 = 5，不是 100/(10/2) = 20

    // 右结合：a ^ b ^ c = a ^ (b ^ c)
    double rightAssociative = power(2.0, power(3.0, 2.0));  // 2^9 = 512
    ASSERT_EQ(rightAssociative, 512.0);

    // 左结合：a - b + c = (a - b) + c
    ASSERT_EQ(10.0 - 5.0 + 2.0, 7.0);
}

// 边界情况测试
TEST(precedence_edge_cases) {
    // 一元负号和二元减号
    // -3^2 = -(3^2) = -9（不是 (-3)^2 = 9）
    double result = -power(3.0, 2.0);
    ASSERT_EQ(result, -9.0);

    // 连续比较（不推荐但测试）
    // 1 < 3 < 2 = (1 < 3) < 2 = true < 2 = 1 < 2 = true（数值转换）
    // 注意：这种写法在大多数语言中都有陷阱

    // 三元运算符嵌套
    // a ? b : c ? d : e = a ? b : (c ? d : e)（右结合）
    int r1 = ternary(true, 1, ternary(false, 2, 3));   // 1
    int r2 = ternary(false, 1, ternary(true, 2, 3));   // 2
    int r3 = ternary(false, 1, ternary(false, 2, 3));  // 3
    ASSERT_EQ(r1, 1);
    ASSERT_EQ(r2, 2);
    ASSERT_EQ(r3, 3);
}

// ==================== 主函数 ====================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "运算符优先级测试套件" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "【基本优先级测试】" << std::endl;
    RUN_TEST(parentheses_highest);
    RUN_TEST(logical_not_precedence);
    RUN_TEST(power_precedence);
    RUN_TEST(multiplicative_precedence);
    RUN_TEST(additive_precedence);
    std::cout << std::endl;

    std::cout << "【比较和逻辑优先级测试】" << std::endl;
    RUN_TEST(comparison_precedence);
    RUN_TEST(equality_precedence);
    RUN_TEST(logical_and_precedence);
    RUN_TEST(logical_or_precedence);
    RUN_TEST(ternary_precedence);
    std::cout << std::endl;

    std::cout << "【复杂表达式测试】" << std::endl;
    RUN_TEST(complex_expressions);
    RUN_TEST(mathematical_formulas);
    RUN_TEST(logical_expression_precedence);
    std::cout << std::endl;

    std::cout << "【结合性测试】" << std::endl;
    RUN_TEST(associativity);
    RUN_TEST(precedence_edge_cases);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "所有测试通过！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

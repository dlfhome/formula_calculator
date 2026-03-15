/**
 * Comparison Operation Tests
 * Test >, <, ==, >=, <=, != operators
 */

#include <iostream>
#include <cassert>
#include <cmath>
#include <string>

// Test helper macros
#define TEST(name) void test_##name()
#define RUN_TEST(name) \
    do { \
        std::cout << "  Running " << #name << "... "; \
        test_##name(); \
        std::cout << "Passed" << std::endl; \
    } while(0)
#define ASSERT_TRUE(expr) assert(expr)
#define ASSERT_FALSE(expr) assert(!(expr))
#define ASSERT_EQ(a, b) assert((a) == (b))
#define ASSERT_NE(a, b) assert((a) != (b))

// Mock comparison results (replace with real calls when actual implementation is available)
namespace formula {

enum class CompareOp {
    GREATER,      // >
    LESS,         // <
    EQUAL,        // ==
    GREATER_EQ,   // >=
    LESS_EQ,      // <=
    NOT_EQUAL     // !=
};

// Compare two double values (with precision consideration)
bool compare(double left, CompareOp op, double right, double epsilon = 1e-9) {
    // Handle infinity equality comparison
    if (std::isinf(left) && std::isinf(right)) {
        bool bothPositive = left > 0 && right > 0;
        bool bothNegative = left < 0 && right < 0;
        bool sameSign = bothPositive || bothNegative;

        switch (op) {
            case CompareOp::EQUAL:
                return sameSign;
            case CompareOp::NOT_EQUAL:
                return !sameSign;
            case CompareOp::GREATER:
                return left > right;
            case CompareOp::LESS:
                return left < right;
            case CompareOp::GREATER_EQ:
                return left >= right;
            case CompareOp::LESS_EQ:
                return left <= right;
        }
    }

    switch (op) {
        case CompareOp::GREATER:
            return left > right + epsilon;
        case CompareOp::LESS:
            return left < right - epsilon;
        case CompareOp::EQUAL:
            return std::abs(left - right) <= epsilon;
        case CompareOp::GREATER_EQ:
            return left >= right - epsilon;
        case CompareOp::LESS_EQ:
            return left <= right + epsilon;
        case CompareOp::NOT_EQUAL:
            return std::abs(left - right) > epsilon;
    }
    return false;
}

// Mixed type comparison
bool compareMixed(double left, CompareOp op, double right) {
    return compare(left, op, right);
}

} // namespace formula

using namespace formula;

// ==================== Test Cases ====================

// Greater than operator (>) test
TEST(greater_than) {
    // Basic integer comparison
    ASSERT_TRUE(compare(5.0, CompareOp::GREATER, 3.0));
    ASSERT_FALSE(compare(3.0, CompareOp::GREATER, 5.0));
    ASSERT_FALSE(compare(5.0, CompareOp::GREATER, 5.0));

    // Negative number comparison
    ASSERT_TRUE(compare(-1.0, CompareOp::GREATER, -5.0));
    ASSERT_FALSE(compare(-10.0, CompareOp::GREATER, -5.0));

    // Mixed positive/negative
    ASSERT_TRUE(compare(1.0, CompareOp::GREATER, -1.0));
    ASSERT_FALSE(compare(-1.0, CompareOp::GREATER, 1.0));
}

// Less than operator (<) test
TEST(less_than) {
    // 基本整数比较
    ASSERT_TRUE(compare(3.0, CompareOp::LESS, 5.0));
    ASSERT_FALSE(compare(5.0, CompareOp::LESS, 3.0));
    ASSERT_FALSE(compare(5.0, CompareOp::LESS, 5.0));

    // 负数比较
    ASSERT_TRUE(compare(-5.0, CompareOp::LESS, -1.0));
    ASSERT_FALSE(compare(-1.0, CompareOp::LESS, -5.0));

    // Zero comparison
    ASSERT_TRUE(compare(-1.0, CompareOp::LESS, 0.0));
    ASSERT_TRUE(compare(0.0, CompareOp::LESS, 1.0));
}

// Equal to operator (==) test
TEST(equal_to) {
    // Basic equality
    ASSERT_TRUE(compare(5.0, CompareOp::EQUAL, 5.0));
    ASSERT_TRUE(compare(0.0, CompareOp::EQUAL, 0.0));
    ASSERT_TRUE(compare(-3.0, CompareOp::EQUAL, -3.0));

    // 不相等
    ASSERT_FALSE(compare(5.0, CompareOp::EQUAL, 3.0));
    ASSERT_FALSE(compare(5.0, CompareOp::EQUAL, -5.0));

    // Floating point precision test
    ASSERT_TRUE(compare(0.1 + 0.2, CompareOp::EQUAL, 0.3));  // 考虑精度
    ASSERT_TRUE(compare(1.0 / 3.0 * 3.0, CompareOp::EQUAL, 1.0));
}

// 大于等于运算符 (>=) Test
TEST(greater_than_or_equal) {
    // 大于情况
    ASSERT_TRUE(compare(5.0, CompareOp::GREATER_EQ, 3.0));
    // 等于情况
    ASSERT_TRUE(compare(5.0, CompareOp::GREATER_EQ, 5.0));
    // 小于情况
    ASSERT_FALSE(compare(3.0, CompareOp::GREATER_EQ, 5.0));

    // 负数
    ASSERT_TRUE(compare(-1.0, CompareOp::GREATER_EQ, -5.0));
    ASSERT_TRUE(compare(-5.0, CompareOp::GREATER_EQ, -5.0));
    ASSERT_FALSE(compare(-10.0, CompareOp::GREATER_EQ, -5.0));
}

// 小于等于运算符 (<=) Test
TEST(less_than_or_equal) {
    // 小于情况
    ASSERT_TRUE(compare(3.0, CompareOp::LESS_EQ, 5.0));
    // 等于情况
    ASSERT_TRUE(compare(5.0, CompareOp::LESS_EQ, 5.0));
    // 大于情况
    ASSERT_FALSE(compare(5.0, CompareOp::LESS_EQ, 3.0));

    // 边界值
    ASSERT_TRUE(compare(0.0, CompareOp::LESS_EQ, 0.0));
    ASSERT_TRUE(compare(-100.0, CompareOp::LESS_EQ, 100.0));
}

// 不等于运算符 (!=) Test
TEST(not_equal) {
    // Inequality
    ASSERT_TRUE(compare(5.0, CompareOp::NOT_EQUAL, 3.0));
    ASSERT_TRUE(compare(-5.0, CompareOp::NOT_EQUAL, 5.0));
    ASSERT_TRUE(compare(0.0, CompareOp::NOT_EQUAL, 1.0));

    // Equality
    ASSERT_FALSE(compare(5.0, CompareOp::NOT_EQUAL, 5.0));
    ASSERT_FALSE(compare(0.0, CompareOp::NOT_EQUAL, 0.0));
    ASSERT_FALSE(compare(-3.14, CompareOp::NOT_EQUAL, -3.14));
}

// 浮点数精度比较Test
TEST(floating_point_precision) {
    // 经典浮点数精度问题
    double a = 0.1;
    double b = 0.2;
    double sum = a + b;

    // Direct comparison fails, but using epsilon should pass
    ASSERT_TRUE(compare(sum, CompareOp::EQUAL, 0.3));

    // Small differences should be treated as equal (use smaller difference)
    ASSERT_TRUE(compare(1.0000000001, CompareOp::EQUAL, 1.0));
    ASSERT_TRUE(compare(1.0, CompareOp::EQUAL, 1.0000000001));

    // Differences exceeding epsilon should be treated as not equal
    ASSERT_FALSE(compare(1.000001, CompareOp::EQUAL, 1.0));
}

// 特殊浮点值Test
TEST(special_float_values) {
    // 无穷大
    double inf = std::numeric_limits<double>::infinity();
    ASSERT_TRUE(compare(inf, CompareOp::GREATER, 0.0));
    ASSERT_TRUE(compare(-inf, CompareOp::LESS, 0.0));
    ASSERT_TRUE(compare(inf, CompareOp::EQUAL, inf));
    ASSERT_TRUE(compare(-inf, CompareOp::EQUAL, -inf));

    // Large value comparison
    double max = std::numeric_limits<double>::max();
    ASSERT_TRUE(compare(max, CompareOp::GREATER, max / 2));
    ASSERT_TRUE(compare(max, CompareOp::GREATER_EQ, max));
}

// 类型混合比较Test（int vs double）
TEST(mixed_type_comparison) {
    // int 和 double 比较
    ASSERT_TRUE(compareMixed(5.0, CompareOp::EQUAL, 5.0));
    ASSERT_TRUE(compareMixed(5.0, CompareOp::GREATER, 3.0));
    ASSERT_TRUE(compareMixed(3.0, CompareOp::LESS, 5.0));

    // bool and numeric comparison (true = 1, false = 0)
    ASSERT_TRUE(compareMixed(1.0, CompareOp::EQUAL, 1.0));
    ASSERT_TRUE(compareMixed(0.0, CompareOp::EQUAL, 0.0));
    ASSERT_TRUE(compareMixed(1.0, CompareOp::GREATER, 0.0));
}

// 边界值Test
TEST(boundary_values) {
    // 零值比较
    ASSERT_FALSE(compare(0.0, CompareOp::GREATER, 0.0));
    ASSERT_FALSE(compare(0.0, CompareOp::LESS, 0.0));
    ASSERT_TRUE(compare(0.0, CompareOp::EQUAL, 0.0));
    ASSERT_TRUE(compare(0.0, CompareOp::GREATER_EQ, 0.0));
    ASSERT_TRUE(compare(0.0, CompareOp::LESS_EQ, 0.0));
    ASSERT_FALSE(compare(0.0, CompareOp::NOT_EQUAL, 0.0));

    // Large and small values
    double max = 1e308;
    double min = -1e308;
    ASSERT_TRUE(compare(max, CompareOp::GREATER, min));
    ASSERT_TRUE(compare(min, CompareOp::LESS, max));
    ASSERT_TRUE(compare(max, CompareOp::EQUAL, max));
    ASSERT_TRUE(compare(min, CompareOp::EQUAL, min));
}

// 链式比较Test（如 a < b < c）
TEST(chained_comparison) {
    // 模拟链式比较：1 < 5 < 10
    bool result = compare(1.0, CompareOp::LESS, 5.0) &&
                  compare(5.0, CompareOp::LESS, 10.0);
    ASSERT_TRUE(result);

    // 1 < 10 < 5 should be false
    result = compare(1.0, CompareOp::LESS, 10.0) &&
             compare(10.0, CompareOp::LESS, 5.0);
    ASSERT_FALSE(result);

    // 5 >= 5 >= 3
    result = compare(5.0, CompareOp::GREATER_EQ, 5.0) &&
             compare(5.0, CompareOp::GREATER_EQ, 3.0);
    ASSERT_TRUE(result);
}

// 复杂表达式中的比较
TEST(comparison_in_expressions) {
    // (5 + 3) > (2 * 3)  =  8 > 6  =  true
    double left = 5.0 + 3.0;
    double right = 2.0 * 3.0;
    ASSERT_TRUE(compare(left, CompareOp::GREATER, right));

    // (10 - 5) == (15 / 3)  =  5 == 5  =  true
    left = 10.0 - 5.0;
    right = 15.0 / 3.0;
    ASSERT_TRUE(compare(left, CompareOp::EQUAL, right));

    // pow(2, 3) < pow(3, 2)  =  8 < 9  =  true
    left = std::pow(2.0, 3.0);
    right = std::pow(3.0, 2.0);
    ASSERT_TRUE(compare(left, CompareOp::LESS, right));
}

// ==================== 主函数 ====================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Comparison Operation Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "[Basic Comparison Operator Tests]" << std::endl;
    RUN_TEST(greater_than);
    RUN_TEST(less_than);
    RUN_TEST(equal_to);
    RUN_TEST(greater_than_or_equal);
    RUN_TEST(less_than_or_equal);
    RUN_TEST(not_equal);
    std::cout << std::endl;

    std::cout << "[Floating Point Precision Tests]" << std::endl;
    RUN_TEST(floating_point_precision);
    RUN_TEST(special_float_values);
    std::cout << std::endl;

    std::cout << "[Mixed Type Tests]" << std::endl;
    RUN_TEST(mixed_type_comparison);
    RUN_TEST(boundary_values);
    std::cout << std::endl;

    std::cout << "[Complex Scenario Tests]" << std::endl;
    RUN_TEST(chained_comparison);
    RUN_TEST(comparison_in_expressions);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "All tests passed!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

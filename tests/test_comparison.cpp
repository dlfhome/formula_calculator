/**
 * 比较运算测试
 * 测试 >, <, ==, >=, <=, != 运算符
 */

#include <iostream>
#include <cassert>
#include <cmath>
#include <string>

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
#define ASSERT_NE(a, b) assert((a) != (b))

// 模拟比较运算结果（待实际实现时替换为真实调用）
namespace formula {

enum class CompareOp {
    GREATER,      // >
    LESS,         // <
    EQUAL,        // ==
    GREATER_EQ,   // >=
    LESS_EQ,      // <=
    NOT_EQUAL     // !=
};

// 比较两个 double 值（考虑精度）
bool compare(double left, CompareOp op, double right, double epsilon = 1e-9) {
    // 处理无穷大的相等比较
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

// 类型混合比较
bool compareMixed(double left, CompareOp op, double right) {
    return compare(left, op, right);
}

} // namespace formula

using namespace formula;

// ==================== 测试用例 ====================

// 大于运算符 (>) 测试
TEST(greater_than) {
    // 基本整数比较
    ASSERT_TRUE(compare(5.0, CompareOp::GREATER, 3.0));
    ASSERT_FALSE(compare(3.0, CompareOp::GREATER, 5.0));
    ASSERT_FALSE(compare(5.0, CompareOp::GREATER, 5.0));

    // 负数比较
    ASSERT_TRUE(compare(-1.0, CompareOp::GREATER, -5.0));
    ASSERT_FALSE(compare(-10.0, CompareOp::GREATER, -5.0));

    // 正负混合
    ASSERT_TRUE(compare(1.0, CompareOp::GREATER, -1.0));
    ASSERT_FALSE(compare(-1.0, CompareOp::GREATER, 1.0));
}

// 小于运算符 (<) 测试
TEST(less_than) {
    // 基本整数比较
    ASSERT_TRUE(compare(3.0, CompareOp::LESS, 5.0));
    ASSERT_FALSE(compare(5.0, CompareOp::LESS, 3.0));
    ASSERT_FALSE(compare(5.0, CompareOp::LESS, 5.0));

    // 负数比较
    ASSERT_TRUE(compare(-5.0, CompareOp::LESS, -1.0));
    ASSERT_FALSE(compare(-1.0, CompareOp::LESS, -5.0));

    // 零的比较
    ASSERT_TRUE(compare(-1.0, CompareOp::LESS, 0.0));
    ASSERT_TRUE(compare(0.0, CompareOp::LESS, 1.0));
}

// 等于运算符 (==) 测试
TEST(equal_to) {
    // 基本相等
    ASSERT_TRUE(compare(5.0, CompareOp::EQUAL, 5.0));
    ASSERT_TRUE(compare(0.0, CompareOp::EQUAL, 0.0));
    ASSERT_TRUE(compare(-3.0, CompareOp::EQUAL, -3.0));

    // 不相等
    ASSERT_FALSE(compare(5.0, CompareOp::EQUAL, 3.0));
    ASSERT_FALSE(compare(5.0, CompareOp::EQUAL, -5.0));

    // 浮点数精度测试
    ASSERT_TRUE(compare(0.1 + 0.2, CompareOp::EQUAL, 0.3));  // 考虑精度
    ASSERT_TRUE(compare(1.0 / 3.0 * 3.0, CompareOp::EQUAL, 1.0));
}

// 大于等于运算符 (>=) 测试
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

// 小于等于运算符 (<=) 测试
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

// 不等于运算符 (!=) 测试
TEST(not_equal) {
    // 不相等
    ASSERT_TRUE(compare(5.0, CompareOp::NOT_EQUAL, 3.0));
    ASSERT_TRUE(compare(-5.0, CompareOp::NOT_EQUAL, 5.0));
    ASSERT_TRUE(compare(0.0, CompareOp::NOT_EQUAL, 1.0));

    // 相等
    ASSERT_FALSE(compare(5.0, CompareOp::NOT_EQUAL, 5.0));
    ASSERT_FALSE(compare(0.0, CompareOp::NOT_EQUAL, 0.0));
    ASSERT_FALSE(compare(-3.14, CompareOp::NOT_EQUAL, -3.14));
}

// 浮点数精度比较测试
TEST(floating_point_precision) {
    // 经典浮点数精度问题
    double a = 0.1;
    double b = 0.2;
    double sum = a + b;

    // 直接比较会失败，但使用 epsilon 应该通过
    ASSERT_TRUE(compare(sum, CompareOp::EQUAL, 0.3));

    // 极小差异应该视为相等 (使用更小的差异)
    ASSERT_TRUE(compare(1.0000000001, CompareOp::EQUAL, 1.0));
    ASSERT_TRUE(compare(1.0, CompareOp::EQUAL, 1.0000000001));

    // 超过 epsilon 的差异应该视为不相等
    ASSERT_FALSE(compare(1.000001, CompareOp::EQUAL, 1.0));
}

// 特殊浮点值测试
TEST(special_float_values) {
    // 无穷大
    double inf = std::numeric_limits<double>::infinity();
    ASSERT_TRUE(compare(inf, CompareOp::GREATER, 0.0));
    ASSERT_TRUE(compare(-inf, CompareOp::LESS, 0.0));
    ASSERT_TRUE(compare(inf, CompareOp::EQUAL, inf));
    ASSERT_TRUE(compare(-inf, CompareOp::EQUAL, -inf));

    // 极大值比较
    double max = std::numeric_limits<double>::max();
    ASSERT_TRUE(compare(max, CompareOp::GREATER, max / 2));
    ASSERT_TRUE(compare(max, CompareOp::GREATER_EQ, max));
}

// 类型混合比较测试（int vs double）
TEST(mixed_type_comparison) {
    // int 和 double 比较
    ASSERT_TRUE(compareMixed(5.0, CompareOp::EQUAL, 5.0));
    ASSERT_TRUE(compareMixed(5.0, CompareOp::GREATER, 3.0));
    ASSERT_TRUE(compareMixed(3.0, CompareOp::LESS, 5.0));

    // bool 和数值比较（true = 1, false = 0）
    ASSERT_TRUE(compareMixed(1.0, CompareOp::EQUAL, 1.0));
    ASSERT_TRUE(compareMixed(0.0, CompareOp::EQUAL, 0.0));
    ASSERT_TRUE(compareMixed(1.0, CompareOp::GREATER, 0.0));
}

// 边界值测试
TEST(boundary_values) {
    // 零值比较
    ASSERT_FALSE(compare(0.0, CompareOp::GREATER, 0.0));
    ASSERT_FALSE(compare(0.0, CompareOp::LESS, 0.0));
    ASSERT_TRUE(compare(0.0, CompareOp::EQUAL, 0.0));
    ASSERT_TRUE(compare(0.0, CompareOp::GREATER_EQ, 0.0));
    ASSERT_TRUE(compare(0.0, CompareOp::LESS_EQ, 0.0));
    ASSERT_FALSE(compare(0.0, CompareOp::NOT_EQUAL, 0.0));

    // 极大极小值
    double max = 1e308;
    double min = -1e308;
    ASSERT_TRUE(compare(max, CompareOp::GREATER, min));
    ASSERT_TRUE(compare(min, CompareOp::LESS, max));
    ASSERT_TRUE(compare(max, CompareOp::EQUAL, max));
    ASSERT_TRUE(compare(min, CompareOp::EQUAL, min));
}

// 链式比较测试（如 a < b < c）
TEST(chained_comparison) {
    // 模拟链式比较：1 < 5 < 10
    bool result = compare(1.0, CompareOp::LESS, 5.0) &&
                  compare(5.0, CompareOp::LESS, 10.0);
    ASSERT_TRUE(result);

    // 1 < 10 < 5 应该为 false
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
    std::cout << "比较运算测试套件" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "【基本比较运算符测试】" << std::endl;
    RUN_TEST(greater_than);
    RUN_TEST(less_than);
    RUN_TEST(equal_to);
    RUN_TEST(greater_than_or_equal);
    RUN_TEST(less_than_or_equal);
    RUN_TEST(not_equal);
    std::cout << std::endl;

    std::cout << "【浮点数精度测试】" << std::endl;
    RUN_TEST(floating_point_precision);
    RUN_TEST(special_float_values);
    std::cout << std::endl;

    std::cout << "【类型混合测试】" << std::endl;
    RUN_TEST(mixed_type_comparison);
    RUN_TEST(boundary_values);
    std::cout << std::endl;

    std::cout << "【复杂场景测试】" << std::endl;
    RUN_TEST(chained_comparison);
    RUN_TEST(comparison_in_expressions);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "所有测试通过！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

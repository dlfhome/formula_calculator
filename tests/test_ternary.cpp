/**
 * Ternary Operator Tests
 * Test condition?true_value:false_value operator
 * Including nested ternary operators and short-circuit evaluation
 */

#include <iostream>
#include <cassert>
#include <vector>
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
#define ASSERT_NEAR(a, b, eps) assert(std::abs((a) - (b)) < (eps))

namespace formula {

// Mock ternary operator (using macro to implement short-circuit evaluation)
#define ternary(condition, trueValue, falseValue) ((condition) ? (trueValue) : (falseValue))

// Numeric to boolean conversion
bool toBool(double value) {
    return value != 0.0;
}

// Short-circuit evaluation tracker
class ShortCircuitTracker {
public:
    std::vector<std::string> evaluated;

    void reset() {
        evaluated.clear();
    }

    double trackAndReturn(const std::string& name, double value) {
        evaluated.push_back(name);
        return value;
    }

    bool trackAndReturn(const std::string& name, bool value) {
        evaluated.push_back(name);
        return value;
    }
};

} // namespace formula

using namespace formula;

// ==================== Test Cases ====================

// Basic ternary operator test
TEST(ternary_basic) {
    // true ? a : b = a
    ASSERT_EQ(ternary(true, 10, 20), 10);
    ASSERT_EQ(ternary(true, 1.5, 2.5), 1.5);

    // false ? a : b = b
    ASSERT_EQ(ternary(false, 10, 20), 20);
    ASSERT_EQ(ternary(false, 1.5, 2.5), 2.5);
}

// Numeric condition test (non-zero is true, zero is false)
TEST(ternary_numeric_condition) {
    // Non-zero value as true
    ASSERT_EQ(ternary(toBool(1.0), 100, 200), 100);
    ASSERT_EQ(ternary(toBool(-5.0), 100, 200), 100);
    ASSERT_EQ(ternary(toBool(0.001), 100, 200), 100);

    // Zero value as false
    ASSERT_EQ(ternary(toBool(0.0), 100, 200), 200);
    ASSERT_EQ(ternary(toBool(-0.0), 100, 200), 200);
}

// Different return type test
TEST(ternary_return_types) {
    // int 类型
    ASSERT_EQ(ternary(true, 42, 0), 42);
    ASSERT_EQ(ternary(false, 42, 0), 0);

    // double 类型
    ASSERT_NEAR(ternary(true, 3.14, 2.71), 3.14, 1e-10);
    ASSERT_NEAR(ternary(false, 3.14, 2.71), 2.71, 1e-10);

    // bool 类型
    ASSERT_EQ(ternary(true, true, false), true);
    ASSERT_EQ(ternary(false, true, false), false);
}

// Nested ternary operator test
TEST(ternary_nested) {
    // 单层嵌套: a ? b : (c ? d : e)
    // true ? 1 : (false ? 2 : 3) = 1
    ASSERT_EQ(ternary(true, 1, ternary(false, 2, 3)), 1);

    // false ? 1 : (true ? 2 : 3) = 2
    ASSERT_EQ(ternary(false, 1, ternary(true, 2, 3)), 2);

    // false ? 1 : (false ? 2 : 3) = 3
    ASSERT_EQ(ternary(false, 1, ternary(false, 2, 3)), 3);

    // Multi-level nesting
    // true ? (false ? 10 : 20) : (true ? 30 : 40) = 20
    ASSERT_EQ(ternary(true,
                      ternary(false, 10, 20),
                      ternary(true, 30, 40)), 20);

    // false ? (true ? 10 : 20) : (false ? 30 : 40) = 40
    ASSERT_EQ(ternary(false,
                      ternary(true, 10, 20),
                      ternary(false, 30, 40)), 40);
}

// Ternary operator as condition
TEST(ternary_as_condition) {
    // (a ? b : c) ? d : e
    // (true ? true : false) ? 100 : 200 = 100
    bool inner = ternary(true, true, false);
    ASSERT_EQ(ternary(inner, 100, 200), 100);

    // (false ? true : false) ? 100 : 200 = 200
    inner = ternary(false, true, false);
    ASSERT_EQ(ternary(inner, 100, 200), 200);
}

// Short-circuit evaluation test - true branch
TEST(ternary_short_circuit_true_branch) {
    ShortCircuitTracker tracker;

    // true ? a : b - 只有 a 被求值
    tracker.reset();
    double result = ternary(
        tracker.trackAndReturn("condition", true),
        tracker.trackAndReturn("trueBranch", 10.0),
        tracker.trackAndReturn("falseBranch", 20.0)
    );
    ASSERT_EQ(result, 10.0);
    ASSERT_EQ(tracker.evaluated.size(), 2);
    ASSERT_EQ(tracker.evaluated[0], "condition");
    ASSERT_EQ(tracker.evaluated[1], "trueBranch");
    // falseBranch should not be evaluated
}

// Short-circuit evaluation test - false branch
TEST(ternary_short_circuit_false_branch) {
    ShortCircuitTracker tracker;

    // false ? a : b - 只有 b 被求值
    tracker.reset();
    double result = ternary(
        tracker.trackAndReturn("condition", false),
        tracker.trackAndReturn("trueBranch", 10.0),
        tracker.trackAndReturn("falseBranch", 20.0)
    );
    ASSERT_EQ(result, 20.0);
    ASSERT_EQ(tracker.evaluated.size(), 2);
    ASSERT_EQ(tracker.evaluated[0], "condition");
    ASSERT_EQ(tracker.evaluated[1], "falseBranch");
    // trueBranch should not be evaluated
}

// Complex nested short-circuit evaluation
TEST(ternary_nested_short_circuit) {
    ShortCircuitTracker tracker;

    // false ? (a ? b : c) : (d ? e : f)
    // 应该只求值 d ? e : f，且根据 d 的值决定求值 e 或 f
    tracker.reset();
    bool cond = tracker.trackAndReturn("outerCond", false);
    double result = ternary(cond,
        ternary(tracker.trackAndReturn("innerCond1", true),
                tracker.trackAndReturn("true1", 1.0),
                tracker.trackAndReturn("false1", 2.0)),
        ternary(tracker.trackAndReturn("innerCond2", true),
                tracker.trackAndReturn("true2", 3.0),
                tracker.trackAndReturn("false2", 4.0))
    );
    ASSERT_EQ(result, 3.0);
    // Should evaluate: outerCond, innerCond2, true2
    ASSERT_EQ(tracker.evaluated.size(), 3);
    ASSERT_EQ(tracker.evaluated[0], "outerCond");
    ASSERT_EQ(tracker.evaluated[1], "innerCond2");
    ASSERT_EQ(tracker.evaluated[2], "true2");
}

// Boundary value test
TEST(ternary_boundary_values) {
    // Large and small values
    double max = 1e308;
    double min = -1e308;
    ASSERT_EQ(ternary(true, max, min), max);
    ASSERT_EQ(ternary(false, max, min), min);

    // 零值
    ASSERT_EQ(ternary(true, 0.0, 1.0), 0.0);
    ASSERT_EQ(ternary(false, 1.0, 0.0), 0.0);

    // Equal values
    ASSERT_EQ(ternary(true, 42.0, 42.0), 42.0);
    ASSERT_EQ(ternary(false, 42.0, 42.0), 42.0);
}

// Real-world scenario test
TEST(ternary_real_world_usage) {
    // 绝对值: x >= 0 ? x : -x
    auto abs = [](double x) {
        return ternary(x >= 0, x, -x);
    };
    ASSERT_EQ(abs(5), 5);
    ASSERT_EQ(abs(-5), 5);
    ASSERT_EQ(abs(0), 0);

    // 最大值: a > b ? a : b
    auto max = [](double a, double b) {
        return ternary(a > b, a, b);
    };
    ASSERT_EQ(max(10, 5), 10);
    ASSERT_EQ(max(5, 10), 10);
    ASSERT_EQ(max(7, 7), 7);

    // 最小值: a < b ? a : b
    auto min = [](double a, double b) {
        return ternary(a < b, a, b);
    };
    ASSERT_EQ(min(10, 5), 5);
    ASSERT_EQ(min(5, 10), 5);
    ASSERT_EQ(min(7, 7), 7);

    // 符号函数: x > 0 ? 1 : (x < 0 ? -1 : 0)
    auto sign = [](double x) {
        return ternary(x > 0, 1.0, ternary(x < 0, -1.0, 0.0));
    };
    ASSERT_EQ(sign(5), 1);
    ASSERT_EQ(sign(-5), -1);
    ASSERT_EQ(sign(0), 0);

    // Clamp function: x < min ? min : (x > max ? max : x)
    auto clamp = [](double x, double minVal, double maxVal) {
        return ternary(x < minVal, minVal,
                      ternary(x > maxVal, maxVal, x));
    };
    ASSERT_EQ(clamp(50, 0, 100), 50);
    ASSERT_EQ(clamp(-10, 0, 100), 0);
    ASSERT_EQ(clamp(150, 0, 100), 100);
}

// Combined with comparison operators
TEST(ternary_with_comparison) {
    // 成绩等级: score >= 90 ? "A" : (score >= 80 ? "B" : "C")
    auto getGrade = [](double score) {
        return ternary(score >= 90, 1,  // A = 1
                      ternary(score >= 80, 2,  // B = 2
                             ternary(score >= 60, 3, 4)));  // C = 3, D = 4
    };
    ASSERT_EQ(getGrade(95), 1);  // A
    ASSERT_EQ(getGrade(85), 2);  // B
    ASSERT_EQ(getGrade(70), 3);  // C
    ASSERT_EQ(getGrade(50), 4);  // D
}

// Combined with logical operators
TEST(ternary_with_logical) {
    // (a && b) ? x : y
    auto result = ternary(true && true, 100, 200);
    ASSERT_EQ(result, 100);

    result = ternary(true && false, 100, 200);
    ASSERT_EQ(result, 200);

    // (a || b) ? x : y
    result = ternary(false || true, 100, 200);
    ASSERT_EQ(result, 100);

    result = ternary(false || false, 100, 200);
    ASSERT_EQ(result, 200);

    // !a ? x : y
    result = ternary(!true, 100, 200);
    ASSERT_EQ(result, 200);

    result = ternary(!false, 100, 200);
    ASSERT_EQ(result, 100);
}

// Type promotion test
TEST(ternary_type_promotion) {
    // int and double mixed, result should be double
    double result = ternary(true, 5, 3.14);  // int 5 提升为 double
    ASSERT_NEAR(result, 5.0, 1e-10);

    result = ternary(false, 5, 3.14);
    ASSERT_NEAR(result, 3.14, 1e-10);

    // bool 和 int 混合
    int intResult = ternary(true, true, 0);  // true 转为 1
    ASSERT_EQ(intResult, 1);

    intResult = ternary(false, 1, false);  // false 转为 0
    ASSERT_EQ(intResult, 0);
}

// ==================== Main Function ====================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Ternary Operator Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "[Basic Function Tests]" << std::endl;
    RUN_TEST(ternary_basic);
    RUN_TEST(ternary_numeric_condition);
    RUN_TEST(ternary_return_types);
    std::cout << std::endl;

    std::cout << "[Nested Ternary Operator Tests]" << std::endl;
    RUN_TEST(ternary_nested);
    RUN_TEST(ternary_as_condition);
    std::cout << std::endl;

    std::cout << "[Short-circuit Evaluation Tests]" << std::endl;
    RUN_TEST(ternary_short_circuit_true_branch);
    RUN_TEST(ternary_short_circuit_false_branch);
    RUN_TEST(ternary_nested_short_circuit);
    std::cout << std::endl;

    std::cout << "[Boundary Value Tests]" << std::endl;
    RUN_TEST(ternary_boundary_values);
    std::cout << std::endl;

    std::cout << "[Real-world Scenario Tests]" << std::endl;
    RUN_TEST(ternary_real_world_usage);
    RUN_TEST(ternary_with_comparison);
    RUN_TEST(ternary_with_logical);
    RUN_TEST(ternary_type_promotion);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "All tests passed!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

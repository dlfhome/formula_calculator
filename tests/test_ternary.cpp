/**
 * 三元运算符测试
 * 测试 条件?真值:假值 运算符
 * 包括嵌套三元运算符和短路求值
 */

#include <iostream>
#include <cassert>
#include <vector>
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
#define ASSERT_NEAR(a, b, eps) assert(std::abs((a) - (b)) < (eps))

namespace formula {

// 模拟三元运算符（使用宏实现短路求值）
#define ternary(condition, trueValue, falseValue) ((condition) ? (trueValue) : (falseValue))

// 数值转布尔
bool toBool(double value) {
    return value != 0.0;
}

// 短路求值追踪器
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

// ==================== 测试用例 ====================

// 基本三元运算符测试
TEST(ternary_basic) {
    // true ? a : b = a
    ASSERT_EQ(ternary(true, 10, 20), 10);
    ASSERT_EQ(ternary(true, 1.5, 2.5), 1.5);

    // false ? a : b = b
    ASSERT_EQ(ternary(false, 10, 20), 20);
    ASSERT_EQ(ternary(false, 1.5, 2.5), 2.5);
}

// 数值条件测试（非零为真，零为假）
TEST(ternary_numeric_condition) {
    // 非零值作为 true
    ASSERT_EQ(ternary(toBool(1.0), 100, 200), 100);
    ASSERT_EQ(ternary(toBool(-5.0), 100, 200), 100);
    ASSERT_EQ(ternary(toBool(0.001), 100, 200), 100);

    // 零值作为 false
    ASSERT_EQ(ternary(toBool(0.0), 100, 200), 200);
    ASSERT_EQ(ternary(toBool(-0.0), 100, 200), 200);
}

// 不同类型返回值测试
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

// 嵌套三元运算符测试
TEST(ternary_nested) {
    // 单层嵌套: a ? b : (c ? d : e)
    // true ? 1 : (false ? 2 : 3) = 1
    ASSERT_EQ(ternary(true, 1, ternary(false, 2, 3)), 1);

    // false ? 1 : (true ? 2 : 3) = 2
    ASSERT_EQ(ternary(false, 1, ternary(true, 2, 3)), 2);

    // false ? 1 : (false ? 2 : 3) = 3
    ASSERT_EQ(ternary(false, 1, ternary(false, 2, 3)), 3);

    // 多层嵌套
    // true ? (false ? 10 : 20) : (true ? 30 : 40) = 20
    ASSERT_EQ(ternary(true,
                      ternary(false, 10, 20),
                      ternary(true, 30, 40)), 20);

    // false ? (true ? 10 : 20) : (false ? 30 : 40) = 40
    ASSERT_EQ(ternary(false,
                      ternary(true, 10, 20),
                      ternary(false, 30, 40)), 40);
}

// 三元运算符作为条件
TEST(ternary_as_condition) {
    // (a ? b : c) ? d : e
    // (true ? true : false) ? 100 : 200 = 100
    bool inner = ternary(true, true, false);
    ASSERT_EQ(ternary(inner, 100, 200), 100);

    // (false ? true : false) ? 100 : 200 = 200
    inner = ternary(false, true, false);
    ASSERT_EQ(ternary(inner, 100, 200), 200);
}

// 短路求值测试 - 真值分支
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
    // falseBranch 不应被求值
}

// 短路求值测试 - 假值分支
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
    // trueBranch 不应被求值
}

// 复杂嵌套短路求值
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
    // 应该求值: outerCond, innerCond2, true2
    ASSERT_EQ(tracker.evaluated.size(), 3);
    ASSERT_EQ(tracker.evaluated[0], "outerCond");
    ASSERT_EQ(tracker.evaluated[1], "innerCond2");
    ASSERT_EQ(tracker.evaluated[2], "true2");
}

// 边界值测试
TEST(ternary_boundary_values) {
    // 极大极小值
    double max = 1e308;
    double min = -1e308;
    ASSERT_EQ(ternary(true, max, min), max);
    ASSERT_EQ(ternary(false, max, min), min);

    // 零值
    ASSERT_EQ(ternary(true, 0.0, 1.0), 0.0);
    ASSERT_EQ(ternary(false, 1.0, 0.0), 0.0);

    // 相等值
    ASSERT_EQ(ternary(true, 42.0, 42.0), 42.0);
    ASSERT_EQ(ternary(false, 42.0, 42.0), 42.0);
}

// 实际应用场景测试
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

    // 钳制函数: x < min ? min : (x > max ? max : x)
    auto clamp = [](double x, double minVal, double maxVal) {
        return ternary(x < minVal, minVal,
                      ternary(x > maxVal, maxVal, x));
    };
    ASSERT_EQ(clamp(50, 0, 100), 50);
    ASSERT_EQ(clamp(-10, 0, 100), 0);
    ASSERT_EQ(clamp(150, 0, 100), 100);
}

// 与比较运算符组合
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

// 与逻辑运算符组合
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

// 类型提升测试
TEST(ternary_type_promotion) {
    // int 和 double 混合，结果应为 double
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

// ==================== 主函数 ====================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "三元运算符测试套件" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "【基本功能测试】" << std::endl;
    RUN_TEST(ternary_basic);
    RUN_TEST(ternary_numeric_condition);
    RUN_TEST(ternary_return_types);
    std::cout << std::endl;

    std::cout << "【嵌套三元运算符测试】" << std::endl;
    RUN_TEST(ternary_nested);
    RUN_TEST(ternary_as_condition);
    std::cout << std::endl;

    std::cout << "【短路求值测试】" << std::endl;
    RUN_TEST(ternary_short_circuit_true_branch);
    RUN_TEST(ternary_short_circuit_false_branch);
    RUN_TEST(ternary_nested_short_circuit);
    std::cout << std::endl;

    std::cout << "【边界值测试】" << std::endl;
    RUN_TEST(ternary_boundary_values);
    std::cout << std::endl;

    std::cout << "【实际应用场景测试】" << std::endl;
    RUN_TEST(ternary_real_world_usage);
    RUN_TEST(ternary_with_comparison);
    RUN_TEST(ternary_with_logical);
    RUN_TEST(ternary_type_promotion);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "所有测试通过！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

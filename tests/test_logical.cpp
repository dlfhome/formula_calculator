/**
 * 逻辑运算测试
 * 测试 && (与), || (或), ! (非) 运算符
 * 包括短路求值测试
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

namespace formula {

// 模拟逻辑运算（待实际实现时替换为真实调用）

// 逻辑与 (&&) - 使用宏实现短路求值
#define logicalAnd(left, right) ((left) && (right))

// 逻辑或 (||) - 使用宏实现短路求值
#define logicalOr(left, right) ((left) || (right))

// 逻辑非 (!)
inline bool logicalNot(bool operand) {
    return !operand;
}

// 数值转布尔（用于逻辑运算）
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

    bool trackAndReturn(const std::string& name, bool value) {
        evaluated.push_back(name);
        return value;
    }

    bool trackAndReturn(const std::string& name, double value) {
        evaluated.push_back(name);
        return value != 0.0;
    }
};

} // namespace formula

using namespace formula;

// ==================== 测试用例 ====================

// 逻辑与 (&&) 基本测试
TEST(logical_and_basic) {
    // true && true = true
    ASSERT_TRUE(logicalAnd(true, true));

    // true && false = false
    ASSERT_FALSE(logicalAnd(true, false));

    // false && true = false
    ASSERT_FALSE(logicalAnd(false, true));

    // false && false = false
    ASSERT_FALSE(logicalAnd(false, false));
}

// 逻辑或 (||) 基本测试
TEST(logical_or_basic) {
    // true || true = true
    ASSERT_TRUE(logicalOr(true, true));

    // true || false = true
    ASSERT_TRUE(logicalOr(true, false));

    // false || true = true
    ASSERT_TRUE(logicalOr(false, true));

    // false || false = false
    ASSERT_FALSE(logicalOr(false, false));
}

// 逻辑非 (!) 基本测试
TEST(logical_not_basic) {
    // !true = false
    ASSERT_FALSE(logicalNot(true));

    // !false = true
    ASSERT_TRUE(logicalNot(false));

    // 双重否定
    ASSERT_TRUE(logicalNot(logicalNot(true)));
    ASSERT_FALSE(logicalNot(logicalNot(false)));
}

// 数值转布尔测试
TEST(numeric_to_bool) {
    // 非零值为 true
    ASSERT_TRUE(toBool(1.0));
    ASSERT_TRUE(toBool(-1.0));
    ASSERT_TRUE(toBool(0.0001));
    ASSERT_TRUE(toBool(1000000.0));
    ASSERT_TRUE(toBool(-999.99));

    // 零值为 false
    ASSERT_FALSE(toBool(0.0));
    ASSERT_FALSE(toBool(-0.0));  // 负零也是零
}

// 逻辑与短路求值测试
TEST(logical_and_short_circuit) {
    ShortCircuitTracker tracker;

    // false && anything = false，右侧不应被求值
    tracker.reset();
    bool left = tracker.trackAndReturn("left", false);
    bool result = left && tracker.trackAndReturn("right", true);
    ASSERT_FALSE(result);
    // 右侧不应被求值
    ASSERT_EQ(tracker.evaluated.size(), 1);
    ASSERT_EQ(tracker.evaluated[0], "left");

    // true && right = right，右侧应该被求值
    tracker.reset();
    left = tracker.trackAndReturn("left", true);
    result = left && tracker.trackAndReturn("right", false);
    ASSERT_FALSE(result);
    // 两侧都应被求值
    ASSERT_EQ(tracker.evaluated.size(), 2);
    ASSERT_EQ(tracker.evaluated[0], "left");
    ASSERT_EQ(tracker.evaluated[1], "right");
}

// 逻辑或短路求值测试
TEST(logical_or_short_circuit) {
    ShortCircuitTracker tracker;

    // true || anything = true，右侧不应被求值
    tracker.reset();
    bool left = tracker.trackAndReturn("left", true);
    bool result = left || tracker.trackAndReturn("right", false);
    ASSERT_TRUE(result);
    // 右侧不应被求值
    ASSERT_EQ(tracker.evaluated.size(), 1);
    ASSERT_EQ(tracker.evaluated[0], "left");

    // false || right = right，右侧应该被求值
    tracker.reset();
    left = tracker.trackAndReturn("left", false);
    result = left || tracker.trackAndReturn("right", true);
    ASSERT_TRUE(result);
    // 两侧都应被求值
    ASSERT_EQ(tracker.evaluated.size(), 2);
    ASSERT_EQ(tracker.evaluated[0], "left");
    ASSERT_EQ(tracker.evaluated[1], "right");
}

// 复杂短路求值场景
TEST(complex_short_circuit) {
    ShortCircuitTracker tracker;

    // (false && x) || (true && y) - 第一个 && 短路，第二个 && 不短路
    tracker.reset();
    bool result = (tracker.trackAndReturn("a", false) &&
                   tracker.trackAndReturn("b", true)) ||  // b 不应被求值
                  (tracker.trackAndReturn("c", true) &&
                   tracker.trackAndReturn("d", true));   // d 应该被求值
    ASSERT_TRUE(result);
    // a, c, d 应该被求值，b 不应该
    ASSERT_EQ(tracker.evaluated.size(), 3);
    ASSERT_EQ(tracker.evaluated[0], "a");
    ASSERT_EQ(tracker.evaluated[1], "c");
    ASSERT_EQ(tracker.evaluated[2], "d");
}

// 逻辑运算组合测试
TEST(logical_combinations) {
    // 德摩根定律: !(a && b) == !a || !b
    for (bool a : {true, false}) {
        for (bool b : {true, false}) {
            bool left = logicalNot(logicalAnd(a, b));
            bool right = logicalOr(logicalNot(a), logicalNot(b));
            ASSERT_EQ(left, right);
        }
    }

    // 德摩根定律: !(a || b) == !a && !b
    for (bool a : {true, false}) {
        for (bool b : {true, false}) {
            bool left = logicalNot(logicalOr(a, b));
            bool right = logicalAnd(logicalNot(a), logicalNot(b));
            ASSERT_EQ(left, right);
        }
    }
}

// 多操作数链式逻辑运算
TEST(chained_logical_operations) {
    // true && true && true = true
    ASSERT_TRUE(logicalAnd(logicalAnd(true, true), true));

    // true && true && false = false
    ASSERT_FALSE(logicalAnd(logicalAnd(true, true), false));

    // false || false || true = true
    ASSERT_TRUE(logicalOr(logicalOr(false, false), true));

    // false || false || false = false
    ASSERT_FALSE(logicalOr(logicalOr(false, false), false));

    // 混合: true && false || true
    // 注意：&& 优先级高于 ||，所以是 (true && false) || true = true
    ASSERT_TRUE(logicalOr(logicalAnd(true, false), true));

    // true || false && false = true（&& 先计算）
    ASSERT_TRUE(logicalOr(true, logicalAnd(false, false)));
}

// 边界情况测试
TEST(logical_edge_cases) {
    // 与自身进行逻辑运算
    ASSERT_TRUE(logicalAnd(true, true));
    ASSERT_FALSE(logicalAnd(false, false));
    ASSERT_TRUE(logicalOr(true, true));
    ASSERT_FALSE(logicalOr(false, false));

    // 与非进行逻辑运算
    ASSERT_FALSE(logicalAnd(true, logicalNot(true)));
    ASSERT_TRUE(logicalOr(false, logicalNot(false)));

    // 连续非运算
    ASSERT_TRUE(logicalNot(logicalNot(logicalNot(logicalNot(true)))));
    ASSERT_FALSE(logicalNot(logicalNot(logicalNot(logicalNot(false)))));
}

// 实际表达式场景测试
TEST(real_world_expressions) {
    // 范围检查: x >= 0 && x <= 100
    auto inRange = [](double x) {
        return toBool(x >= 0.0) && toBool(x <= 100.0);
    };
    ASSERT_TRUE(inRange(0));
    ASSERT_TRUE(inRange(50));
    ASSERT_TRUE(inRange(100));
    ASSERT_FALSE(inRange(-1));
    ASSERT_FALSE(inRange(101));

    // 有效性检查: x != 0 && y != 0
    auto bothNonZero = [](double x, double y) {
        return toBool(x) && toBool(y);
    };
    ASSERT_TRUE(bothNonZero(1, 2));
    ASSERT_FALSE(bothNonZero(0, 1));
    ASSERT_FALSE(bothNonZero(1, 0));
    ASSERT_FALSE(bothNonZero(0, 0));

    // 默认回退: valid || default_value
    auto withDefault = [](bool valid, double value, double defaultVal) {
        return valid ? value : defaultVal;
    };
    ASSERT_EQ(withDefault(true, 42, 0), 42);
    ASSERT_EQ(withDefault(false, 42, 100), 100);
}

// 短路求值的副作用避免测试
TEST(short_circuit_avoids_side_effects) {
    int sideEffectCount = 0;

    auto withSideEffect = [&sideEffectCount](bool value) -> bool {
        sideEffectCount++;
        return value;
    };

    // false && anything - 副作用只发生一次
    sideEffectCount = 0;
    bool result = withSideEffect(false) && withSideEffect(true);
    ASSERT_FALSE(result);
    ASSERT_EQ(sideEffectCount, 1);  // 只有左侧被求值

    // true || anything - 副作用只发生一次
    sideEffectCount = 0;
    result = withSideEffect(true) || withSideEffect(false);
    ASSERT_TRUE(result);
    ASSERT_EQ(sideEffectCount, 1);  // 只有左侧被求值

    // true && anything - 副作用发生两次
    sideEffectCount = 0;
    result = withSideEffect(true) && withSideEffect(true);
    ASSERT_TRUE(result);
    ASSERT_EQ(sideEffectCount, 2);  // 两侧都被求值
}

// ==================== 主函数 ====================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "逻辑运算测试套件" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "【基本逻辑运算测试】" << std::endl;
    RUN_TEST(logical_and_basic);
    RUN_TEST(logical_or_basic);
    RUN_TEST(logical_not_basic);
    std::cout << std::endl;

    std::cout << "【数值转布尔测试】" << std::endl;
    RUN_TEST(numeric_to_bool);
    std::cout << std::endl;

    std::cout << "【短路求值测试】" << std::endl;
    RUN_TEST(logical_and_short_circuit);
    RUN_TEST(logical_or_short_circuit);
    RUN_TEST(complex_short_circuit);
    std::cout << std::endl;

    std::cout << "【组合运算测试】" << std::endl;
    RUN_TEST(logical_combinations);
    RUN_TEST(chained_logical_operations);
    RUN_TEST(logical_edge_cases);
    std::cout << std::endl;

    std::cout << "【实际场景测试】" << std::endl;
    RUN_TEST(real_world_expressions);
    RUN_TEST(short_circuit_avoids_side_effects);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "所有测试通过！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

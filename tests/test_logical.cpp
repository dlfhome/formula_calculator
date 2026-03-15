/**
 * Logical Operator Tests
 * Test && (AND), || (OR), ! (NOT) operators
 * Including short-circuit evaluation tests
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

namespace formula {

// Mock logical operations (replace with real calls when actual implementation is available)

// Logical AND (&&) - use macro to implement short-circuit evaluation
#define logicalAnd(left, right) ((left) && (right))

// Logical OR (||) - use macro to implement short-circuit evaluation
#define logicalOr(left, right) ((left) || (right))

// Logical NOT (!)
inline bool logicalNot(bool operand) {
    return !operand;
}

// Convert numeric to boolean (for logical operations)
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

// ==================== Test Cases ====================

// Logical AND (&&) basic tests
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

// Logical OR (||) basic tests
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

// Logical NOT (!) basic tests
TEST(logical_not_basic) {
    // !true = false
    ASSERT_FALSE(logicalNot(true));

    // !false = true
    ASSERT_TRUE(logicalNot(false));

    // Double negation
    ASSERT_TRUE(logicalNot(logicalNot(true)));
    ASSERT_FALSE(logicalNot(logicalNot(false)));
}

// Numeric to boolean conversion tests
TEST(numeric_to_boolean) {
    // Non-zero value is true
    ASSERT_TRUE(toBool(1.0));
    ASSERT_TRUE(toBool(-1.0));
    ASSERT_TRUE(toBool(3.14));

    // Zero value is false
    ASSERT_FALSE(toBool(0.0));
    ASSERT_FALSE(toBool(-0.0));
}

// Logical AND short-circuit evaluation tests
TEST(logical_and_short_circuit) {
    ShortCircuitTracker tracker;

    // false && anything = false, right side should not be evaluated
    tracker.reset();
    bool result = false && tracker.trackAndReturn("right", true);
    ASSERT_FALSE(result);
    ASSERT_EQ(tracker.evaluated.size(), 0);  // Right side should not be evaluated

    // true && right = right, right side should be evaluated
    tracker.reset();
    result = true && tracker.trackAndReturn("right", true);
    ASSERT_TRUE(result);
    ASSERT_EQ(tracker.evaluated.size(), 1);  // Right side should be evaluated

    // Both sides should be evaluated
    tracker.reset();
    result = tracker.trackAndReturn("left", true) && tracker.trackAndReturn("right", true);
    ASSERT_TRUE(result);
    ASSERT_EQ(tracker.evaluated.size(), 2);
}

// Logical OR short-circuit evaluation tests
TEST(logical_or_short_circuit) {
    ShortCircuitTracker tracker;

    // true || anything = true, right side should not be evaluated
    tracker.reset();
    bool result = true || tracker.trackAndReturn("right", false);
    ASSERT_TRUE(result);
    ASSERT_EQ(tracker.evaluated.size(), 0);  // Right side should not be evaluated

    // false || right = right, right side should be evaluated
    tracker.reset();
    result = false || tracker.trackAndReturn("right", true);
    ASSERT_TRUE(result);
    ASSERT_EQ(tracker.evaluated.size(), 1);  // Right side should be evaluated

    // Both sides should be evaluated
    tracker.reset();
    result = tracker.trackAndReturn("left", false) || tracker.trackAndReturn("right", true);
    ASSERT_TRUE(result);
    ASSERT_EQ(tracker.evaluated.size(), 2);
}

// Complex short-circuit evaluation scenarios
TEST(complex_short_circuit) {
    ShortCircuitTracker tracker;

    // (false && x) || (true && y) - first && short-circuits, second && does not
    tracker.reset();
    bool result = (false && tracker.trackAndReturn("a", true)) ||  // a should not be evaluated
                  (true && tracker.trackAndReturn("b", true));    // b should be evaluated

    // Only b should be evaluated
    ASSERT_TRUE(result);
    ASSERT_EQ(tracker.evaluated.size(), 1);
    ASSERT_EQ(tracker.evaluated[0], "b");
}

// Logical operation combination tests
TEST(logical_combinations) {
    // De Morgan's laws: !(a && b) == !a || !b
    bool a = true, b = false;
    ASSERT_EQ(logicalNot(logicalAnd(a, b)), logicalOr(logicalNot(a), logicalNot(b)));

    // De Morgan's laws: !(a || b) == !a && !b
    ASSERT_EQ(logicalNot(logicalOr(a, b)), logicalAnd(logicalNot(a), logicalNot(b)));
}

// Multi-operand chained logical operations
TEST(chained_logical_ops) {
    // true && true && true = true
    ASSERT_TRUE(true && true && true);

    // true && false && true = false
    ASSERT_FALSE(true && false && true);

    // false || false || true = true
    ASSERT_TRUE(false || false || true);

    // Mixed: true && false || true
    // Note: && has higher precedence than ||, so (true && false) || true = true
    ASSERT_TRUE(true && false || true);

    // true || false && false = true (&& first)
    ASSERT_TRUE(true || false && false);
}

// Boundary case tests
TEST(boundary_cases) {
    // Logical operation with itself
    ASSERT_TRUE(true && true);
    ASSERT_FALSE(false && false);
    ASSERT_TRUE(true || true);
    ASSERT_FALSE(false || false);

    // Logical operation with NOT
    ASSERT_FALSE(true && !true);
    ASSERT_TRUE(true || !true);

    // Continuous NOT operations
    ASSERT_TRUE(!false);
    ASSERT_FALSE(!true);
    ASSERT_TRUE(!(!true));
}

// Actual expression scenario tests
TEST(actual_expression_scenarios) {
    // Range check: x >= 0 && x <= 100
    double x = 50.0;
    ASSERT_TRUE(x >= 0.0 && x <= 100.0);

    x = 150.0;
    ASSERT_FALSE(x >= 0.0 && x <= 100.0);

    // Validity check: x != 0 && y != 0
    double val1 = 5.0, val2 = 10.0;
    ASSERT_TRUE(val1 != 0.0 && val2 != 0.0);

    // Default fallback: valid || default_value
    bool valid = false;
    double default_value = 42.0;
    double result = (valid ? 1.0 : default_value);
    ASSERT_EQ(result, default_value);
}

// Short-circuit evaluation side effect avoidance tests
TEST(short_circuit_side_effects) {
    int sideEffectCount = 0;

    auto sideEffect = [&sideEffectCount]() -> bool {
        sideEffectCount++;
        return true;
    };

    // false && anything - side effect occurs only once (for left side evaluation)
    sideEffectCount = 0;
    bool r1 = false && sideEffect();
    ASSERT_EQ(sideEffectCount, 0);  // Right side should not be evaluated

    // true || anything - side effect occurs only once (for left side evaluation)
    sideEffectCount = 0;
    bool r2 = true || sideEffect();
    ASSERT_EQ(sideEffectCount, 0);  // Right side should not be evaluated

    // true && anything - side effect occurs twice
    sideEffectCount = 0;
    bool r3 = sideEffect() && sideEffect();
    ASSERT_EQ(sideEffectCount, 2);  // Both sides should be evaluated
}

// ==================== Main Function ====================

int main() {
    std::cout << "Logical Operator Test Suite" << std::endl;

    std::cout << "[Basic Logical Operator Tests]" << std::endl;
    RUN_TEST(logical_and_basic);
    RUN_TEST(logical_or_basic);
    RUN_TEST(logical_not_basic);

    std::cout << "[Numeric to Boolean Conversion Tests]" << std::endl;
    RUN_TEST(numeric_to_boolean);

    std::cout << "[Short-circuit Evaluation Tests]" << std::endl;
    RUN_TEST(logical_and_short_circuit);
    RUN_TEST(logical_or_short_circuit);
    RUN_TEST(complex_short_circuit);

    std::cout << "[Combination Operation Tests]" << std::endl;
    RUN_TEST(logical_combinations);
    RUN_TEST(chained_logical_ops);

    std::cout << "[Real-world Scenario Tests]" << std::endl;
    RUN_TEST(boundary_cases);
    RUN_TEST(actual_expression_scenarios);
    RUN_TEST(short_circuit_side_effects);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

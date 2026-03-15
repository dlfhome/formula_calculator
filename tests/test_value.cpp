/**
 * Value Type Tests
 * Test multi-type value system (bool, int, double) construction, type checking and conversion
 */

#include <iostream>
#include <cassert>
#include <cmath>

// Mock Value type implementation (replace with real header when implemented)
namespace formula {

enum class ValueType {
    BOOL,
    INT,
    DOUBLE
};

class Value {
public:
    Value() : type_(ValueType::DOUBLE), doubleValue_(0.0) {}
    explicit Value(bool val) : type_(ValueType::BOOL), boolValue_(val) {}
    explicit Value(int val) : type_(ValueType::INT), intValue_(val) {}
    explicit Value(double val) : type_(ValueType::DOUBLE), doubleValue_(val) {}

    ValueType getType() const { return type_; }

    bool isBool() const { return type_ == ValueType::BOOL; }
    bool isInt() const { return type_ == ValueType::INT; }
    bool isDouble() const { return type_ == ValueType::DOUBLE; }

    // 类型转换方法
    bool asBool() const {
        switch (type_) {
            case ValueType::BOOL: return boolValue_;
            case ValueType::INT: return intValue_ != 0;
            case ValueType::DOUBLE: return doubleValue_ != 0.0;
        }
        return false;
    }

    int asInt() const {
        switch (type_) {
            case ValueType::BOOL: return boolValue_ ? 1 : 0;
            case ValueType::INT: return intValue_;
            case ValueType::DOUBLE: return static_cast<int>(doubleValue_);
        }
        return 0;
    }

    double asDouble() const {
        switch (type_) {
            case ValueType::BOOL: return boolValue_ ? 1.0 : 0.0;
            case ValueType::INT: return static_cast<double>(intValue_);
            case ValueType::DOUBLE: return doubleValue_;
        }
        return 0.0;
    }

private:
    ValueType type_;
    bool boolValue_;
    int intValue_;
    double doubleValue_;
};

} // namespace formula

using namespace formula;

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

// ==================== Test Cases ====================

// Bool type construction and checking
TEST(bool_construction) {
    Value v1(true);
    ASSERT_TRUE(v1.isBool());
    ASSERT_FALSE(v1.isInt());
    ASSERT_FALSE(v1.isDouble());
    ASSERT_EQ(v1.getType(), ValueType::BOOL);

    Value v2(false);
    ASSERT_TRUE(v2.isBool());
    ASSERT_FALSE(v2.asBool());
}

// Int type construction and checking
TEST(int_construction) {
    Value v1(42);
    ASSERT_FALSE(v1.isBool());
    ASSERT_TRUE(v1.isInt());
    ASSERT_FALSE(v1.isDouble());
    ASSERT_EQ(v1.getType(), ValueType::INT);

    Value v2(-100);
    ASSERT_TRUE(v2.isInt());
    ASSERT_EQ(v2.asInt(), -100);

    Value v3(0);
    ASSERT_TRUE(v3.isInt());
    ASSERT_EQ(v3.asInt(), 0);
}

// Double type construction and checking
TEST(double_construction) {
    Value v1(3.14159);
    ASSERT_FALSE(v1.isBool());
    ASSERT_FALSE(v1.isInt());
    ASSERT_TRUE(v1.isDouble());
    ASSERT_EQ(v1.getType(), ValueType::DOUBLE);

    Value v2(-2.5);
    ASSERT_TRUE(v2.isDouble());
    ASSERT_NEAR(v2.asDouble(), -2.5, 1e-10);

    Value v3(0.0);
    ASSERT_TRUE(v3.isDouble());
    ASSERT_NEAR(v3.asDouble(), 0.0, 1e-10);
}

// Bool to other type conversions
TEST(bool_conversion) {
    Value v1(true);
    ASSERT_EQ(v1.asBool(), true);
    ASSERT_EQ(v1.asInt(), 1);
    ASSERT_NEAR(v1.asDouble(), 1.0, 1e-10);

    Value v2(false);
    ASSERT_EQ(v2.asBool(), false);
    ASSERT_EQ(v2.asInt(), 0);
    ASSERT_NEAR(v2.asDouble(), 0.0, 1e-10);
}

// Int to other type conversions
TEST(int_conversion) {
    Value v1(42);
    ASSERT_EQ(v1.asBool(), true);  // 非零转换为 true
    ASSERT_EQ(v1.asInt(), 42);
    ASSERT_NEAR(v1.asDouble(), 42.0, 1e-10);

    Value v2(0);
    ASSERT_EQ(v2.asBool(), false);  // 零转换为 false
    ASSERT_EQ(v2.asInt(), 0);
    ASSERT_NEAR(v2.asDouble(), 0.0, 1e-10);

    Value v3(-5);
    ASSERT_EQ(v3.asBool(), true);  // 负数也转换为 true
    ASSERT_EQ(v3.asInt(), -5);
}

// Double to other type conversions
TEST(double_conversion) {
    Value v1(3.7);
    ASSERT_EQ(v1.asBool(), true);
    ASSERT_EQ(v1.asInt(), 3);  // 截断小数部分
    ASSERT_NEAR(v1.asDouble(), 3.7, 1e-10);

    Value v2(0.0);
    ASSERT_EQ(v2.asBool(), false);
    ASSERT_EQ(v2.asInt(), 0);

    Value v3(0.0001);
    ASSERT_EQ(v3.asBool(), true);  // 非零小数转换为 true

    Value v4(-3.9);
    ASSERT_EQ(v4.asInt(), -3);  // 负数截断
}

// Boundary value test
TEST(boundary_values) {
    // Max/min int
    Value v1(INT_MAX);
    ASSERT_EQ(v1.asInt(), INT_MAX);
    ASSERT_TRUE(v1.asBool());

    Value v2(INT_MIN);
    ASSERT_EQ(v2.asInt(), INT_MIN);
    ASSERT_TRUE(v2.asBool());

    // Large/small double
    Value v3(1e308);
    ASSERT_NEAR(v3.asDouble(), 1e308, 1e298);

    Value v4(-1e308);
    ASSERT_NEAR(v4.asDouble(), -1e308, 1e298);

    // Near-zero double
    Value v5(1e-10);
    ASSERT_TRUE(v5.asBool());  // 非零即 true
    ASSERT_EQ(v5.asInt(), 0);  // 转换为 int 后为 0
}

// Type promotion test
TEST(type_promotion) {
    // In actual implementation, type promotion should happen during operations
    // Here we test the various types supported by Value itself

    // bool + int scenario (expect int)
    Value boolVal(true);
    Value intVal(5);
    ASSERT_EQ(boolVal.asInt() + intVal.asInt(), 6);

    // int + double scenario (expect double)
    Value doubleVal(2.5);
    ASSERT_NEAR(intVal.asDouble() + doubleVal.asDouble(), 7.5, 1e-10);

    // bool + double scenario (expect double)
    ASSERT_NEAR(boolVal.asDouble() + doubleVal.asDouble(), 3.5, 1e-10);
}

// Default value test
TEST(default_values) {
    Value v;  // Default construction
    ASSERT_TRUE(v.isDouble());  // 默认应为 double 类型
    ASSERT_NEAR(v.asDouble(), 0.0, 1e-10);
}

// ==================== 主函数 ====================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Value 类型TestSuite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "[Bool Type Tests]" << std::endl;
    RUN_TEST(bool_construction);
    RUN_TEST(bool_conversion);
    std::cout << std::endl;

    std::cout << "[Int Type Tests]" << std::endl;
    RUN_TEST(int_construction);
    RUN_TEST(int_conversion);
    std::cout << std::endl;

    std::cout << "[Double Type Tests]" << std::endl;
    RUN_TEST(double_construction);
    RUN_TEST(double_conversion);
    std::cout << std::endl;

    std::cout << "[Boundary Value Tests]" << std::endl;
    RUN_TEST(boundary_values);
    std::cout << std::endl;

    std::cout << "[Type Promotion Tests]" << std::endl;
    RUN_TEST(type_promotion);
    std::cout << std::endl;

    std::cout << "[Default Value Tests]" << std::endl;
    RUN_TEST(default_values);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "All tests passed!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

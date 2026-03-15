/**
 * Value 类型测试
 * 测试多类型值系统（bool、int、double）的构造、类型检查和转换
 */

#include <iostream>
#include <cassert>
#include <cmath>

// 模拟 Value 类型实现（待实际实现时替换为真实头文件）
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

// ==================== 测试用例 ====================

// Bool 类型构造和检查
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

// Int 类型构造和检查
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

// Double 类型构造和检查
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

// Bool 到其他类型的转换
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

// Int 到其他类型的转换
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

// Double 到其他类型的转换
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

// 边界值测试
TEST(boundary_values) {
    // 最大/最小 int
    Value v1(INT_MAX);
    ASSERT_EQ(v1.asInt(), INT_MAX);
    ASSERT_TRUE(v1.asBool());

    Value v2(INT_MIN);
    ASSERT_EQ(v2.asInt(), INT_MIN);
    ASSERT_TRUE(v2.asBool());

    // 极大/极小 double
    Value v3(1e308);
    ASSERT_NEAR(v3.asDouble(), 1e308, 1e298);

    Value v4(-1e308);
    ASSERT_NEAR(v4.asDouble(), -1e308, 1e298);

    // 接近零的 double
    Value v5(1e-10);
    ASSERT_TRUE(v5.asBool());  // 非零即 true
    ASSERT_EQ(v5.asInt(), 0);  // 转换为 int 后为 0
}

// 类型提升规则测试
TEST(type_promotion) {
    // 在实际实现中，类型提升应该在运算时发生
    // 这里测试 Value 本身支持的各种类型

    // bool + int 场景（预期 int）
    Value boolVal(true);
    Value intVal(5);
    ASSERT_EQ(boolVal.asInt() + intVal.asInt(), 6);

    // int + double 场景（预期 double）
    Value doubleVal(2.5);
    ASSERT_NEAR(intVal.asDouble() + doubleVal.asDouble(), 7.5, 1e-10);

    // bool + double 场景（预期 double）
    ASSERT_NEAR(boolVal.asDouble() + doubleVal.asDouble(), 3.5, 1e-10);
}

// 默认值测试
TEST(default_values) {
    Value v;  // 默认构造
    ASSERT_TRUE(v.isDouble());  // 默认应为 double 类型
    ASSERT_NEAR(v.asDouble(), 0.0, 1e-10);
}

// ==================== 主函数 ====================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Value 类型测试套件" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "【Bool 类型测试】" << std::endl;
    RUN_TEST(bool_construction);
    RUN_TEST(bool_conversion);
    std::cout << std::endl;

    std::cout << "【Int 类型测试】" << std::endl;
    RUN_TEST(int_construction);
    RUN_TEST(int_conversion);
    std::cout << std::endl;

    std::cout << "【Double 类型测试】" << std::endl;
    RUN_TEST(double_construction);
    RUN_TEST(double_conversion);
    std::cout << std::endl;

    std::cout << "【边界值测试】" << std::endl;
    RUN_TEST(boundary_values);
    std::cout << std::endl;

    std::cout << "【类型提升测试】" << std::endl;
    RUN_TEST(type_promotion);
    std::cout << std::endl;

    std::cout << "【默认值测试】" << std::endl;
    RUN_TEST(default_values);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "所有测试通过！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

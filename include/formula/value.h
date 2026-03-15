#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>
#include <iostream>

namespace formula {

/**
 * Value 类型枚举
 */
enum class ValueType {
    BOOL,    // 布尔类型
    INT,     // 整数类型
    DOUBLE   // 浮点类型
};

/**
 * Value 类型转换异常
 */
class ValueTypeException : public std::runtime_error {
public:
    explicit ValueTypeException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * Value 类 - 支持 bool、int64_t、double 三种类型
 * 使用 union 存储实际值，提供类型安全的访问接口
 */
class Value {
public:
    // 默认构造函数，初始化为 double 类型的 0.0
    Value();

    // 从 bool 构造
    Value(bool value);

    // 从 int 构造
    Value(int value);

    // 从 int64_t 构造
    Value(int64_t value);

    // 从 double 构造
    Value(double value);

    // 拷贝构造函数
    Value(const Value& other);

    // 移动构造函数
    Value(Value&& other) noexcept;

    // 拷贝赋值运算符
    Value& operator=(const Value& other);

    // 移动赋值运算符
    Value& operator=(Value&& other) noexcept;

    // 析构函数
    ~Value() = default;

    // 获取类型
    ValueType getType() const { return type_; }

    // 类型检查
    bool isBool() const { return type_ == ValueType::BOOL; }
    bool isInt() const { return type_ == ValueType::INT; }
    bool isDouble() const { return type_ == ValueType::DOUBLE; }

    // 转换为 bool（支持所有类型的转换）
    bool asBool() const;

    // 转换为 int64_t（支持所有类型的转换）
    int64_t asInt() const;

    // 转换为 double（支持所有类型的转换）
    double asDouble() const;

    // 获取原始值（仅当类型匹配时可用）
    bool getBool() const;
    int64_t getInt() const;
    double getDouble() const;

    // 相等比较
    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const;

    // 转换为字符串表示
    std::string toString() const;

    // 流输出操作符
    friend std::ostream& operator<<(std::ostream& os, const Value& value);

private:
    ValueType type_;
    union {
        bool boolValue_;
        int64_t intValue_;
        double doubleValue_;
    };
};

/**
 * 将 ValueType 转换为字符串
 */
const char* valueTypeToString(ValueType type);

} // namespace formula

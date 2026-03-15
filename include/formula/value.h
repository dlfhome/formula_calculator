#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>
#include <iostream>

namespace formula {

/**
 * Value type enumeration
 */
enum class ValueType {
    BOOL,    // Boolean type
    INT,     // Integer type
    DOUBLE   // Floating-point type
};

/**
 * Value type conversion exception
 */
class ValueTypeException : public std::runtime_error {
public:
    explicit ValueTypeException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * Value class - supports bool, int64_t, double types
 * Uses union to store actual value, provides type-safe access interface
 */
class Value {
public:
    // Default constructor, initializes to double 0.0
    Value();

    // Construct from bool
    Value(bool value);

    // Construct from int
    Value(int value);

    // Construct from int64_t
    Value(int64_t value);

    // Construct from double
    Value(double value);

    // Copy constructor
    Value(const Value& other);

    // Move constructor
    Value(Value&& other) noexcept;

    // Copy assignment operator
    Value& operator=(const Value& other);

    // Move assignment operator
    Value& operator=(Value&& other) noexcept;

    // Destructor
    ~Value() = default;

    // Get type
    ValueType getType() const { return type_; }

    // Type checking
    bool isBool() const { return type_ == ValueType::BOOL; }
    bool isInt() const { return type_ == ValueType::INT; }
    bool isDouble() const { return type_ == ValueType::DOUBLE; }

    // Convert to bool (supports conversion from all types)
    bool asBool() const;

    // Convert to int64_t (supports conversion from all types)
    int64_t asInt() const;

    // Convert to double (supports conversion from all types)
    double asDouble() const;

    // Get original value (only available when type matches)
    bool getBool() const;
    int64_t getInt() const;
    double getDouble() const;

    // Equality comparison
    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const;

    // Convert to string representation
    std::string toString() const;

    // Stream output operator
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
 * Convert ValueType to string
 */
const char* valueTypeToString(ValueType type);

} // namespace formula

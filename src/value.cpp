#include "formula/value.h"
#include <sstream>
#include <iomanip>

namespace formula {

Value::Value() : type_(ValueType::DOUBLE), doubleValue_(0.0) {}

Value::Value(bool value) : type_(ValueType::BOOL), boolValue_(value) {}

Value::Value(int value) : type_(ValueType::INT), intValue_(value) {}

Value::Value(int64_t value) : type_(ValueType::INT), intValue_(value) {}

Value::Value(double value) : type_(ValueType::DOUBLE), doubleValue_(value) {}

Value::Value(const Value& other) : type_(other.type_) {
    switch (type_) {
        case ValueType::BOOL:
            boolValue_ = other.boolValue_;
            break;
        case ValueType::INT:
            intValue_ = other.intValue_;
            break;
        case ValueType::DOUBLE:
            doubleValue_ = other.doubleValue_;
            break;
    }
}

Value::Value(Value&& other) noexcept : type_(other.type_) {
    switch (type_) {
        case ValueType::BOOL:
            boolValue_ = other.boolValue_;
            break;
        case ValueType::INT:
            intValue_ = other.intValue_;
            break;
        case ValueType::DOUBLE:
            doubleValue_ = other.doubleValue_;
            break;
    }
}

Value& Value::operator=(const Value& other) {
    if (this != &other) {
        type_ = other.type_;
        switch (type_) {
            case ValueType::BOOL:
                boolValue_ = other.boolValue_;
                break;
            case ValueType::INT:
                intValue_ = other.intValue_;
                break;
            case ValueType::DOUBLE:
                doubleValue_ = other.doubleValue_;
                break;
        }
    }
    return *this;
}

Value& Value::operator=(Value&& other) noexcept {
    if (this != &other) {
        type_ = other.type_;
        switch (type_) {
            case ValueType::BOOL:
                boolValue_ = other.boolValue_;
                break;
            case ValueType::INT:
                intValue_ = other.intValue_;
                break;
            case ValueType::DOUBLE:
                doubleValue_ = other.doubleValue_;
                break;
        }
    }
    return *this;
}

bool Value::asBool() const {
    switch (type_) {
        case ValueType::BOOL:
            return boolValue_;
        case ValueType::INT:
            return intValue_ != 0;
        case ValueType::DOUBLE:
            return doubleValue_ != 0.0;
        default:
            return false;
    }
}

int64_t Value::asInt() const {
    switch (type_) {
        case ValueType::BOOL:
            return boolValue_ ? 1 : 0;
        case ValueType::INT:
            return intValue_;
        case ValueType::DOUBLE:
            return static_cast<int64_t>(doubleValue_);
        default:
            return 0;
    }
}

double Value::asDouble() const {
    switch (type_) {
        case ValueType::BOOL:
            return boolValue_ ? 1.0 : 0.0;
        case ValueType::INT:
            return static_cast<double>(intValue_);
        case ValueType::DOUBLE:
            return doubleValue_;
        default:
            return 0.0;
    }
}

bool Value::getBool() const {
    if (type_ != ValueType::BOOL) {
        throw ValueTypeException("值不是布尔类型");
    }
    return boolValue_;
}

int64_t Value::getInt() const {
    if (type_ != ValueType::INT) {
        throw ValueTypeException("值不是整数类型");
    }
    return intValue_;
}

double Value::getDouble() const {
    if (type_ != ValueType::DOUBLE) {
        throw ValueTypeException("值不是浮点类型");
    }
    return doubleValue_;
}

bool Value::operator==(const Value& other) const {
    if (type_ != other.type_) {
        // 类型不同时，转换为 double 比较
        return asDouble() == other.asDouble();
    }
    switch (type_) {
        case ValueType::BOOL:
            return boolValue_ == other.boolValue_;
        case ValueType::INT:
            return intValue_ == other.intValue_;
        case ValueType::DOUBLE:
            return doubleValue_ == other.doubleValue_;
        default:
            return false;
    }
}

bool Value::operator!=(const Value& other) const {
    return !(*this == other);
}

std::string Value::toString() const {
    std::ostringstream oss;
    switch (type_) {
        case ValueType::BOOL:
            oss << (boolValue_ ? "true" : "false");
            break;
        case ValueType::INT:
            oss << intValue_;
            break;
        case ValueType::DOUBLE:
            oss << std::setprecision(15) << doubleValue_;
            break;
    }
    return oss.str();
}

const char* valueTypeToString(ValueType type) {
    switch (type) {
        case ValueType::BOOL:
            return "BOOL";
        case ValueType::INT:
            return "INT";
        case ValueType::DOUBLE:
            return "DOUBLE";
        default:
            return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, const Value& value) {
    os << value.toString();
    return os;
}

} // namespace formula

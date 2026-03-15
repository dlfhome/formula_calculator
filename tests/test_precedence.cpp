/**
 * Operator Precedence Tests
 * Verify precedence and associativity of operators in complex expressions
 *
 * Expected precedence (from high to low):
 * 1. () Parentheses
 * 2. ! Logical NOT
 * 3. ^ Power
 * 4. *, /, % Multiply/Divide/Modulo
 * 5. +, - Add/Subtract
 * 6. >, <, >=, <= Comparison
 * 7. ==, != Equality
 * 8. && Logical AND
 * 9. || Logical OR
 * 10. ?: Ternary
 */

#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include <functional>
#include <map>

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

// Helper functions: mock expression evaluation
// Note: These functions are used to verify precedence rules

// Logical NOT has highest precedence (among binary operators)
bool logicalNot(bool a) { return !a; }

// Power operation
double power(double base, double exp) { return std::pow(base, exp); }

// Comparison operations
bool greater(double a, double b) { return a > b; }
bool less(double a, double b) { return a < b; }
bool greaterEqual(double a, double b) { return a >= b; }
bool lessEqual(double a, double b) { return a <= b; }
bool equal(double a, double b) { return a == b; }
bool notEqual(double a, double b) { return a != b; }

// Logical operations
bool logicalAnd(bool a, bool b) { return a && b; }
bool logicalOr(bool a, bool b) { return a || b; }

// Ternary operator
double ternary(bool cond, double trueVal, double falseVal) {
    return cond ? trueVal : falseVal;
}

// ==================== Test Cases ====================

// Parentheses precedence tests (highest)
TEST(parentheses) {
    // (2 + 3) * 4 = 20, not 2 + (3 * 4) = 14
    double result = (2.0 + 3.0) * 4.0;
    ASSERT_EQ(result, 20.0);
    
    result = 2.0 + 3.0 * 4.0;
    ASSERT_EQ(result, 14.0);
    
    // Nested parentheses
    result = ((2.0 + 3.0) * (4.0 - 1.0));
    ASSERT_EQ(result, 15.0);
}

// Logical NOT precedence tests
TEST(logical_not) {
    // !a && b should parse as (!a) && b, not !(a && b)
    bool a = true, b = true;
    ASSERT_FALSE(logicalAnd(logicalNot(a), b));
    ASSERT_TRUE(logicalNot(logicalAnd(a, b)) == false);
    
    // !a || b should parse as (!a) || b
    ASSERT_TRUE(logicalOr(logicalNot(a), b));
    
    // ! with comparison: !a > b should be (!a) > b
    // Assume a = true(1), b = 0
    a = true;
    double bVal = 0.0;
    ASSERT_FALSE(greater(static_cast<double>(!a), bVal));
}

// Power operation precedence tests
TEST(power) {
    // 2 ^ 3 ^ 2 = 2 ^ (3 ^ 2) = 2 ^ 9 = 512 (right-associative)
    double result = power(2.0, power(3.0, 2.0));
    ASSERT_EQ(result, 512.0);
    
    // 2 * 3 ^ 2 = 2 * 9 = 18, not (2 * 3) ^ 2 = 36
    result = 2.0 * power(3.0, 2.0);
    ASSERT_EQ(result, 18.0);
    
    // 2 ^ 3 * 4 = 8 * 4 = 32, not 2 ^ (3 * 4)
    result = power(2.0, 3.0) * 4.0;
    ASSERT_EQ(result, 32.0);
}

// Multiply/divide/modulo precedence tests
TEST(multiply_divide) {
    // 2 + 3 * 4 = 14, not (2 + 3) * 4 = 20
    double result = 2.0 + 3.0 * 4.0;
    ASSERT_EQ(result, 14.0);
    
    // 10 - 6 / 2 = 7, not (10 - 6) / 2 = 2
    result = 10.0 - 6.0 / 2.0;
    ASSERT_EQ(result, 7.0);
    
    // 8 / 4 * 2 = 4 (left-associative)
    result = 8.0 / 4.0 * 2.0;
    ASSERT_EQ(result, 4.0);
}

// Add/subtract precedence tests
TEST(add_subtract) {
    // 5 - 3 + 2 = 4 (left-associative)
    double result = 5.0 - 3.0 + 2.0;
    ASSERT_EQ(result, 4.0);
    
    // Mixed with multiply/divide
    result = 2.0 + 3.0 * 4.0 - 5.0;
    ASSERT_EQ(result, 9.0);
}

// Comparison operator precedence tests
TEST(comparison) {
    // Comparison operators have same precedence, left-associative
    // 5 > 3 < 7 = (5 > 3) < 7 = true < 7 = true (numeric conversion)
    bool result = less(greater(5.0, 3.0), 7.0);
    ASSERT_TRUE(result);
    
    // Note: This notation is rare and confusing in practice
}

// Equality operator precedence tests
TEST(equality) {
    // Comparison before equality: 5 > 3 == 4 > 2
    bool result = equal(greater(5.0, 3.0), greater(4.0, 2.0));
    ASSERT_TRUE(result);
}

// Logical AND precedence test
TEST(logical_and) {
    // Comparison before logical AND: 5 > 3 && 4 > 6 = true && false = false
    bool result = logicalAnd(greater(5.0, 3.0), greater(4.0, 6.0));
    ASSERT_FALSE(result);
}

// Logical OR precedence test
TEST(logical_or) {
    // && has higher precedence than ||
    bool result = logicalOr(logicalAnd(true, false), true);
    ASSERT_TRUE(result);
    
    // Complex: (5 > 3 && 4 < 2) || 10 > 5
    result = logicalOr(logicalAnd(greater(5.0, 3.0), less(4.0, 2.0)), greater(10.0, 5.0));
    ASSERT_TRUE(result);
}

// Ternary operator precedence tests (lowest)
TEST(ternary) {
    // || has higher precedence than ?:, so (true || false) ? 1 : 0 = 1
    double result = ternary(logicalOr(true, false), 1.0, 0.0);
    ASSERT_EQ(result, 1.0);
}

// Complex comprehensive expression test
TEST(complex_expression) {
    // Full expression with all operators
    // !a || b && c > d ? e : f
    bool a = false, b = true, c = 5.0, d = 3.0;
    double e = 10.0, f = 20.0;
    
    // Step breakdown:
    // 1. !a = true
    // 2. c > d = true
    // 3. b && (c > d) = true
    // 4. !a || (b && c > d) = true
    // 5. true ? e : f = e = 10.0
    
    // Simplified version test
    double result = ternary(logicalOr(logicalNot(a), logicalAnd(b, greater(c, d))), e, f);
    ASSERT_EQ(result, 10.0);
}

// Mathematical formula precedence test
TEST(mathematical_formula) {
    // Part of quadratic formula: -b + sqrt(b^2 - 4ac) / 2a
    double a = 1.0, b = -5.0, c = 6.0;
    double discriminant = b * b - 4 * a * c;
    double result = (-b + std::sqrt(discriminant)) / (2 * a);
    ASSERT_EQ(result, 3.0);
    
    // Arithmetic sequence sum: n * (a1 + an) / 2
    double n = 10.0, a1 = 1.0, an = 10.0;
    result = n * (a1 + an) / 2.0;
    ASSERT_EQ(result, 55.0);
    
    // Circle area: pi * r^2
    double pi = 3.14159265358979323846;
    double r = 5.0;
    result = pi * power(r, 2.0);
    ASSERT_NEAR(result, 78.53981633974483, 1e-10);
}

// Logical expression precedence test
TEST(logical_expression) {
    // !(a && b) == !a || !b (De Morgan laws)
    bool a = true, b = false;
    ASSERT_EQ(logicalNot(logicalAnd(a, b)), logicalOr(logicalNot(a), logicalNot(b)));
    
    // && has higher precedence than ||
    bool result1 = logicalOr(logicalAnd(true, false), true);
    bool result2 = logicalAnd(true, false) || true;  // same as above
    ASSERT_EQ(result1, result2);
}

// Associativity test
TEST(associativity) {
    // Left-associative: 10 - 5 - 2 = (10-5)-2 = 3, not 10-(5-2) = 7
    ASSERT_EQ(10.0 - 5.0 - 2.0, 3.0);
    
    // Left-associative: 100 / 10 / 2 = (100/10)/2 = 5, not 100/(10/2) = 20
    ASSERT_EQ(100.0 / 10.0 / 2.0, 5.0);
}

// Boundary case test
TEST(boundary) {
    // Unary minus and binary minus
    // -3^2 = -(3^2) = -9 (not (-3)^2 = 9)
    double result = -power(3.0, 2.0);
    ASSERT_EQ(result, -9.0);
    
    // Chained comparison (not recommended but tested)
    // 1 < 3 < 2 = (1 < 3) < 2 = true < 2 = 1 < 2 = true (numeric conversion)
    // Note: This notation has pitfalls in most languages
    
    // Ternary operator nesting
    // a ? b : c ? d : e = a ? b : (c ? d : e) (right-associative)
    double nestedResult = ternary(true, 1.0, ternary(false, 2.0, 3.0));
    ASSERT_EQ(nestedResult, 1.0);
}

// ==================== Main Function ====================

int main() {
    std::cout << "Operator Precedence Test Suite" << std::endl;

    std::cout << "[Basic Precedence Tests]" << std::endl;
    RUN_TEST(parentheses);
    RUN_TEST(logical_not);
    RUN_TEST(power);
    RUN_TEST(multiply_divide);
    RUN_TEST(add_subtract);

    std::cout << "[Comparison and Logical Precedence Tests]" << std::endl;
    RUN_TEST(comparison);
    RUN_TEST(equality);
    RUN_TEST(logical_and);
    RUN_TEST(logical_or);
    RUN_TEST(ternary);

    std::cout << "[Complex Expression Tests]" << std::endl;
    RUN_TEST(complex_expression);
    RUN_TEST(mathematical_formula);
    RUN_TEST(logical_expression);

    std::cout << "[Associativity Tests]" << std::endl;
    RUN_TEST(associativity);
    RUN_TEST(boundary);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

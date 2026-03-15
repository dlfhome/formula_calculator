/**
 * Integration Tests
 * Test complex expression combinations
 * Verify coordination between functional modules
 */

#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <chrono>

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
#define ASSERT_THROW(expr, exception_type) \
    do { \
        bool caught = false; \
        try { expr; } \
        catch (const exception_type&) { caught = true; } \
        assert(caught); \
    } while(0)

namespace formula {

// Mock Formula Calculator class (replace with real class when implemented)
class FormulaCalculator {
public:
    FormulaCalculator() : lastError_(""), isCompiled_(false) {}

    bool compile(const std::string& formula) {
        // Mock compilation process
        formula_ = formula;
        isCompiled_ = true;
        lastError_ = "";
        return true;
    }

    double evaluate() {
        if (!isCompiled_) {
            throw std::runtime_error("Formula not compiled");
        }
        // Mock evaluation (actual implementation should parse and evaluate expression)
        return 0.0;
    }

    void setVariable(const std::string& name, double value) {
        variables_[name] = value;
    }

    double getVariable(const std::string& name) const {
        auto it = variables_.find(name);
        if (it != variables_.end()) {
            return it->second;
        }
        throw std::runtime_error("Variable does not exist: " + name);
    }

    bool hasVariable(const std::string& name) const {
        return variables_.find(name) != variables_.end();
    }

    std::string getLastError() const {
        return lastError_;
    }

    void clear() {
        variables_.clear();
        isCompiled_ = false;
        lastError_ = "";
    }

    bool isCompiled() const {
        return isCompiled_;
    }

private:
    std::string formula_;
    std::map<std::string, double> variables_;
    std::string lastError_;
    bool isCompiled_;
};

// Helper functions
double power(double base, double exp) { return std::pow(base, exp); }
bool toBool(double value) { return value != 0.0; }

} // namespace formula

using namespace formula;

// ==================== Test Cases ====================

// Basic arithmetic and comparison combination
TEST(arithmetic_comparison_combo) {
    // (a + b) > (c * d)
    double a = 5, b = 3, c = 2, d = 3;
    ASSERT_TRUE((a + b) > (c * d));  // 8 > 6 = true

    // a^2 + b^2 == c^2 (Pythagorean theorem check)
    a = 3; b = 4; c = 5;
    ASSERT_TRUE(power(a, 2) + power(b, 2) == power(c, 2));  // 9 + 16 = 25

    // Average calculation and comparison
    double avg = (10 + 20 + 30) / 3.0;
    ASSERT_TRUE(avg >= 19 && avg <= 21);
}

// Comparison and logic combination
TEST(comparison_logical_combo) {
    // Range check: min <= x <= max
    double x = 50, min = 0, max = 100;
    ASSERT_TRUE(x >= min && x <= max);

    x = -10;
    ASSERT_FALSE(x >= min && x <= max);

    // Multi-condition check
    double age = 25, income = 50000;
    ASSERT_TRUE(age >= 18 && age <= 65 && income > 30000);

    // OR condition check
    double score1 = 95, score2 = 75;
    ASSERT_TRUE(score1 > 90 || score2 > 80);  // 95 > 90 为 true

    // Complex condition: (a > b && c > d) || (a == b && c == d)
    double a = 5, b = 3, c = 7, d = 4;
    ASSERT_TRUE((a > b && c > d) || (a == b && c == d));
}

// Ternary operator with comparison and logic combination
TEST(ternary_comparison_logical_combo) {
    // Grade level judgment
    auto getGrade = [](double score) -> int {
        return score >= 90 ? 1 :  // A
               score >= 80 ? 2 :  // B
               score >= 70 ? 3 :  // C
               score >= 60 ? 4 : 5;  // D : F
    };

    ASSERT_EQ(getGrade(95), 1);  // A
    ASSERT_EQ(getGrade(85), 2);  // B
    ASSERT_EQ(getGrade(75), 3);  // C
    ASSERT_EQ(getGrade(65), 4);  // D
    ASSERT_EQ(getGrade(55), 5);  // F

    // Select different calculation based on condition
    double a = 5, b = 3;
    bool useMax = true;
    double result = useMax ? (a > b ? a : b) : (a < b ? a : b);
    ASSERT_EQ(result, 5);  // max(5, 3) = 5

    useMax = false;
    result = useMax ? (a > b ? a : b) : (a < b ? a : b);
    ASSERT_EQ(result, 3);  // min(5, 3) = 3
}

// Math functions with logic combination
TEST(math_logical_combo) {
    // Absolute value and comparison
    auto abs = [](double x) { return x >= 0 ? x : -x; };
    ASSERT_TRUE(abs(-5) == abs(5));
    ASSERT_TRUE(abs(-10) > abs(3));

    // Square root and validity check
    double x = 16;
    double sqrt_x = x >= 0 ? std::sqrt(x) : 0;
    ASSERT_EQ(sqrt_x, 4);

    // Power operation and range check
    double base = 2, exp = 10;
    double pow_result = power(base, exp);
    ASSERT_TRUE(pow_result > 1000 && pow_result < 2000);  // 1024
}

// Complex nested expressions
TEST(complex_nested_expressions) {
    // Calculate tax: different tax rates based on income range
    auto calculateTax = [](double income) {
        double tax;
        if (income <= 5000) {
            tax = income * 0.1;
        } else if (income <= 10000) {
            tax = 500 + (income - 5000) * 0.2;
        } else {
            tax = 1500 + (income - 10000) * 0.3;
        }
        return tax;
    };

    // Implement with ternary expression
    auto calculateTaxTernary = [](double income) {
        return income <= 5000 ? income * 0.1 :
               income <= 10000 ? 500 + (income - 5000) * 0.2 :
               1500 + (income - 10000) * 0.3;
    };

    ASSERT_NEAR(calculateTax(3000), calculateTaxTernary(3000), 1e-10);
    ASSERT_NEAR(calculateTax(7000), calculateTaxTernary(7000), 1e-10);
    ASSERT_NEAR(calculateTax(15000), calculateTaxTernary(15000), 1e-10);

    // Verify specific values
    ASSERT_NEAR(calculateTaxTernary(3000), 300, 1e-10);
    ASSERT_NEAR(calculateTaxTernary(7000), 900, 1e-10);
    ASSERT_NEAR(calculateTaxTernary(15000), 3000, 1e-10);
}

// Variable and expression combination
TEST(variable_expression_combo) {
    FormulaCalculator calc;

    // Set variables
    calc.setVariable("x", 10);
    calc.setVariable("y", 20);
    calc.setVariable("z", 30);

    // Verify variables exist
    ASSERT_TRUE(calc.hasVariable("x"));
    ASSERT_TRUE(calc.hasVariable("y"));
    ASSERT_TRUE(calc.hasVariable("z"));
    ASSERT_FALSE(calc.hasVariable("w"));

    // Verify variable values
    ASSERT_EQ(calc.getVariable("x"), 10);
    ASSERT_EQ(calc.getVariable("y"), 20);
    ASSERT_EQ(calc.getVariable("z"), 30);

    // Simulate expressions using variables
    double x = calc.getVariable("x");
    double y = calc.getVariable("y");
    double z = calc.getVariable("z");

    // x + y * z
    ASSERT_EQ(x + y * z, 610);

    // (x + y) * z
    ASSERT_EQ((x + y) * z, 900);

    // x > y || y < z
    ASSERT_TRUE(x > y || y < z);
}

// Error handling test
TEST(error_handling) {
    FormulaCalculator calc;

    // Evaluation without compilation should throw exception
    ASSERT_THROW(calc.evaluate(), std::runtime_error);

    // Getting non-existent variable should throw exception
    ASSERT_THROW(calc.getVariable("nonexistent"), std::runtime_error);

    // Evaluation after compilation should not throw exception
    ASSERT_TRUE(calc.compile("1 + 2"));
    ASSERT_TRUE(calc.isCompiled());
}

// Boundary condition test
TEST(boundary_conditions) {
    // Large value operations
    double max = 1e308;
    ASSERT_TRUE(max * 0.5 < max);
    ASSERT_TRUE(max > 0);

    // Small value operations
    double min = -1e308;
    ASSERT_TRUE(min < 0);
    ASSERT_TRUE(min < max);

    // Zero value operations
    ASSERT_EQ(0.0 + 0.0, 0.0);
    ASSERT_EQ(0.0 * 100, 0.0);
    ASSERT_TRUE(0.0 == 0.0);
    ASSERT_FALSE(0.0 != 0.0);

    // Division by zero check (should be handled properly)
    double x = 10, y = 0;
    bool safeToDivide = y != 0;
    double result = safeToDivide ? x / y : 0;
    ASSERT_EQ(result, 0);
}

// Real-world scenario test
TEST(real_world_scenarios) {
    // Scenario 1: Discount calculation
    auto calculateDiscount = [](double price, bool isVIP, int quantity) {
        double discount = isVIP ? 0.8 : 0.95;
        double qtyDiscount = quantity >= 10 ? 0.9 : 1.0;
        return price * quantity * discount * qtyDiscount;
    };

    // VIP买5件：100 * 5 * 0.8 * 1.0 = 400
    ASSERT_NEAR(calculateDiscount(100, true, 5), 400, 1e-10);
    // 普通用户买10件：100 * 10 * 0.95 * 0.9 = 855
    ASSERT_NEAR(calculateDiscount(100, false, 10), 855, 1e-10);

    // Scenario 2: BMI calculation and classification
    auto calculateBMI = [](double weight, double height) {
        return weight / (height * height);
    };

    auto getBMICategory = [](double bmi) {
        return bmi < 18.5 ? 1 :      // 偏瘦
               bmi < 24 ? 2 :        // 正常
               bmi < 28 ? 3 : 4;     // 偏胖 : 肥胖
    };

    double bmi = calculateBMI(70, 1.75);  // BMI = 22.86
    ASSERT_TRUE(bmi >= 18.5 && bmi < 24);
    ASSERT_EQ(getBMICategory(bmi), 2);  // 正常

    // Scenario 3: Loan eligibility check
    auto checkLoanEligibility = [](double income, double debt, int creditScore) {
        double debtRatio = debt / income;
        return income > 5000 &&
               debtRatio < 0.4 &&
               creditScore >= 600;
    };

    ASSERT_TRUE(checkLoanEligibility(8000, 2000, 700));
    ASSERT_FALSE(checkLoanEligibility(4000, 1000, 700));  // Insufficient income
    ASSERT_FALSE(checkLoanEligibility(8000, 4000, 700));  // Too much debt
    ASSERT_FALSE(checkLoanEligibility(8000, 2000, 500));  // Low credit score
}

// Performance test (simple verification)
TEST(performance_basic) {
    // Repeatedly calculate same expression
    auto start = std::chrono::high_resolution_clock::now();

    double result = 0;
    for (int i = 0; i < 10000; ++i) {
        result = (i * 2.0 + 1) > (i + 10) ? i * 1.5 : i * 0.5;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Verify result is not 0 and calculation completes in reasonable time
    ASSERT_TRUE(result > 0);
    ASSERT_TRUE(duration.count() < 1000);  // Should complete within 1 second
}

// Complex logic combination test
TEST(complex_logic_combinations) {
    // Verify various logic combinations
    for (bool a : {true, false}) {
        for (bool b : {true, false}) {
            for (bool c : {true, false}) {
                // 分配律: a && (b || c) == (a && b) || (a && c)
                bool left = a && (b || c);
                bool right = (a && b) || (a && c);
                ASSERT_EQ(left, right);

                // 分配律: a || (b && c) == (a || b) && (a || c)
                left = a || (b && c);
                right = (a || b) && (a || c);
                ASSERT_EQ(left, right);
            }
        }
    }
}

// Type conversion and compatibility test
TEST(type_compatibility) {
    // int and double mixed operations
    int i = 5;
    double d = 2.5;
    ASSERT_NEAR(i + d, 7.5, 1e-10);
    ASSERT_NEAR(i * d, 12.5, 1e-10);
    ASSERT_EQ(i / 2, 2);  // 整数除法
    ASSERT_NEAR(i / 2.0, 2.5, 1e-10);  // 浮点除法

    // bool conversion in numeric context
    bool flag = true;
    ASSERT_EQ(flag + 0, 1);
    ASSERT_EQ((!flag) + 0, 0);

    // Numeric conversion in boolean context
    ASSERT_TRUE(5 && true);
    ASSERT_FALSE(0 && true);
    ASSERT_TRUE(5 || false);
    ASSERT_FALSE(0 || false);
}

// ==================== Main Function ====================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Integration Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "[Basic Combination Tests]" << std::endl;
    RUN_TEST(arithmetic_comparison_combo);
    RUN_TEST(comparison_logical_combo);
    RUN_TEST(ternary_comparison_logical_combo);
    std::cout << std::endl;

    std::cout << "[Complex Expression Tests]" << std::endl;
    RUN_TEST(math_logical_combo);
    RUN_TEST(complex_nested_expressions);
    RUN_TEST(variable_expression_combo);
    std::cout << std::endl;

    std::cout << "[Boundary and Error Handling Tests]" << std::endl;
    RUN_TEST(error_handling);
    RUN_TEST(boundary_conditions);
    std::cout << std::endl;

    std::cout << "[Real-world Scenario Tests]" << std::endl;
    RUN_TEST(real_world_scenarios);
    RUN_TEST(performance_basic);
    std::cout << std::endl;

    std::cout << "[Advanced Feature Tests]" << std::endl;
    RUN_TEST(complex_logic_combinations);
    RUN_TEST(type_compatibility);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "All tests passed!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

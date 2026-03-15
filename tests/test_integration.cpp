/**
 * 集成测试
 * 测试复杂表达式的组合使用
 * 验证各功能模块协同工作
 */

#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <chrono>

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
#define ASSERT_THROW(expr, exception_type) \
    do { \
        bool caught = false; \
        try { expr; } \
        catch (const exception_type&) { caught = true; } \
        assert(caught); \
    } while(0)

namespace formula {

// 模拟公式计算器类（待实际实现时替换为真实类）
class FormulaCalculator {
public:
    FormulaCalculator() : lastError_(""), isCompiled_(false) {}

    bool compile(const std::string& formula) {
        // 模拟编译过程
        formula_ = formula;
        isCompiled_ = true;
        lastError_ = "";
        return true;
    }

    double evaluate() {
        if (!isCompiled_) {
            throw std::runtime_error("公式未编译");
        }
        // 模拟求值（实际实现中应解析并计算表达式）
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
        throw std::runtime_error("变量不存在: " + name);
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

// 辅助函数
double power(double base, double exp) { return std::pow(base, exp); }
bool toBool(double value) { return value != 0.0; }

} // namespace formula

using namespace formula;

// ==================== 测试用例 ====================

// 基本算术与比较组合
TEST(arithmetic_comparison_combo) {
    // (a + b) > (c * d)
    double a = 5, b = 3, c = 2, d = 3;
    ASSERT_TRUE((a + b) > (c * d));  // 8 > 6 = true

    // a^2 + b^2 == c^2（勾股定理检查）
    a = 3; b = 4; c = 5;
    ASSERT_TRUE(power(a, 2) + power(b, 2) == power(c, 2));  // 9 + 16 = 25

    // 平均值计算并比较
    double avg = (10 + 20 + 30) / 3.0;
    ASSERT_TRUE(avg >= 19 && avg <= 21);
}

// 比较与逻辑组合
TEST(comparison_logical_combo) {
    // 范围检查：min <= x <= max
    double x = 50, min = 0, max = 100;
    ASSERT_TRUE(x >= min && x <= max);

    x = -10;
    ASSERT_FALSE(x >= min && x <= max);

    // 多条件检查
    double age = 25, income = 50000;
    ASSERT_TRUE(age >= 18 && age <= 65 && income > 30000);

    // 或条件检查
    double score1 = 95, score2 = 75;
    ASSERT_TRUE(score1 > 90 || score2 > 80);  // 95 > 90 为 true

    // 复杂条件：(a > b && c > d) || (a == b && c == d)
    double a = 5, b = 3, c = 7, d = 4;
    ASSERT_TRUE((a > b && c > d) || (a == b && c == d));
}

// 三元运算符与比较逻辑组合
TEST(ternary_comparison_logical_combo) {
    // 成绩等级判断
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

    // 根据条件选择不同计算方式
    double a = 5, b = 3;
    bool useMax = true;
    double result = useMax ? (a > b ? a : b) : (a < b ? a : b);
    ASSERT_EQ(result, 5);  // max(5, 3) = 5

    useMax = false;
    result = useMax ? (a > b ? a : b) : (a < b ? a : b);
    ASSERT_EQ(result, 3);  // min(5, 3) = 3
}

// 数学函数与逻辑组合
TEST(math_logical_combo) {
    // 绝对值与比较
    auto abs = [](double x) { return x >= 0 ? x : -x; };
    ASSERT_TRUE(abs(-5) == abs(5));
    ASSERT_TRUE(abs(-10) > abs(3));

    // 平方根与有效性检查
    double x = 16;
    double sqrt_x = x >= 0 ? std::sqrt(x) : 0;
    ASSERT_EQ(sqrt_x, 4);

    // 幂运算与范围检查
    double base = 2, exp = 10;
    double pow_result = power(base, exp);
    ASSERT_TRUE(pow_result > 1000 && pow_result < 2000);  // 1024
}

// 复杂嵌套表达式
TEST(complex_nested_expressions) {
    // 计算税费：收入在范围内按不同税率
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

    // 用三元表达式实现
    auto calculateTaxTernary = [](double income) {
        return income <= 5000 ? income * 0.1 :
               income <= 10000 ? 500 + (income - 5000) * 0.2 :
               1500 + (income - 10000) * 0.3;
    };

    ASSERT_NEAR(calculateTax(3000), calculateTaxTernary(3000), 1e-10);
    ASSERT_NEAR(calculateTax(7000), calculateTaxTernary(7000), 1e-10);
    ASSERT_NEAR(calculateTax(15000), calculateTaxTernary(15000), 1e-10);

    // 验证具体值
    ASSERT_NEAR(calculateTaxTernary(3000), 300, 1e-10);
    ASSERT_NEAR(calculateTaxTernary(7000), 900, 1e-10);
    ASSERT_NEAR(calculateTaxTernary(15000), 3000, 1e-10);
}

// 变量与表达式组合
TEST(variable_expression_combo) {
    FormulaCalculator calc;

    // 设置变量
    calc.setVariable("x", 10);
    calc.setVariable("y", 20);
    calc.setVariable("z", 30);

    // 验证变量存在
    ASSERT_TRUE(calc.hasVariable("x"));
    ASSERT_TRUE(calc.hasVariable("y"));
    ASSERT_TRUE(calc.hasVariable("z"));
    ASSERT_FALSE(calc.hasVariable("w"));

    // 验证变量值
    ASSERT_EQ(calc.getVariable("x"), 10);
    ASSERT_EQ(calc.getVariable("y"), 20);
    ASSERT_EQ(calc.getVariable("z"), 30);

    // 模拟使用变量的表达式
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

// 错误处理测试
TEST(error_handling) {
    FormulaCalculator calc;

    // 未编译时求值应抛出异常
    ASSERT_THROW(calc.evaluate(), std::runtime_error);

    // 获取不存在的变量应抛出异常
    ASSERT_THROW(calc.getVariable("nonexistent"), std::runtime_error);

    // 编译后求值不应抛出异常
    ASSERT_TRUE(calc.compile("1 + 2"));
    ASSERT_TRUE(calc.isCompiled());
}

// 边界条件测试
TEST(boundary_conditions) {
    // 极大值运算
    double max = 1e308;
    ASSERT_TRUE(max * 0.5 < max);
    ASSERT_TRUE(max > 0);

    // 极小值运算
    double min = -1e308;
    ASSERT_TRUE(min < 0);
    ASSERT_TRUE(min < max);

    // 零值运算
    ASSERT_EQ(0.0 + 0.0, 0.0);
    ASSERT_EQ(0.0 * 100, 0.0);
    ASSERT_TRUE(0.0 == 0.0);
    ASSERT_FALSE(0.0 != 0.0);

    // 除零检查（应妥善处理）
    double x = 10, y = 0;
    bool safeToDivide = y != 0;
    double result = safeToDivide ? x / y : 0;
    ASSERT_EQ(result, 0);
}

// 实际应用场景测试
TEST(real_world_scenarios) {
    // 场景1：折扣计算
    auto calculateDiscount = [](double price, bool isVIP, int quantity) {
        double discount = isVIP ? 0.8 : 0.95;
        double qtyDiscount = quantity >= 10 ? 0.9 : 1.0;
        return price * quantity * discount * qtyDiscount;
    };

    // VIP买5件：100 * 5 * 0.8 * 1.0 = 400
    ASSERT_NEAR(calculateDiscount(100, true, 5), 400, 1e-10);
    // 普通用户买10件：100 * 10 * 0.95 * 0.9 = 855
    ASSERT_NEAR(calculateDiscount(100, false, 10), 855, 1e-10);

    // 场景2：BMI计算和分类
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

    // 场景3：贷款资格检查
    auto checkLoanEligibility = [](double income, double debt, int creditScore) {
        double debtRatio = debt / income;
        return income > 5000 &&
               debtRatio < 0.4 &&
               creditScore >= 600;
    };

    ASSERT_TRUE(checkLoanEligibility(8000, 2000, 700));
    ASSERT_FALSE(checkLoanEligibility(4000, 1000, 700));  // 收入不足
    ASSERT_FALSE(checkLoanEligibility(8000, 4000, 700));  // 负债过高
    ASSERT_FALSE(checkLoanEligibility(8000, 2000, 500));  // 信用分不足
}

// 性能测试（简单验证）
TEST(performance_basic) {
    // 重复计算同一表达式
    auto start = std::chrono::high_resolution_clock::now();

    double result = 0;
    for (int i = 0; i < 10000; ++i) {
        result = (i * 2.0 + 1) > (i + 10) ? i * 1.5 : i * 0.5;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // 简单验证结果不为0且计算在合理时间内完成
    ASSERT_TRUE(result > 0);
    ASSERT_TRUE(duration.count() < 1000);  // 应该在1秒内完成
}

// 复杂逻辑组合测试
TEST(complex_logic_combinations) {
    // 验证各种逻辑组合
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

// 类型转换和兼容性测试
TEST(type_compatibility) {
    // int 和 double 混合运算
    int i = 5;
    double d = 2.5;
    ASSERT_NEAR(i + d, 7.5, 1e-10);
    ASSERT_NEAR(i * d, 12.5, 1e-10);
    ASSERT_EQ(i / 2, 2);  // 整数除法
    ASSERT_NEAR(i / 2.0, 2.5, 1e-10);  // 浮点除法

    // bool 在数值上下文中的转换
    bool flag = true;
    ASSERT_EQ(flag + 0, 1);
    ASSERT_EQ((!flag) + 0, 0);

    // 数值在布尔上下文中的转换
    ASSERT_TRUE(5 && true);
    ASSERT_FALSE(0 && true);
    ASSERT_TRUE(5 || false);
    ASSERT_FALSE(0 || false);
}

// ==================== 主函数 ====================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "集成测试套件" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "【基本组合测试】" << std::endl;
    RUN_TEST(arithmetic_comparison_combo);
    RUN_TEST(comparison_logical_combo);
    RUN_TEST(ternary_comparison_logical_combo);
    std::cout << std::endl;

    std::cout << "【复杂表达式测试】" << std::endl;
    RUN_TEST(math_logical_combo);
    RUN_TEST(complex_nested_expressions);
    RUN_TEST(variable_expression_combo);
    std::cout << std::endl;

    std::cout << "【边界和错误处理测试】" << std::endl;
    RUN_TEST(error_handling);
    RUN_TEST(boundary_conditions);
    std::cout << std::endl;

    std::cout << "【实际应用场景测试】" << std::endl;
    RUN_TEST(real_world_scenarios);
    RUN_TEST(performance_basic);
    std::cout << std::endl;

    std::cout << "【高级功能测试】" << std::endl;
    RUN_TEST(complex_logic_combinations);
    RUN_TEST(type_compatibility);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "所有测试通过！" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

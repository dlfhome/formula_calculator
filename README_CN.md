# Formula Calculator

一个功能强大的 C++ 数学公式计算库，支持表达式编译、求值、变量管理和自定义函数。

## 功能特性

- **表达式编译与求值**: 解析并计算数学公式表达式
- **完整运算符支持**: 支持 +、-、*、/、^（幂运算）等算术运算符
- **比较与逻辑运算**: 支持 >、<、>=、<=、==、!=、&&、||、! 等运算符
- **三元运算符**: 支持条件表达式 `condition ? true_value : false_value`
- **短路求值**: 逻辑运算支持短路求值，提高性能
- **变量管理**: 支持设置、获取和管理变量
- **内置数学函数**: 提供丰富的数学函数（sin、cos、log、sqrt 等）
- **内置常量**: 支持 PI、E 等常用数学常量
- **自定义函数**: 支持注册用户自定义函数
- **类型系统**: 基于 Value 类型的灵活数值系统
- **跨平台**: 支持 Windows、Linux 和 macOS

## 项目结构

```
formula_calculator/
├── include/formula/    # 头文件目录
│   ├── formula_calculator.h   # 主接口类
│   ├── value.h                # Value 类型定义
│   ├── lexer.h                # 词法分析器
│   ├── parser.h               # 语法分析器
│   ├── evaluator.h            # 表达式求值器
│   ├── variable_manager.h     # 变量管理
│   ├── function_registry.h    # 函数注册表
│   ├── constant_registry.h    # 常量注册表
│   └── ast.h                  # 抽象语法树定义
├── src/                # 源文件目录
├── examples/           # 示例程序
├── tests/              # 单元测试
├── docs/               # 文档
└── CMakeLists.txt      # CMake 构建配置
```

## 构建要求

- CMake 3.14 或更高版本
- C++17 兼容的编译器（GCC、Clang、MSVC）
- POSIX 线程支持

## 构建步骤

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 编译
cmake --build .

# 运行测试
ctest
```

### 构建选项

| 选项 | 说明 | 默认值 |
|------|------|--------|
| `BUILD_TESTS` | 构建单元测试 | ON |
| `BUILD_EXAMPLES` | 构建示例程序 | ON |
| `CMAKE_BUILD_TYPE` | 构建类型（Debug/Release） | Release |

## 使用示例

### 基础用法

```cpp
#include "formula/formula_calculator.h"
#include <iostream>

using namespace formula;

int main() {
    FormulaCalculator calc;

    // 编译并计算简单表达式
    if (calc.compile("2 + 3 * 4")) {
        Value result = calc.evaluate();
        std::cout << "结果: " << result << std::endl;  // 输出: 14
    }

    // 使用变量
    calc.clear();
    if (calc.compile("x * x + y * y")) {
        calc.setVariable("x", 3.0);
        calc.setVariable("y", 4.0);
        Value result = calc.evaluate();
        std::cout << "结果: " << result << std::endl;  // 输出: 25
    }

    return 0;
}
```

### 使用数学函数

```cpp
FormulaCalculator calc;

if (calc.compile("sin(PI/2) + cos(0)")) {
    Value result = calc.evaluate();
    std::cout << "结果: " << result << std::endl;  // 输出: 2
}
```

### 条件表达式

```cpp
FormulaCalculator calc;

if (calc.compile("x > 0 ? x : -x")) {
    calc.setVariable("x", -5.0);
    Value result = calc.evaluate();
    std::cout << "绝对值: " << result << std::endl;  // 输出: 5
}
```

### 注册自定义函数

```cpp
FormulaCalculator calc;

// 注册自定义函数
calc.registerFunction("double", [](const std::vector<Value>& args) -> Value {
    return args[0].asDouble() * 2;
}, 1);

if (calc.compile("double(21)")) {
    Value result = calc.evaluate();
    std::cout << "结果: " << result << std::endl;  // 输出: 42
}
```

## 支持的运算符

| 优先级 | 运算符 | 说明 | 结合性 |
|--------|--------|------|--------|
| 1 | `()` | 括号 | - |
| 2 | `!` | 逻辑非 | 右结合 |
| 3 | `^` | 幂运算 | 右结合 |
| 4 | `*` `/` `%` | 乘除、取模 | 左结合 |
| 5 | `+` `-` | 加减 | 左结合 |
| 6 | `>` `<` `>=` `<=` | 比较运算 | 左结合 |
| 7 | `==` `!=` | 相等性判断 | 左结合 |
| 8 | `&&` | 逻辑与 | 左结合 |
| 9 | `\|\|` | 逻辑或 | 左结合 |
| 10 | `?:` | 三元条件 | 右结合 |

## 内置函数

### 数学函数

- `sin(x)` - 正弦函数
- `cos(x)` - 余弦函数
- `tan(x)` - 正切函数
- `asin(x)` - 反正弦函数
- `acos(x)` - 反余弦函数
- `atan(x)` - 反正切函数
- `sqrt(x)` - 平方根
- `log(x)` - 自然对数
- `log10(x)` - 常用对数
- `exp(x)` - 指数函数 e^x
- `abs(x)` - 绝对值
- `floor(x)` - 向下取整
- `ceil(x)` - 向上取整
- `round(x)` - 四舍五入
- `min(a, b)` - 最小值
- `max(a, b)` - 最大值
- `pow(x, y)` - 幂函数 x^y

## 内置常量

- `PI` - 圆周率 π (3.14159...)
- `E` - 自然对数的底 e (2.71828...)

## API 参考

### FormulaCalculator 类

```cpp
class FormulaCalculator {
public:
    // 编译公式
    bool compile(const std::string& formula);

    // 计算表达式结果
    Value evaluate();

    // 变量管理
    void setVariable(const std::string& name, const Value& value);
    Value getVariable(const std::string& name) const;
    bool hasVariable(const std::string& name) const;

    // 自定义函数注册
    bool registerFunction(const std::string& name, FunctionPtr func, int arity = -1);

    // 错误处理
    std::string getLastError() const;

    // 状态管理
    void clear();
    bool isCompiled() const;

    // 获取支持的函数和常量列表
    std::vector<std::string> getSupportedFunctions() const;
    std::vector<std::string> getSupportedConstants() const;
};
```

### Value 类

```cpp
class Value {
public:
    Value();                          // 创建 nil 值
    Value(double value);              // 从 double 创建
    Value(bool value);                // 从 bool 创建
    Value(int value);                 // 从 int 创建

    double asDouble() const;          // 转换为 double
    bool asBool() const;              // 转换为 bool
    bool isNil() const;               // 检查是否为 nil
};
```

## 运行测试

```bash
cd build
ctest --output-on-failure
```

测试包括：
- Value 类型测试
- 运算符优先级测试
- 比较运算测试
- 逻辑运算测试
- 三元运算符测试
- 集成测试

## 运行示例

```bash
cd build
./examples/basic_usage       # 基础使用示例
./examples/advanced_usage    # 高级功能示例
./examples/performance_test  # 性能测试
```

## 许可证

本项目采用 MIT 许可证。

## 贡献

欢迎提交 Issue 和 Pull Request。

## 更新日志

### v1.0.0
- 新增 Value 系统，支持多类型数值
- 新增比较运算和逻辑运算
- 新增三元运算符
- 新增短路求值
- 支持自定义函数注册
- 完善测试覆盖

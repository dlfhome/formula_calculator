# Formula Calculator

A powerful C++ mathematical formula calculation library with expression compilation, evaluation, variable management, and custom function support.

## Features

- **Expression Compilation & Evaluation**: Parse and compute mathematical formula expressions
- **Complete Operator Support**: Supports +, -, *, /, ^ (power) and other arithmetic operators
- **Comparison & Logical Operations**: Supports >, <, >=, <=, ==, !=, &&, ||, ! operators
- **Ternary Operator**: Supports conditional expressions `condition ? true_value : false_value`
- **Short-Circuit Evaluation**: Logical operations support short-circuit evaluation for better performance
- **Variable Management**: Support for setting, getting, and managing variables
- **Built-in Math Functions**: Rich set of mathematical functions (sin, cos, log, sqrt, etc.)
- **Built-in Constants**: Support for common mathematical constants like PI, E
- **Custom Functions**: Support for registering user-defined functions
- **Type System**: Flexible Value-based numerical type system
- **Cross-Platform**: Supports Windows, Linux, and macOS

## Project Structure

```
formula_calculator/
├── include/formula/    # Header files
│   ├── formula_calculator.h   # Main interface class
│   ├── value.h                # Value type definition
│   ├── lexer.h                # Lexical analyzer
│   ├── parser.h               # Syntax parser
│   ├── evaluator.h            # Expression evaluator
│   ├── variable_manager.h     # Variable management
│   ├── function_registry.h    # Function registry
│   ├── constant_registry.h    # Constant registry
│   └── ast.h                  # Abstract syntax tree definition
├── src/                # Source files
├── examples/           # Example programs
├── tests/              # Unit tests
├── docs/               # Documentation
└── CMakeLists.txt      # CMake build configuration
```

## Build Requirements

- CMake 3.14 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- POSIX thread support

## Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure project
cmake ..

# Build
cmake --build .

# Run tests
ctest
```

### Build Options

| Option | Description | Default |
|--------|-------------|---------|
| `BUILD_TESTS` | Build unit tests | ON |
| `BUILD_EXAMPLES` | Build example programs | ON |
| `CMAKE_BUILD_TYPE` | Build type (Debug/Release) | Release |

## Usage Examples

### Basic Usage

```cpp
#include "formula/formula_calculator.h"
#include <iostream>

using namespace formula;

int main() {
    FormulaCalculator calc;

    // Compile and evaluate simple expression
    if (calc.compile("2 + 3 * 4")) {
        Value result = calc.evaluate();
        std::cout << "Result: " << result << std::endl;  // Output: 14
    }

    // Using variables
    calc.clear();
    if (calc.compile("x * x + y * y")) {
        calc.setVariable("x", 3.0);
        calc.setVariable("y", 4.0);
        Value result = calc.evaluate();
        std::cout << "Result: " << result << std::endl;  // Output: 25
    }

    return 0;
}
```

### Using Math Functions

```cpp
FormulaCalculator calc;

if (calc.compile("sin(PI/2) + cos(0)")) {
    Value result = calc.evaluate();
    std::cout << "Result: " << result << std::endl;  // Output: 2
}
```

### Conditional Expressions

```cpp
FormulaCalculator calc;

if (calc.compile("x > 0 ? x : -x")) {
    calc.setVariable("x", -5.0);
    Value result = calc.evaluate();
    std::cout << "Absolute value: " << result << std::endl;  // Output: 5
}
```

### Registering Custom Functions

```cpp
FormulaCalculator calc;

// Register custom function
calc.registerFunction("double", [](const std::vector<Value>& args) -> Value {
    return args[0].asDouble() * 2;
}, 1);

if (calc.compile("double(21)")) {
    Value result = calc.evaluate();
    std::cout << "Result: " << result << std::endl;  // Output: 42
}
```

## Supported Operators

| Precedence | Operator | Description | Associativity |
|------------|----------|-------------|---------------|
| 1 | `()` | Parentheses | - |
| 2 | `!` | Logical NOT | Right |
| 3 | `^` | Power | Right |
| 4 | `*` `/` `%` | Multiply, Divide, Modulo | Left |
| 5 | `+` `-` | Add, Subtract | Left |
| 6 | `>` `<` `>=` `<=` | Comparison | Left |
| 7 | `==` `!=` | Equality | Left |
| 8 | `&&` | Logical AND | Left |
| 9 | `||` | Logical OR | Left |
| 10 | `?:` | Ternary conditional | Right |

## Built-in Functions

### Math Functions

- `sin(x)` - Sine function
- `cos(x)` - Cosine function
- `tan(x)` - Tangent function
- `asin(x)` - Arcsine function
- `acos(x)` - Arccosine function
- `atan(x)` - Arctangent function
- `sqrt(x)` - Square root
- `log(x)` - Natural logarithm
- `log10(x)` - Common logarithm
- `exp(x)` - Exponential function e^x
- `abs(x)` - Absolute value
- `floor(x)` - Floor
- `ceil(x)` - Ceiling
- `round(x)` - Round
- `min(a, b)` - Minimum
- `max(a, b)` - Maximum
- `pow(x, y)` - Power function x^y

## Built-in Constants

- `PI` - Pi (3.14159...)
- `E` - Euler's number (2.71828...)

## API Reference

### FormulaCalculator Class

```cpp
class FormulaCalculator {
public:
    // Compile formula
    bool compile(const std::string& formula);

    // Evaluate expression
    Value evaluate();

    // Variable management
    void setVariable(const std::string& name, const Value& value);
    Value getVariable(const std::string& name) const;
    bool hasVariable(const std::string& name) const;

    // Custom function registration
    bool registerFunction(const std::string& name, FunctionPtr func, int arity = -1);

    // Error handling
    std::string getLastError() const;

    // State management
    void clear();
    bool isCompiled() const;

    // Get supported functions and constants
    std::vector<std::string> getSupportedFunctions() const;
    std::vector<std::string> getSupportedConstants() const;
};
```

### Value Class

```cpp
class Value {
public:
    Value();                          // Create nil value
    Value(double value);              // Create from double
    Value(bool value);                // Create from bool
    Value(int value);                 // Create from int

    double asDouble() const;          // Convert to double
    bool asBool() const;              // Convert to bool
    bool isNil() const;               // Check if nil
};
```

## Running Tests

```bash
cd build
ctest --output-on-failure
```

Tests include:
- Value type tests
- Operator precedence tests
- Comparison operation tests
- Logical operation tests
- Ternary operator tests
- Integration tests

## Running Examples

```bash
cd build
./examples/basic_usage       # Basic usage example
./examples/advanced_usage    # Advanced features example
./examples/performance_test  # Performance test
```

## License

This project is licensed under the MIT License.

## Contributing

Issues and Pull Requests are welcome.

## Changelog

### v1.0.0
- Added Value system with multi-type numeric support
- Added comparison and logical operations
- Added ternary operator
- Added short-circuit evaluation
- Support for custom function registration
- Improved test coverage

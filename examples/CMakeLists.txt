# Examples CMakeLists.txt

# Basic usage example
add_executable(basic_usage basic_usage.cpp)
target_link_libraries(basic_usage formula_calculator_static)

# Advanced usage example
add_executable(advanced_usage advanced_usage.cpp)
target_link_libraries(advanced_usage formula_calculator_static)

# Performance test
add_executable(performance_test performance_test.cpp)
target_link_libraries(performance_test formula_calculator_static)

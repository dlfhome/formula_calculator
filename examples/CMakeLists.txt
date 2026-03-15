# 示例程序 CMakeLists.txt

# 基础使用示例
add_executable(basic_usage basic_usage.cpp)
target_link_libraries(basic_usage formula_calculator_static)

# 高级使用示例
add_executable(advanced_usage advanced_usage.cpp)
target_link_libraries(advanced_usage formula_calculator_static)

# 性能测试
add_executable(performance_test performance_test.cpp)
target_link_libraries(performance_test formula_calculator_static)

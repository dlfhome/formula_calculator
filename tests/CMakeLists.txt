# 测试 CMakeLists.txt
enable_testing()

# 设置测试编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Value 类型测试
add_executable(test_value test_value.cpp)
target_include_directories(test_value PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME ValueTest COMMAND test_value)

# 比较运算测试
add_executable(test_comparison test_comparison.cpp)
target_include_directories(test_comparison PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME ComparisonTest COMMAND test_comparison)

# 逻辑运算测试
add_executable(test_logical test_logical.cpp)
target_include_directories(test_logical PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME LogicalTest COMMAND test_logical)

# 三元运算符测试
add_executable(test_ternary test_ternary.cpp)
target_include_directories(test_ternary PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME TernaryTest COMMAND test_ternary)

# 运算符优先级测试
add_executable(test_precedence test_precedence.cpp)
target_include_directories(test_precedence PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME PrecedenceTest COMMAND test_precedence)

# 集成测试
add_executable(test_integration test_integration.cpp)
target_include_directories(test_integration PRIVATE ${CMAKE_SOURCE_DIR}/include)
# 集成测试需要链接公式计算器库
# target_link_libraries(test_integration formula_calculator_static)
add_test(NAME IntegrationTest COMMAND test_integration)

# 新功能综合测试
add_executable(test_new_features test_new_features.cpp)
target_include_directories(test_new_features PRIVATE ${CMAKE_SOURCE_DIR}/include)
# 优先使用静态库，动态库也可通过 option 切换
option(USE_SHARED_LIB "Use shared library for tests" OFF)
if(USE_SHARED_LIB)
    target_link_libraries(test_new_features PRIVATE formula_calculator_shared)
else()
    target_link_libraries(test_new_features PRIVATE formula_calculator_static)
endif()
add_test(NAME NewFeaturesTest COMMAND test_new_features)
# Windows 下动态库需要设置 DLL 搜索路径
if(USE_SHARED_LIB AND WIN32)
    set_tests_properties(NewFeaturesTest PROPERTIES
        ENVIRONMENT "PATH=$<TARGET_FILE_DIR:formula_calculator_shared>;$ENV{PATH}"
    )
endif()

# 打印测试信息
message(STATUS "已配置以下测试:")
message(STATUS "  - ValueTest: Value 类型测试")
message(STATUS "  - ComparisonTest: 比较运算测试")
message(STATUS "  - LogicalTest: 逻辑运算测试")
message(STATUS "  - TernaryTest: 三元运算符测试")
message(STATUS "  - PrecedenceTest: 运算符优先级测试")
message(STATUS "  - IntegrationTest: 集成测试")
message(STATUS "  - NewFeaturesTest: 新功能综合测试")

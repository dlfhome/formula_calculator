cmake_minimum_required(VERSION 3.14)
project(formula_calculator VERSION 1.0.0 LANGUAGES CXX)

# C++ 标准设置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
if(MSVC)
    add_compile_options(/W4 /WX- /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 源文件列表
set(FORMULA_SOURCES
    src/value.cpp
    src/token.cpp
    src/lexer.cpp
    src/ast.cpp
    src/parser.cpp
    src/evaluator.cpp
    src/variable_manager.cpp
    src/function_registry.cpp
    src/constant_registry.cpp
    src/formula_calculator.cpp
)

# 创建静态库
add_library(formula_calculator_static STATIC ${FORMULA_SOURCES})
set_target_properties(formula_calculator_static PROPERTIES
    OUTPUT_NAME formula_calculator_static
)

# 创建动态库
add_library(formula_calculator_shared SHARED ${FORMULA_SOURCES})
set_target_properties(formula_calculator_shared PROPERTIES
    OUTPUT_NAME formula_calculator
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# 线程支持
find_package(Threads REQUIRED)
target_link_libraries(formula_calculator_static Threads::Threads)
target_link_libraries(formula_calculator_shared Threads::Threads)

# 数学库链接 (Linux 需要)
if(UNIX)
    target_link_libraries(formula_calculator_static m)
    target_link_libraries(formula_calculator_shared m)
endif()

# 安装规则
install(TARGETS formula_calculator_static formula_calculator_shared
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/formula DESTINATION include)

# 测试配置
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# 示例配置
option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 打印配置信息
message(STATUS "")
message(STATUS "=== Formula Calculator Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build Tests: ${BUILD_TESTS}")
message(STATUS "Build Examples: ${BUILD_EXAMPLES}")
message(STATUS "========================================")
message(STATUS "")

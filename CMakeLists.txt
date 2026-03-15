cmake_minimum_required(VERSION 3.14)
project(formula_calculator VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler Options
if(MSVC)
    add_compile_options(/W4 /WX- /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include Directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source File List
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

# Create static library
add_library(formula_calculator_static STATIC ${FORMULA_SOURCES})
set_target_properties(formula_calculator_static PROPERTIES
    OUTPUT_NAME formula_calculator_static
)

# Create shared library
add_library(formula_calculator_shared SHARED ${FORMULA_SOURCES})
set_target_properties(formula_calculator_shared PROPERTIES
    OUTPUT_NAME formula_calculator
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# Thread support
find_package(Threads REQUIRED)
target_link_libraries(formula_calculator_static Threads::Threads)
target_link_libraries(formula_calculator_shared Threads::Threads)

# Math library link (needed for Linux)
if(UNIX)
    target_link_libraries(formula_calculator_static m)
    target_link_libraries(formula_calculator_shared m)
endif()

# Installation rules
install(TARGETS formula_calculator_static formula_calculator_shared
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/formula DESTINATION include)

# Test configuration
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Example configuration
option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Print configuration info
message(STATUS "")
message(STATUS "=== Formula Calculator Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build Tests: ${BUILD_TESTS}")
message(STATUS "Build Examples: ${BUILD_EXAMPLES}")
message(STATUS "========================================")
message(STATUS "")

# Tests CMakeLists.txt
enable_testing()

# Set test compilation options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Value type tests
add_executable(test_value test_value.cpp)
target_include_directories(test_value PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME ValueTest COMMAND test_value)

# Comparison operation tests
add_executable(test_comparison test_comparison.cpp)
target_include_directories(test_comparison PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME ComparisonTest COMMAND test_comparison)

# Logical operation tests
add_executable(test_logical test_logical.cpp)
target_include_directories(test_logical PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME LogicalTest COMMAND test_logical)

# Ternary operator tests
add_executable(test_ternary test_ternary.cpp)
target_include_directories(test_ternary PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME TernaryTest COMMAND test_ternary)

# Operator precedence tests
add_executable(test_precedence test_precedence.cpp)
target_include_directories(test_precedence PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME PrecedenceTest COMMAND test_precedence)

# Integration tests
add_executable(test_integration test_integration.cpp)
target_include_directories(test_integration PRIVATE ${CMAKE_SOURCE_DIR}/include)
# Integration tests need to link formula calculator library
# target_link_libraries(test_integration formula_calculator_static)
add_test(NAME IntegrationTest COMMAND test_integration)

# New feature comprehensive tests
add_executable(test_new_features test_new_features.cpp)
target_include_directories(test_new_features PRIVATE ${CMAKE_SOURCE_DIR}/include)
# Prefer static library, dynamic library can also be switched via option
option(USE_SHARED_LIB "Use shared library for tests" OFF)
if(USE_SHARED_LIB)
    target_link_libraries(test_new_features PRIVATE formula_calculator_shared)
else()
    target_link_libraries(test_new_features PRIVATE formula_calculator_static)
endif()
add_test(NAME NewFeaturesTest COMMAND test_new_features)
# Windows requires setting DLL search path for dynamic library
if(USE_SHARED_LIB AND WIN32)
    set_tests_properties(NewFeaturesTest PROPERTIES
        ENVIRONMENT "PATH=$<TARGET_FILE_DIR:formula_calculator_shared>;$ENV{PATH}"
    )
endif()

# Print test information
message(STATUS "Configured tests:")
message(STATUS "  - ValueTest: Value type tests")
message(STATUS "  - ComparisonTest: Comparison operation tests")
message(STATUS "  - LogicalTest: Logical operation tests")
message(STATUS "  - TernaryTest: Ternary operator tests")
message(STATUS "  - PrecedenceTest: Operator precedence tests")
message(STATUS "  - IntegrationTest: Integration tests")
message(STATUS "  - NewFeaturesTest: New feature comprehensive tests")

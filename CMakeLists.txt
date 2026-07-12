cmake_minimum_required(VERSION 3.20)
project(MemoryLeakPerformanceAnalyzer VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MLPA_BUILD_TESTS "Build tests" ON)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

add_library(mlpa_core
  src/parser/AnalyzerFrontend.cpp
  src/analyzers/MemorySafetyAnalyzer.cpp
  src/analyzers/PerformanceAnalyzer.cpp
  src/analyzers/ModernCppAnalyzer.cpp
  src/metrics/MetricsAnalyzer.cpp
  src/reporting/JsonReporter.cpp
  src/reporting/HtmlReporter.cpp
)

target_include_directories(mlpa_core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${LLVM_INCLUDE_DIRS}
  ${CLANG_INCLUDE_DIRS}
)
target_compile_definitions(mlpa_core PUBLIC ${LLVM_DEFINITIONS})
target_compile_options(mlpa_core PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(mlpa_core PUBLIC
  clangTooling clangFrontend clangSerialization clangDriver clangParse
  clangSema clangAnalysis clangAST clangASTMatchers clangEdit clangLex clangBasic
  LLVM
)

add_executable(mlpa src/tool/main.cpp)
target_link_libraries(mlpa PRIVATE mlpa_core)

install(TARGETS mlpa RUNTIME DESTINATION bin)

if(MLPA_BUILD_TESTS)
  enable_testing()
  add_test(NAME analyze_sample_json COMMAND mlpa --format=json --output=${CMAKE_BINARY_DIR}/sample-report.json ${CMAKE_CURRENT_SOURCE_DIR}/samples/intentional_issues.cpp -- -std=c++20)
  add_test(NAME analyze_sample_html COMMAND mlpa --format=html --output=${CMAKE_BINARY_DIR}/sample-report.html ${CMAKE_CURRENT_SOURCE_DIR}/samples/intentional_issues.cpp -- -std=c++20)
  add_test(NAME smoke_json COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/smoke_test.sh $<TARGET_FILE:mlpa> ${CMAKE_CURRENT_SOURCE_DIR}/samples/intentional_issues.cpp)
endif()

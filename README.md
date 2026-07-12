# Memory Leak Performance Analyzer

A Modern C++20 static analysis tool built on Clang LibTooling. It parses C++ source files into Clang ASTs and reports memory-safety, performance, modernization, and software-metric findings without executing the target program.

## Features

- **Memory safety:** raw `new`, raw pointer declarations, repeated `delete` operands, and unsafe C-style arrays.
- **Performance:** expensive pass-by-value of STL containers/strings, range-loop copies, repeated `push_back` reserve opportunities, and `std::map` usage that may be replaceable by `std::unordered_map`.
- **Modern C++:** `NULL` replacement, `typedef` replacement, missing `override`, `explicit` constructor suggestions, and const-correctness hints.
- **Metrics:** class/function counts, average function length, basic cyclomatic complexity, maximum nesting depth, and a quality score.
- **Reports:** JSON for machines and HTML for humans.

## Architecture

```text
include/mlpa/core       Shared domain types and analyzer interface
include/mlpa/parser     LibTooling frontend orchestration
include/mlpa/analyzers  Rule analyzers that traverse the Clang AST
include/mlpa/metrics    Metrics analyzer implementation
include/mlpa/reporting  JSON and HTML report renderers
src/tool                CLI entry point
samples                 Intentionally flawed C++ input
```

The design keeps parsing, analysis, metrics, and reporting separate. Each analyzer implements a small interface and writes findings into a shared `AnalysisResult`, which makes rules easy to test and extend.

## Build

Install LLVM/Clang development packages that provide `LLVMConfig.cmake` and Clang LibTooling CMake targets.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## Usage

```bash
./build/mlpa --format=json --output=report.json samples/intentional_issues.cpp -- -std=c++20
./build/mlpa --format=html --output=report.html samples/intentional_issues.cpp -- -std=c++20
```

Arguments after `--` are forwarded to Clang and can include include paths, defines, and language-standard flags.

## Supported Rules

| Rule | Category | Severity | Description |
| --- | --- | --- | --- |
| MEM001 | Memory | High | Raw `new` allocation; prefer RAII. |
| MEM002 | Memory | Medium | Raw pointer declaration. |
| MEM003 | Memory | Critical | Simple repeated `delete` operand pattern. |
| MEM004 | Memory | Medium | C-style array declaration. |
| PERF001 | Performance | Medium | STL container/string passed by value. |
| PERF003 | Performance | Low | Range-based loop copies elements. |
| PERF004 | Performance | Low | `push_back` may need `reserve`. |
| PERF005 | Performance | Low | Consider `unordered_map` when ordering is unnecessary. |
| MOD001 | Modern C++ | Low | Replace `NULL` with `nullptr`. |
| MOD002 | Modern C++ | Low | Replace `typedef` with `using`. |
| MOD003 | Modern C++ | Low | Consider const-correct pointer parameters. |
| MOD004 | Modern C++ | Medium | Add `override`. |
| MOD005 | Modern C++ | Medium | Add `explicit` to converting constructors. |

## Example

The sample file in `samples/intentional_issues.cpp` intentionally includes raw allocation, double delete, container copies, `std::map`, `typedef`, `NULL`, and missing `override` so the report has deterministic findings.

## Extending Rules

1. Add a new analyzer or visitor method under `src/analyzers`.
2. Emit an `Issue` with a stable rule id, severity, explanation, and fix.
3. Add or update a sample and a CTest smoke check.
4. Document the rule in this README.

## Limitations

This is a syntactic and local AST analyzer. It intentionally avoids whole-program alias analysis, path-sensitive ownership modeling, and build-system discovery. For production deployment, integrate it with `compile_commands.json` and add path-sensitive checks for ownership transfer.

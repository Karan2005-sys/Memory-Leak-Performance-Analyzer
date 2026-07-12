# Internship Project Report: C++ Static Code Analyzer for Memory Safety and Performance Optimization

## Abstract

This project implements a production-oriented static analysis command-line tool in Modern C++20 using Clang LibTooling and the Clang AST. The analyzer inspects C++ source files without executing them and produces JSON or HTML reports containing memory-safety, performance, modernization, and software-quality metric findings.

## Objectives

- Parse one or more C++ source files into Clang ASTs.
- Detect common memory-safety problems such as raw allocations, raw pointers, double deletes, and C-style arrays.
- Detect performance problems such as expensive pass-by-value, unnecessary range-loop copies, missed reserve opportunities, and potentially unnecessary ordered maps.
- Recommend Modern C++ improvements including `nullptr`, `using`, `override`, `explicit`, and const-correctness.
- Compute maintainability metrics and summarize them in reports.
- Provide a modular, extensible architecture with samples, tests, and documentation.

## Technology Stack

- Language: C++20
- Parser and AST: Clang LibTooling
- Build system: CMake
- Test runner: CTest
- Report formats: JSON and HTML

## System Architecture

The codebase is divided into clean modules:

1. **Core domain model:** shared issue, severity, location, and metrics data structures.
2. **Parser frontend:** configures Clang Tooling, parses input files, and invokes analyzers.
3. **Analyzers:** independent AST visitors for memory safety, performance, modernization, and metrics.
4. **Reporting:** converts analysis results into JSON or HTML.
5. **CLI:** parses command-line options and coordinates execution.

This separation follows SOLID principles: each analyzer has one responsibility, all analyzers depend on a stable interface, and reporters are interchangeable.

## Implementation Summary

The tool uses `clang::tooling::ClangTool` with a fixed compilation database. Once Clang builds the AST, a custom `ASTConsumer` invokes each analyzer. Analyzers use `clang::RecursiveASTVisitor` to inspect declarations, statements, constructors, function parameters, delete expressions, range loops, and call expressions.

Each issue includes:

- Stable rule id
- Category
- Severity
- File and line number
- Human-readable explanation
- Suggested fix

## Metrics

The metrics analyzer calculates:

- Number of classes
- Number of functions
- Average function length
- Basic cyclomatic complexity
- Maximum nesting depth
- Quality score

The quality score begins at 100 and applies penalties based on issue count and complexity.

## Testing Strategy

The repository includes `samples/intentional_issues.cpp`, which deliberately contains known problems. CTest runs the analyzer against this file in JSON and HTML modes. The smoke test validates that JSON output is parseable and contains expected rules.

## Results

The analyzer successfully identifies common local AST patterns and generates both machine-readable and human-readable reports. The modular design allows additional rules to be added without changing the CLI or reporting layers.

## Limitations and Future Work

- Add support for `compile_commands.json` discovery.
- Add path-sensitive memory ownership analysis.
- Reduce false positives with type traits and data-flow checks.
- Add SARIF output for CI and GitHub code scanning.
- Add richer unit tests using Clang in-memory snippets.

## Conclusion

The project demonstrates a complete static analysis pipeline using industrial compiler infrastructure. It provides practical diagnostics for memory safety, performance, modernization, and maintainability while remaining extensible for future research and production enhancements.

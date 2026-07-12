# Internship Project Report

## C++ Static Code Analyzer for Memory Safety and Performance Optimization

**Project Name:** Memory Leak Performance Analyzer (MLPA)  
**Domain:** Static Program Analysis, Compiler Tooling, Software Quality Engineering  
**Primary Language:** Modern C++20  
**Compiler Infrastructure:** LLVM/Clang LibTooling and Clang AST  
**Build System:** CMake  
**Report Formats:** JSON and HTML  

---

## Table of Contents

1. Executive Summary
2. Introduction
3. Problem Statement
4. Motivation
5. Project Objectives
6. Scope of the Project
7. Technologies and Tools Used
8. Background Study
9. Requirement Analysis
10. System Architecture
11. Module-Wise Design
12. Static Analysis Workflow
13. Rule Design and Detection Strategy
14. Software Metrics Design
15. Report Generation Design
16. Command-Line Interface
17. Sample Input Program
18. Testing and Validation Strategy
19. Build and Execution Procedure
20. Results and Expected Output
21. Limitations
22. Future Enhancements
23. Learning Outcomes
24. Conclusion
25. Appendix A: Supported Rules
26. Appendix B: Repository Structure
27. Appendix C: Example Commands

---

## 1. Executive Summary

The Memory Leak Performance Analyzer is a C++20 static analysis tool designed to inspect C++ source code without executing it. The tool uses Clang LibTooling to parse source files into an Abstract Syntax Tree (AST). It then traverses the AST to identify common memory-safety issues, performance inefficiencies, Modern C++ improvement opportunities, and maintainability metrics.

The project focuses on practical rules that are useful during code review and early quality assurance. It detects patterns such as raw memory allocation with `new`, raw pointer declarations, repeated `delete` operations, unsafe C-style arrays, expensive pass-by-value of STL containers, range-based loop copies, possible missing `std::vector::reserve`, use of `std::map` where `std::unordered_map` may be more appropriate, use of `NULL`, old-style `typedef`, missing `override`, and missing `explicit` constructors.

The system generates two types of reports:

- **JSON reports**, suitable for automation, CI tools, dashboards, and further processing.
- **HTML reports**, suitable for students, developers, mentors, and reviewers to inspect findings visually.

The tool is structured into clean modules: core domain types, parser frontend, analyzers, metrics, reporting, and CLI. This modular architecture makes the project maintainable and extensible.

---

## 2. Introduction

C++ is widely used in systems programming, embedded systems, game engines, financial systems, performance-critical applications, compilers, and infrastructure software. Its power comes from direct control over memory, object lifetime, templates, value semantics, and low-level system access. However, this power also introduces risks. Common C++ programming mistakes can cause memory leaks, undefined behavior, crashes, performance degradation, and difficult-to-maintain code.

Static analysis helps address these problems by examining source code before the program is executed. Unlike dynamic tools, static analyzers do not require a running program or test inputs. Instead, they inspect syntax, types, declarations, statements, and control structures to detect suspicious patterns.

This project implements a static analyzer using Clang LibTooling. Clang is a production-grade C/C++ compiler front end that exposes rich APIs for parsing and analyzing source code. By using Clang's AST, this project avoids unreliable text-only scanning and instead works with semantically meaningful program structures.

---

## 3. Problem Statement

Manual code review is time-consuming and may miss repetitive issues such as raw pointer usage, missing `override`, or inefficient function parameters. Many beginner and intermediate C++ programs still use outdated constructs and unsafe memory-management practices. In addition, performance issues can be introduced unintentionally through unnecessary copies or inappropriate container choices.

The problem is to build a static analysis tool that can:

1. Parse C++ source files reliably.
2. Detect memory-safety issues without executing the program.
3. Detect performance inefficiencies related to common STL usage.
4. Suggest Modern C++ improvements.
5. Compute simple software metrics to summarize maintainability.
6. Generate detailed reports in formats useful for both humans and tools.
7. Be organized in a modular way so new rules can be added easily.

---

## 4. Motivation

The motivation for this project comes from the need to improve C++ code quality early in the development lifecycle. Bugs such as memory leaks or double deletes can be expensive to diagnose after deployment. Performance inefficiencies caused by unnecessary copies can also be difficult to identify in large codebases. Static analysis provides an automated first line of defense.

The project is especially useful in an internship or academic context because it combines multiple important software engineering concepts:

- Compiler front-end usage
- Abstract Syntax Tree traversal
- Static code analysis
- Clean architecture
- Rule-based diagnostics
- Report generation
- CMake-based build systems
- Software metrics
- Modern C++ best practices

---

## 5. Project Objectives

The main objectives of the project are listed below.

### 5.1 Functional Objectives

- Parse one or more C++ source files into Clang ASTs.
- Detect memory-safety issues such as raw allocation, raw pointers, double delete patterns, and C-style arrays.
- Detect performance problems such as large STL pass-by-value, loop copies, possible missing reserve calls, and map-vs-unordered-map opportunities.
- Detect Modern C++ improvements such as replacing `NULL` with `nullptr`, replacing `typedef` with `using`, adding `override`, adding `explicit`, and improving const correctness.
- Compute software metrics including class count, function count, average function length, cyclomatic complexity, nesting depth, and quality score.
- Generate JSON and HTML reports containing severity, file, line, explanation, and suggested fix.

### 5.2 Non-Functional Objectives

- Use Modern C++20.
- Follow modular design and clean architecture.
- Keep analyzers independent and extensible.
- Provide readable documentation.
- Include sample input files with intentional issues.
- Provide tests or smoke checks for validating tool behavior.

---

## 6. Scope of the Project

### 6.1 In Scope

The project supports local AST-based static analysis of C++ source files. It focuses on syntax-aware and type-aware checks that can be implemented using Clang AST visitors. It produces structured issue reports and summary metrics.

### 6.2 Out of Scope

The current version does not implement full interprocedural data-flow analysis, symbolic execution, whole-program alias analysis, or path-sensitive ownership modeling. It also does not yet read `compile_commands.json` automatically. These are suitable future enhancements.

---

## 7. Technologies and Tools Used

| Technology | Purpose |
| --- | --- |
| C++20 | Main implementation language |
| LLVM/Clang | Compiler infrastructure and AST parsing |
| Clang LibTooling | Programmatic access to C++ parsing and frontend actions |
| RecursiveASTVisitor | AST traversal mechanism |
| CMake | Build configuration |
| CTest | Test execution framework |
| JSON | Machine-readable report output |
| HTML | Human-readable report output |
| Bash/Python | Smoke testing support |

---

## 8. Background Study

### 8.1 Static Analysis

Static analysis is the process of analyzing source code or compiled code without running it. It can detect coding mistakes, style problems, security issues, and maintainability risks. Static analysis is commonly used in CI/CD pipelines and code-review workflows.

### 8.2 Abstract Syntax Tree

An Abstract Syntax Tree is a structured representation of source code. Each node represents a program construct such as a function, class, statement, expression, declaration, or type. AST-based tools are more reliable than text scanners because they understand the syntax and semantic structure of the program.

### 8.3 Clang LibTooling

Clang LibTooling allows developers to build standalone tools that use Clang's parser and semantic analysis. It is commonly used to create formatters, refactoring tools, linters, and static analyzers.

### 8.4 Modern C++ Best Practices

Modern C++ encourages RAII, smart pointers, containers, type-safe null pointers, explicit constructors, override annotations, and const-correct interfaces. This project encodes several of these practices as analyzer rules.

---

## 9. Requirement Analysis

### 9.1 Functional Requirements

| ID | Requirement | Implementation Approach |
| --- | --- | --- |
| FR1 | Parse C++ files | Use Clang Tooling frontend |
| FR2 | Detect memory issues | AST visitor for new/delete/pointers/arrays |
| FR3 | Detect performance issues | AST visitor for parameters, loops, calls, containers |
| FR4 | Detect modernization issues | AST visitor for declarations and expressions |
| FR5 | Compute metrics | AST visitor over functions/classes/statements |
| FR6 | Generate JSON | JSON reporter module |
| FR7 | Generate HTML | HTML reporter module |
| FR8 | Provide CLI | `mlpa` executable |
| FR9 | Include sample files | `samples/intentional_issues.cpp` |
| FR10 | Include tests | CTest entries and smoke script |

### 9.2 Non-Functional Requirements

| ID | Requirement | Description |
| --- | --- | --- |
| NFR1 | Maintainability | Separate modules and interfaces |
| NFR2 | Extensibility | New analyzers can be added with minimal changes |
| NFR3 | Portability | CMake-based build for systems with LLVM/Clang development packages |
| NFR4 | Usability | Simple command-line interface |
| NFR5 | Report clarity | Every issue includes explanation and suggested fix |

---

## 10. System Architecture

The architecture follows a layered modular structure.

```text
User / CLI
   |
   v
AnalyzerFrontend
   |
   v
Clang LibTooling Parser
   |
   v
Clang AST
   |
   +--> MemorySafetyAnalyzer
   +--> PerformanceAnalyzer
   +--> ModernCppAnalyzer
   +--> MetricsAnalyzer
   |
   v
AnalysisResult
   |
   +--> JsonReporter
   +--> HtmlReporter
```

### 10.1 Architectural Principles

- **Single Responsibility Principle:** Each analyzer handles one concern.
- **Open/Closed Principle:** New rules can be added without redesigning the CLI.
- **Dependency Inversion:** The frontend depends on the analyzer interface, not concrete rule logic.
- **Separation of Concerns:** Parsing, analysis, metrics, and reporting are separated.

---

## 11. Module-Wise Design

### 11.1 Core Module

The core module defines shared data structures such as:

- `Issue`
- `Severity`
- `Category`
- `SourceLocation`
- `MetricsSummary`
- `AnalysisResult`
- `Analyzer` interface

These structures form the common language between analyzers and reporters.

### 11.2 Parser Module

The parser module configures Clang LibTooling and creates a frontend action. The frontend action creates an AST consumer, and the AST consumer runs all analyzers over the translation unit.

Responsibilities:

- Accept source files and compiler arguments.
- Build a Clang tool instance.
- Invoke analyzers after AST construction.
- Return a complete `AnalysisResult`.

### 11.3 Analyzer Modules

Analyzer modules traverse the AST and emit issues. Each analyzer focuses on one category:

- Memory safety
- Performance
- Modern C++ improvements
- Metrics

### 11.4 Reporting Module

The reporting module transforms `AnalysisResult` into output formats. JSON is designed for machines, while HTML is designed for human review.

### 11.5 CLI Module

The CLI module parses command-line options and coordinates execution. It supports selecting output format, output file, input source files, and compiler arguments after `--`.

---

## 12. Static Analysis Workflow

The workflow of the tool is as follows:

1. The user runs the `mlpa` executable with one or more C++ source files.
2. The CLI separates analyzer options from Clang compiler arguments.
3. The frontend creates a Clang compilation database.
4. Clang parses each input file and builds an AST.
5. The AST consumer invokes each analyzer.
6. Each analyzer traverses AST nodes and emits issues.
7. The metrics analyzer computes software metrics.
8. The result object is passed to a reporter.
9. The reporter generates JSON or HTML.
10. The output is printed to standard output or written to a file.

---

## 13. Rule Design and Detection Strategy

### 13.1 Memory Safety Rules

#### MEM001: Raw `new` Allocation

Raw `new` is detected by visiting `CXXNewExpr` nodes. Manual allocation increases the chance of memory leaks because ownership may not be clear. The suggested fix is to use RAII through `std::unique_ptr`, `std::make_unique`, or standard containers.

#### MEM002: Raw Pointer Declaration

Raw pointer declarations are detected by visiting variable declarations and checking whether the type is a pointer type. Raw pointers are not always incorrect, but frequent use may indicate unclear ownership. The suggested fix depends on intent: references, smart pointers, `std::span`, or observer pointers.

#### MEM003: Possible Double Delete

Delete expressions are inspected and their operand text is recorded. If the same delete operand appears more than once, the analyzer reports a possible double delete. This is a simple syntactic rule and is useful for catching obvious repeated deletes.

#### MEM004: C-Style Array

C-style arrays are detected through variable declaration types. The suggested fix is to use `std::array` for fixed-size storage or `std::vector` for dynamic storage.

### 13.2 Performance Rules

#### PERF001: Expensive Pass-by-Value

Function parameters are inspected. If a parameter is a standard-library container or string and is passed by value, the analyzer reports a potential unnecessary copy. The suggested fix is to use `const T&` unless ownership transfer or move semantics are intended.

#### PERF003: Inefficient Range-Based Loop Copy

Range-based for loops are inspected. If the loop variable is not a reference, the element may be copied on each iteration. The suggested fix is to use `const auto&` or `auto&`.

#### PERF004: Missing `vector::reserve` Opportunity

Calls to `push_back` are reported as possible reserve opportunities. This does not always indicate a bug, but it reminds developers to reserve capacity when the final size is known.

#### PERF005: Consider `std::unordered_map`

Variable declarations using `std::map` are reported as candidates for `std::unordered_map` when ordering is not required. This can improve average-case lookup performance.

### 13.3 Modern C++ Rules

#### MOD001: Replace `NULL` with `nullptr`

`NULL` can cause overload-resolution issues because it is typically an integer constant. `nullptr` is type-safe and should be preferred in Modern C++.

#### MOD002: Replace `typedef` with `using`

`using` aliases are generally more readable and support alias templates. The analyzer reports old-style typedef declarations.

#### MOD003: Improve Const Correctness

Pointer parameters are inspected for possible const-correctness improvements. Const-correct interfaces communicate intent and reduce accidental modification.

#### MOD004: Add `override`

Methods that override base-class virtual functions should use the `override` specifier. This allows the compiler to catch signature mismatches.

#### MOD005: Add `explicit`

Single-argument converting constructors should generally be marked `explicit` to avoid unintended implicit conversions.

---

## 14. Software Metrics Design

The metrics analyzer provides a high-level summary of code complexity and maintainability.

### 14.1 Class Count

The analyzer counts class or struct definitions. This gives a basic size indicator for object-oriented code.

### 14.2 Function Count

The analyzer counts functions with bodies. This helps estimate the amount of executable logic in the source file.

### 14.3 Average Function Length

Function length is estimated using source line numbers from function body begin and end locations. Long functions may indicate poor separation of concerns.

### 14.4 Cyclomatic Complexity

Cyclomatic complexity is estimated by counting branch-like constructs such as:

- `if`
- `for`
- `while`
- `do while`
- `case`
- conditional operator

Each function starts with a base complexity of 1. Every branch increases complexity.

### 14.5 Nesting Depth

Nesting depth measures how deeply branch statements are nested. High nesting depth often makes code harder to read and maintain.

### 14.6 Quality Score

The quality score starts at 100. Penalties are applied based on the number of issues and total complexity. The score is not intended to be a perfect measurement; it provides a simple summary for reports.

---

## 15. Report Generation Design

### 15.1 JSON Report

The JSON report contains:

- Metrics object
- Issue array
- Rule id
- Category
- Severity
- File
- Line
- Title
- Explanation
- Suggested fix

JSON is suitable for automation because other tools can parse it easily.

### 15.2 HTML Report

The HTML report contains:

- Project report title
- Metrics summary
- Issue table
- Severity styling
- Location, explanation, and suggestion columns

HTML is suitable for mentors, reviewers, students, and stakeholders who want a readable report.

---

## 16. Command-Line Interface

The command-line interface has the following format:

```bash
mlpa [--format=json|html] [--output=file] source... -- [clang args]
```

Examples:

```bash
mlpa --format=json --output=report.json samples/intentional_issues.cpp -- -std=c++20
mlpa --format=html --output=report.html samples/intentional_issues.cpp -- -std=c++20
```

Arguments after `--` are passed to Clang. This allows users to specify language standards, include paths, macros, and other compiler options.

---

## 17. Sample Input Program

The repository includes `samples/intentional_issues.cpp`. This file intentionally contains examples of poor C++ practices so the analyzer can produce predictable findings.

Intentional issues include:

- `typedef` usage
- Missing `override`
- Single-argument constructor without `explicit`
- C-style array
- Passing `std::vector<std::string>` by value
- Range loop copying elements
- `push_back` inside a loop
- Raw `new`
- Raw pointer
- Repeated `delete`
- `NULL`
- `std::map`

This sample file is useful for demonstrations, testing, and validation.

---

## 18. Testing and Validation Strategy

Testing is designed around a known-bad sample file.

### 18.1 CTest Checks

The CMake configuration registers tests that run the analyzer in JSON and HTML modes against the sample file.

### 18.2 Smoke Test Script

The smoke test script runs the analyzer, parses the JSON output using Python, and verifies that:

- The output is valid JSON.
- At least one function is counted.
- Issues are produced.
- The raw allocation rule `MEM001` appears.

### 18.3 Manual Validation

A developer can manually inspect the HTML report to confirm that issue locations, descriptions, and suggestions are understandable.

### 18.4 Environment Note

The current container used during development had an incomplete Clang development installation. The Clang CMake package referenced a missing `libclangBasic.a`, so the project could not be fully configured in that environment. On a system with complete LLVM/Clang development packages, the CMake workflow is expected to configure and build.

---

## 19. Build and Execution Procedure

### 19.1 Prerequisites

Install:

- C++20 compiler
- CMake 3.20 or later
- LLVM development package
- Clang development package
- Python 3 for smoke test validation

### 19.2 Build Commands

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

### 19.3 Run Tests

```bash
ctest --test-dir build --output-on-failure
```

### 19.4 Generate Reports

```bash
./build/mlpa --format=json --output=report.json samples/intentional_issues.cpp -- -std=c++20
./build/mlpa --format=html --output=report.html samples/intentional_issues.cpp -- -std=c++20
```

---

## 20. Results and Expected Output

When the analyzer runs on the sample file, it is expected to report findings from all major categories.

### 20.1 Expected Memory Findings

- Raw allocation through `new`
- Raw pointer declarations
- Possible repeated delete
- C-style array declaration

### 20.2 Expected Performance Findings

- STL container passed by value
- Range-based loop copying elements
- Possible missing vector reserve
- `std::map` candidate for `std::unordered_map`

### 20.3 Expected Modernization Findings

- `typedef` can be replaced with `using`
- `NULL` can be replaced with `nullptr`
- Missing `override`
- Constructor can be marked `explicit`

### 20.4 Expected Metrics

The report includes counts and complexity values. These values help reviewers understand the approximate size and complexity of the analyzed source.

---

## 21. Limitations

The project intentionally starts with practical local AST checks. Current limitations include:

- No whole-program analysis.
- No path-sensitive ownership tracking.
- No interprocedural alias analysis.
- `push_back` reserve checks may produce false positives.
- `std::map` to `std::unordered_map` suggestions require human confirmation.
- Raw pointer checks may report non-owning pointers.
- No automatic `compile_commands.json` integration yet.
- No SARIF output yet.

These limitations are acceptable for the first production-oriented version and provide a roadmap for future work.

---

## 22. Future Enhancements

Possible future improvements include:

1. Add support for `compile_commands.json`.
2. Add SARIF output for GitHub code scanning and CI integration.
3. Add path-sensitive memory leak analysis.
4. Track ownership transfers across functions.
5. Reduce false positives using type traits and data-flow analysis.
6. Add configuration files for enabling or disabling rules.
7. Add severity thresholds for CI failure.
8. Add autofix hints or Clang replacement edits.
9. Add more unit tests using in-memory source snippets.
10. Add dashboard-style HTML reports with charts.
11. Add rule documentation pages.
12. Add support for suppressing known false positives with comments.

---

## 23. Learning Outcomes

During this project, the following skills and concepts were practiced:

- Understanding compiler front-end architecture.
- Using Clang LibTooling to parse C++ code.
- Traversing AST nodes with `RecursiveASTVisitor`.
- Designing rule-based static analysis checks.
- Creating clean C++ interfaces and modular architecture.
- Using CMake to organize a multi-module project.
- Generating structured JSON and readable HTML reports.
- Writing project documentation suitable for internship submission.
- Thinking critically about false positives and limitations.
- Planning future work for production-grade static analysis.

---

## 24. Conclusion

The Memory Leak Performance Analyzer demonstrates how compiler infrastructure can be used to build a practical static analysis tool for C++. The project parses source code into a Clang AST, applies modular analyzers, computes metrics, and produces detailed reports. It addresses memory safety, performance optimization, Modern C++ migration, and maintainability measurement.

Although the current implementation focuses on local AST-based rules, the architecture is designed for future extension. More advanced analyses such as path-sensitive leak detection, SARIF integration, rule configuration, and automatic fixes can be added incrementally. Overall, the project provides a strong foundation for learning and demonstrating static analysis, Modern C++ engineering, and software-quality automation.

---

## 25. Appendix A: Supported Rules

| Rule ID | Category | Severity | Detection Target | Suggested Improvement |
| --- | --- | --- | --- | --- |
| MEM001 | Memory Safety | High | Raw `new` allocation | Use RAII, smart pointers, or containers |
| MEM002 | Memory Safety | Medium | Raw pointer declaration | Use references, smart pointers, or `std::span` |
| MEM003 | Memory Safety | Critical | Repeated `delete` operand | Use single ownership and avoid manual delete |
| MEM004 | Memory Safety | Medium | C-style array | Use `std::array` or `std::vector` |
| PERF001 | Performance | Medium | STL object passed by value | Pass by `const&` unless taking ownership |
| PERF003 | Performance | Low | Range-loop element copy | Use `const auto&` or `auto&` |
| PERF004 | Performance | Low | `push_back` without obvious reserve | Call `reserve()` when size is known |
| PERF005 | Performance | Low | `std::map` declaration | Use `std::unordered_map` if ordering is not needed |
| MOD001 | Modern C++ | Low | `NULL` usage | Use `nullptr` |
| MOD002 | Modern C++ | Low | `typedef` declaration | Use `using` |
| MOD003 | Modern C++ | Low | Pointer parameter constness | Add appropriate const qualification |
| MOD004 | Modern C++ | Medium | Missing `override` | Add `override` specifier |
| MOD005 | Modern C++ | Medium | Converting constructor | Add `explicit` |

---

## 26. Appendix B: Repository Structure

```text
Memory-Leak-Performance-Analyzer/
├── CMakeLists.txt
├── README.md
├── docs/
│   └── INTERNSHIP_REPORT.md
├── include/
│   └── mlpa/
│       ├── analyzers/
│       ├── core/
│       ├── parser/
│       └── reporting/
├── samples/
│   └── intentional_issues.cpp
├── src/
│   ├── analyzers/
│   ├── metrics/
│   ├── parser/
│   ├── reporting/
│   └── tool/
└── tests/
    └── smoke_test.sh
```

---

## 27. Appendix C: Example Commands

### Configure

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

### Build

```bash
cmake --build build -j
```

### Test

```bash
ctest --test-dir build --output-on-failure
```

### Generate JSON

```bash
./build/mlpa --format=json --output=report.json samples/intentional_issues.cpp -- -std=c++20
```

### Generate HTML

```bash
./build/mlpa --format=html --output=report.html samples/intentional_issues.cpp -- -std=c++20
```

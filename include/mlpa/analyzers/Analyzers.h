#pragma once
#include "mlpa/core/Analyzer.h"
namespace mlpa {
class MemorySafetyAnalyzer final : public Analyzer { public: void analyze(clang::ASTContext &, AnalysisResult &) override; };
class PerformanceAnalyzer final : public Analyzer { public: void analyze(clang::ASTContext &, AnalysisResult &) override; };
class ModernCppAnalyzer final : public Analyzer { public: void analyze(clang::ASTContext &, AnalysisResult &) override; };
class MetricsAnalyzer final : public Analyzer { public: void analyze(clang::ASTContext &, AnalysisResult &) override; };
}

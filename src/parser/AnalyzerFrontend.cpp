#include "mlpa/parser/AnalyzerFrontend.h"
#include "mlpa/analyzers/Analyzers.h"
#include <clang/AST/ASTConsumer.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>
#include <memory>

namespace mlpa {

std::string toString(Severity severity) {
  switch (severity) { case Severity::Critical: return "Critical"; case Severity::High: return "High"; case Severity::Medium: return "Medium"; case Severity::Low: return "Low"; }
  return "Low";
}
std::string toString(Category category) {
  switch (category) { case Category::MemorySafety: return "MemorySafety"; case Category::Performance: return "Performance"; case Category::ModernCpp: return "ModernCpp"; case Category::Metrics: return "Metrics"; }
  return "Metrics";
}

class AnalysisConsumer final : public clang::ASTConsumer {
public:
  explicit AnalysisConsumer(AnalysisResult &result) : result_(result) {
    analyzers_.push_back(std::make_unique<MemorySafetyAnalyzer>());
    analyzers_.push_back(std::make_unique<PerformanceAnalyzer>());
    analyzers_.push_back(std::make_unique<ModernCppAnalyzer>());
    analyzers_.push_back(std::make_unique<MetricsAnalyzer>());
  }
  void HandleTranslationUnit(clang::ASTContext &context) override {
    for (auto &analyzer : analyzers_) analyzer->analyze(context, result_);
  }
private:
  AnalysisResult &result_;
  std::vector<std::unique_ptr<Analyzer>> analyzers_;
};

class AnalysisAction final : public clang::ASTFrontendAction {
public:
  explicit AnalysisAction(AnalysisResult &result) : result_(result) {}
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef) override {
    return std::make_unique<AnalysisConsumer>(result_);
  }
private: AnalysisResult &result_;
};

class AnalysisActionFactory final : public clang::tooling::FrontendActionFactory {
public:
  explicit AnalysisActionFactory(AnalysisResult &result) : result_(result) {}
  std::unique_ptr<clang::FrontendAction> create() override { return std::make_unique<AnalysisAction>(result_); }
private: AnalysisResult &result_;
};

AnalysisResult AnalyzerFrontend::run(const std::vector<std::string> &sourceFiles,
                                     const std::vector<std::string> &compileArgs) {
  AnalysisResult result;
  clang::tooling::FixedCompilationDatabase compilations(".", compileArgs);
  clang::tooling::ClangTool tool(compilations, sourceFiles);
  AnalysisActionFactory factory(result);
  tool.run(&factory);
  const auto penalty = std::min<unsigned>(90, static_cast<unsigned>(result.issues.size() * 4 + result.metrics.totalCyclomaticComplexity / 2));
  result.metrics.qualityScore = result.metrics.qualityScore > penalty ? result.metrics.qualityScore - penalty : 0;
  return result;
}

} // namespace mlpa

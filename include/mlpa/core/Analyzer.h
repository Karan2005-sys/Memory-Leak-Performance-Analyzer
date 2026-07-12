#pragma once
#include "mlpa/core/AnalysisResult.h"
#include <clang/AST/ASTContext.h>

namespace mlpa {

class Analyzer {
public:
  virtual ~Analyzer() = default;
  virtual void analyze(clang::ASTContext &context, AnalysisResult &result) = 0;
};

} // namespace mlpa

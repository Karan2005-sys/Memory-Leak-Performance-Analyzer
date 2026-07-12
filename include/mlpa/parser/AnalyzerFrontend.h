#pragma once
#include "mlpa/core/Analyzer.h"
#include <memory>
#include <string>
#include <vector>

namespace mlpa {

class AnalyzerFrontend {
public:
  AnalysisResult run(const std::vector<std::string> &sourceFiles,
                     const std::vector<std::string> &compileArgs);
};

} // namespace mlpa

#pragma once
#include "mlpa/core/Issue.h"
#include <vector>

namespace mlpa {

struct MetricsSummary {
  unsigned classCount{0};
  unsigned functionCount{0};
  double averageFunctionLength{0.0};
  unsigned totalCyclomaticComplexity{0};
  unsigned maxNestingDepth{0};
  unsigned qualityScore{100};
};

struct AnalysisResult {
  std::vector<Issue> issues;
  MetricsSummary metrics;
};

} // namespace mlpa

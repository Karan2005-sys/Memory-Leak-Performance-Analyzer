#pragma once
#include "mlpa/core/AnalysisResult.h"
#include <string>
namespace mlpa {
class Reporter { public: virtual ~Reporter() = default; virtual std::string render(const AnalysisResult &) const = 0; };
class JsonReporter final : public Reporter { public: std::string render(const AnalysisResult &) const override; };
class HtmlReporter final : public Reporter { public: std::string render(const AnalysisResult &) const override; };
}

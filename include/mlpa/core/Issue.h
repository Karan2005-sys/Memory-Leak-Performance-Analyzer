#pragma once
#include <string>

namespace mlpa {

enum class Severity { Critical, High, Medium, Low };

enum class Category { MemorySafety, Performance, ModernCpp, Metrics };

struct SourceLocation {
  std::string file;
  unsigned line{0};
  unsigned column{0};
};

struct Issue {
  std::string ruleId;
  Category category{Category::MemorySafety};
  Severity severity{Severity::Low};
  SourceLocation location;
  std::string title;
  std::string explanation;
  std::string suggestion;
};

std::string toString(Severity severity);
std::string toString(Category category);

} // namespace mlpa

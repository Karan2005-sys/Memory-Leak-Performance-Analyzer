#include "mlpa/parser/AnalyzerFrontend.h"
#include "mlpa/reporting/Reporter.h"
#include <fstream>
#include <iostream>
#include <memory>

int main(int argc, const char **argv) {
  std::string format = "json", output;
  std::vector<std::string> files, compileArgs{"-std=c++20"};
  bool afterDashDash = false;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--") { afterDashDash = true; continue; }
    if (afterDashDash) { compileArgs.push_back(arg); continue; }
    if (arg.rfind("--format=",0)==0) format = arg.substr(9);
    else if (arg.rfind("--output=",0)==0) output = arg.substr(9);
    else if (arg == "--help") { std::cout << "Usage: mlpa [--format=json|html] [--output=file] source... -- [clang args]\n"; return 0; }
    else files.push_back(arg);
  }
  if (files.empty()) { std::cerr << "No source files provided. Use --help.\n"; return 2; }
  mlpa::AnalyzerFrontend frontend;
  auto result = frontend.run(files, compileArgs);
  std::unique_ptr<mlpa::Reporter> reporter = format == "html" ? std::make_unique<mlpa::HtmlReporter>() : std::make_unique<mlpa::JsonReporter>();
  auto rendered = reporter->render(result);
  if (output.empty()) std::cout << rendered; else { std::ofstream out(output); out << rendered; }
  return 0;
}

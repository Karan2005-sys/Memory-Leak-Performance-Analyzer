#include <map>
#include <string>
#include <vector>
#include <cstdlib>

typedef std::vector<int> IntVector;

class Base { public: virtual void run() {} };
class Worker : public Base {
public:
  Worker(int id) : id_(id) {}
  void run() { int local[4] = {1,2,3,4}; (void)local; }
private:
  int id_;
};

void consume(std::vector<std::string> names) {
  std::vector<int> values;
  for (auto name : names) {
    values.push_back(static_cast<int>(name.size()));
  }
}

void leaks_and_deletes() {
  int *raw = new int(42);
  delete raw;
  delete raw;
  Worker *worker = NULL;
  (void)worker;
}

int main() {
  std::map<std::string, int> counts;
  consume({"alpha", "beta"});
  return counts.size();
}

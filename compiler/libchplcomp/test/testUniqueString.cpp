#include "chpl/AST/Context.h"
#include "chpl/AST/UniqueString.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cassert>

using namespace chpl;
using namespace ast;

void testPerformance(Context* ctx, const char* inputFile, bool printTiming) {
  int repeat = 10;

  std::ifstream file(inputFile);
  if (file.is_open()) {
    auto start = std::chrono::steady_clock::now();
    std::string line;
    std::string word;
    while (std::getline(file, line)) {
      for (int i = 0; i < repeat; i++) {
        std::istringstream iss(line, std::istringstream::in);
        while (iss >> word) {
          UniqueString::build(ctx, word);
        }
      }
    }
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    if (printTiming)
      std::cout << "unordered map elapsed time: " << elapsed.count() << " s\n";

    file.close();
  } else {
    std::cerr << "could not open file " << inputFile << "\n";
    exit(-1);
  }
}

int main(int argc, char** argv) {
  const char* inputFile = "moby.txt";
  int repeat = 10;
  std::string timingArg = "--timing";
  bool printTiming = false;
  for (int i = 1; i < argc; i++) {
    if (argv[i] == timingArg)
      printTiming = true;
    else
      inputFile = argv[i];
  }

  Context ctx;

  // First, add some strings to the map and make sure we get uniqueness.
  {
    std::string test1 = "test1";
    std::string test1Copy = test1;
    assert(test1.c_str() != test1Copy.c_str());
    UniqueString t1 = UniqueString::build(&ctx, test1);
    UniqueString t2 = UniqueString::build(&ctx, test1Copy);
    UniqueString t3 = UniqueString::build(&ctx, "test1");
    assert(t1.c_str() == t2.c_str());
    assert(t2.c_str() == t3.c_str());

    UniqueString h = UniqueString::build(&ctx, "hello");
    assert(h.c_str() != t1.c_str());

    // check that uniqueString(NULL) == uniqueString("")
    assert(UniqueString::build(&ctx, NULL) == UniqueString::build(&ctx, ""));

    // check that default-constructed unique string matches one from ""
    UniqueString empty;
    assert(empty.c_str() == UniqueString::build(&ctx, "").c_str());
  }

  // Next, measure performance
  testPerformance(&ctx, inputFile, printTiming);
  return 0;
}

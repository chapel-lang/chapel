#include "chpl/AST/UniqueString.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cassert>

void testPerformance(const char* inputFile, bool printTiming) {
  int repeat = 10;

  std::ifstream file(inputFile);
  if (file.is_open()) {
    auto start = std::chrono::steady_clock::now();
    std::string line;
    std::string word;
    while (std::getline(file, line)) {
      for (int i = 0; i < repeat; i++) {
        std::istringstream iss(line, std::istringstream::in);
        while( iss >> word )
        {
          chpl::UniqueString u(word);
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

  // First, add some strings to the map and make sure we get uniqueness.
  {
    std::string test1 = "test1";
    std::string test1Copy = test1;
    assert(test1.c_str() != test1Copy.c_str());
    chpl::UniqueString t1(test1);
    chpl::UniqueString t2(test1Copy);
    chpl::UniqueString t3("test1");
    assert(t1.c_str() == t2.c_str());
    assert(t2.c_str() == t3.c_str());

    chpl::UniqueString h("hello");
    assert(h.c_str() != t1.c_str());
  }

  // Next, measure performance
  testPerformance(inputFile, printTiming);
  return 0;
}

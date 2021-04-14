#include "chpl/AST/Context.h"
#include "chpl/AST/UniqueString.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cassert>

using namespace chpl;
using namespace ast;

void testPerformance(Context* ctx, const char* inputFile, bool printTiming) {
  int outerRepeat = 10;
  int innerRepeat = 1;

  bool dohisto = false;
  int querybysize[41];
  int nqueries = 0;
  for(int i = 0; i < 41; i++) querybysize[i] = 0;

  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < outerRepeat; i++) {
    std::ifstream file(inputFile);
    if (file.is_open()) {
      std::string line;
      std::string word;
      while (std::getline(file, line)) {
        for (int j = 0; j < innerRepeat; j++) {
          std::istringstream iss(line, std::istringstream::in);
          while (iss >> word) {
            // double the word length to better match
            // statistics of Chapel code
            word += word;

            if (dohisto) {
              int len = word.size();
              if (len > 40) len = 40;
              querybysize[len]++;
              nqueries++;
            }
            UniqueString::build(ctx, word);
          }
        }
      }
      file.close();
    } else {
      std::cerr << "could not open file " << inputFile << "\n";
      exit(-1);
    }
  }

  auto end = std::chrono::steady_clock::now();

  std::chrono::duration<double> elapsed = end - start;

  if (printTiming) {
    std::cout << "unordered map elapsed time: " << elapsed.count() << " s\n";

    if (dohisto) {
      std::cout << "Queried " << nqueries << " strings\n";
      int sum = 0;
      for(int i = 0; i < 40; i++) {
        std::cout << "length < " << i << " -- " << sum << "\n";
        sum += querybysize[i];
        std::cout << "length " << i << " -- " << querybysize[i] << "\n";
      }
      std::cout << "length >= 40 " << " -- " << querybysize[40] << "\n";
    }
  }
}

void test0() {
  auto context = Context::build();
  Context* ctx = context.get();

  // First, add some strings to the map and make sure we get uniqueness.
  // needs to be long enough to require strdup etc.
  #define TEST1STRING "this is a very very very long string"
  std::string test1 = TEST1STRING;
  std::string test1Copy = test1;
  assert(test1.c_str() != test1Copy.c_str());
  const char* t1 = ctx->uniqueCString(test1);
  const char* t2 = ctx->uniqueCString(test1Copy);
  const char* t3 = ctx->uniqueCString(TEST1STRING);
  assert(t1 == t2);
  assert(t2 == t3);

  // this string is short enough to be inlined
  std::string hello = "hello";
  const char* h1 = ctx->uniqueCString("hello");
  const char* h2 = ctx->uniqueCString(hello);
  assert(h1 == h2);

  // check that uniqueString(NULL) == uniqueString("")
  assert(ctx->uniqueCString(nullptr) == ctx->uniqueCString(""));

  // check that uniqueString(NULL) != nullptr
  assert(ctx->uniqueCString(nullptr) != nullptr);

  const char* x = ctx->uniqueCString("aVeryLongIdentifierName");
}


void test1() {
  auto context = Context::build();
  Context* ctx = context.get();

  // First, add some strings to the map and make sure we get uniqueness.
  // needs to be long enough to require strdup etc.
  #define TEST1STRING "this is a very very very long string"
  std::string test1 = TEST1STRING;
  std::string test1Copy = test1;
  assert(test1.c_str() != test1Copy.c_str());
  UniqueString t1 = UniqueString::build(ctx, test1);
  UniqueString t2 = UniqueString::build(ctx, test1Copy);
  UniqueString t3 = UniqueString::build(ctx, TEST1STRING);
  assert(t1.c_str() == t2.c_str());
  assert(t2.c_str() == t3.c_str());

  // this string is short enough to be inlined
  std::string hello = "hello";
  UniqueString h1 = UniqueString::build(ctx, "hello");
  UniqueString h2 = UniqueString::build(ctx, hello);
  assert(h1 == h2);

  // check that uniqueString(NULL) == uniqueString("")
  assert(UniqueString::build(ctx, NULL) == UniqueString::build(ctx, ""));

  // check that default-constructed unique string matches one from ""
  UniqueString empty;
  assert(empty == UniqueString::build(ctx, ""));
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

  test0();
  test1();

  auto context = Context::build();
  Context* ctx = context.get();

  // Next, measure performance
  testPerformance(ctx, inputFile, printTiming);
  return 0;
}

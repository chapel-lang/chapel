/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chpl/queries/Context.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/queries/global-strings.h"

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

static void testPerformance(Context* ctx,
                            const char* inputFile,
                            bool printTiming) {
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
            UniqueString::get(ctx, word);
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

static void test0() {
  Context context;
  Context* ctx = &context;

  // First, add some strings to the map and make sure we get uniqueness.
  // needs to be long enough to require strdup etc.
  #define TEST1STRING "this is a very very very long string"
  std::string test1 = TEST1STRING;
  std::string test1Copy = test1;
  assert(test1.c_str() != test1Copy.c_str());
  const char* t1 = ctx->uniqueCString(test1.c_str());
  const char* t2 = ctx->uniqueCString(test1Copy.c_str());
  const char* t3 = ctx->uniqueCString(TEST1STRING);
  assert(t1 == t2);
  assert(t2 == t3);

  // this string is short enough to be inlined
  std::string hello = "hello";
  const char* h1 = ctx->uniqueCString("hello");
  const char* h2 = ctx->uniqueCString(hello.c_str());
  assert(h1 == h2);
  assert(0 == strcmp("hello", h1));
  assert(strlen("hello") == ctx->lengthForUniqueString(h1));

  // check that uniqueString(NULL) == uniqueString("")
  assert(ctx->uniqueCString(nullptr) == ctx->uniqueCString(""));

  // check that uniqueString(NULL) != nullptr
  assert(ctx->uniqueCString(nullptr) != nullptr);

  // check that zero-length uniqueString is the empty string too
  assert(ctx->uniqueCString("bab", 0) == ctx->uniqueCString(""));

  const char* x = ctx->uniqueCString("aVeryLongIdentifierName");
  assert(x != h1 && x != nullptr);
  assert(0 == strcmp("aVeryLongIdentifierName", x));
  assert(strlen("aVeryLongIdentifierName") == ctx->lengthForUniqueString(x));

  // check that strings with null bytes are uniqued differently
  const char* h_plus = ctx->uniqueCString("hello\0plus", 10);
  assert(h_plus != h1);
  assert(ctx->lengthForUniqueString(h_plus) == 10);
  const char* h_p = ctx->uniqueCString("hello\0p", 7);
  assert(h_p != h_plus && h_p != h1);
  assert(ctx->lengthForUniqueString(h_p) == 7);

  // check that truncation works for short strings and long ones
  assert(h1 == ctx->uniqueCString("hello____", strlen("hello")));
  assert(x == ctx->uniqueCString("aVeryLongIdentifierName____",
                                 strlen("aVeryLongIdentifierName")));

  // check concatenation builder
  const char* ab1 = ctx->uniqueCString("aabbb");
  const char* ab2 = ctx->uniqueCStringConcatLen("aa", 2, "bbb", 3);
  assert(ab1 == ab2);

  const char* abcd1 = ctx->uniqueCString("a"
                                         "bb"
                                         "ccc"
                                         "dddd"
                                         "eeeee"
                                         "ffffff"
                                         "ggggggg"
                                         "hhhhhhhh"
                                         "iiiiiiiii");
  const char* abcd2 = ctx->uniqueCStringConcatLen("a", 1,
                                                  "bb", 2,
                                                  "ccc", 3,
                                                  "dddd", 4,
                                                  "eeeee", 5,
                                                  "ffffff", 6,
                                                  "ggggggg", 7,
                                                  "hhhhhhhh", 8,
                                                  "iiiiiiiii", 9);
  const char* abcd3 = ctx->uniqueCStringConcat("a",
                                               "bb",
                                               "ccc",
                                               "dddd",
                                               "eeeee",
                                               "ffffff",
                                               "ggggggg",
                                               "hhhhhhhh",
                                               "iiiiiiiii");

  assert(0 == strcmp(abcd1,  "a"
                             "bb"
                             "ccc"
                             "dddd"
                             "eeeee"
                             "ffffff"
                             "ggggggg"
                             "hhhhhhhh"
                             "iiiiiiiii"));
  assert(abcd1 == abcd2);
  assert(abcd2 == abcd3);
}


static void test1() {
  Context context;
  Context* ctx = &context;

  // First, add some strings to the map and make sure we get uniqueness.
  // needs to be long enough to require strdup etc.
  #define TEST1STRING "this is a very very very long string"
  std::string test1 = TEST1STRING;
  std::string test1Copy = test1;
  assert(test1.c_str() != test1Copy.c_str());
  UniqueString t1 = UniqueString::get(ctx, test1);
  UniqueString t2 = UniqueString::get(ctx, test1Copy);
  UniqueString t3 = UniqueString::get(ctx, TEST1STRING);
  assert(0 == strcmp(t1.c_str(), test1.c_str()));
  assert(t1.c_str() == t2.c_str());
  assert(t2.c_str() == t3.c_str());
  assert(t1.astr(ctx) == ctx->uniqueCString(test1.c_str()));
  assert(t1.length() == strlen(TEST1STRING));

  // this string is short enough to be inlined
  std::string hello = "hello";
  UniqueString h1 = UniqueString::get(ctx, "hello");
  UniqueString h2 = UniqueString::get(ctx, hello);
  assert(0 == strcmp(h1.c_str(), hello.c_str()));
  assert(h1 == h2);
  assert(h1.astr(ctx) == ctx->uniqueCString(hello.c_str()));
  assert(h1.length() == strlen("hello"));

  // check that uniqueString(NULL) == uniqueString("")
  assert(UniqueString::get(ctx, NULL) == UniqueString::get(ctx, ""));

  // check that default-constructed unique string matches one from ""
  UniqueString empty;
  assert(empty == UniqueString::get(ctx, ""));

  // check that truncation works for short strings and long ones
  assert(h1 == UniqueString::get(ctx, "hello____", strlen("hello")));
  assert(t1 == UniqueString::get(ctx, TEST1STRING "_____",
                                   strlen(TEST1STRING)));

  // check concatenation builder
  UniqueString ab1 = UniqueString::get(ctx, "aabbb");
  UniqueString ab2 = UniqueString::getConcat(ctx, "aa", "bbb");
  assert(ab1 == ab2);
  assert(0 == strcmp(ab1.c_str(), "aabbb"));

  UniqueString abcd1 = UniqueString::get(ctx,
                                           "a"
                                           "bb"
                                           "ccc"
                                           "dddd"
                                           "eeeee"
                                           "ffffff"
                                           "ggggggg"
                                           "hhhhhhhh"
                                           "iiiiiiiii");
  UniqueString abcd2 = UniqueString::getConcat(ctx,
                                                 "a",
                                                 "bb",
                                                 "ccc",
                                                 "dddd",
                                                 "eeeee",
                                                 "ffffff",
                                                 "ggggggg",
                                                 "hhhhhhhh",
                                                 "iiiiiiiii");
  assert(abcd1 == abcd2);
  assert(0 == strcmp(abcd1.c_str(), "a"
                                    "bb"
                                    "ccc"
                                    "dddd"
                                    "eeeee"
                                    "ffffff"
                                    "ggggggg"
                                    "hhhhhhhh"
                                    "iiiiiiiii"));

  // check ==
  assert(t1 == t2);
  assert(!(t1 != t2));
  assert(h1 != t1);
  assert(!(h1 == t1));

  // test copy init
  UniqueString t1Copy = t1;
  UniqueString h1Copy = h1;
  assert(t1Copy == t1);
  assert(h1Copy == h1);

  // test assignment
  UniqueString t1assign;
  UniqueString h1assign;
  t1assign = t1;
  h1assign = h1;
  assert(t1assign == t1);
  assert(h1assign == h1);

  // test swap
  t1Copy.swap(h1Copy);
  assert(t1Copy == h1);
  assert(h1Copy == t1);

  // test hash
  assert(t1.hash() != h1.hash());
}

// Global Strings are created once and identical between
// contexts. Global strings also don't get deleted
static void test2() {
  Context context1;
  Context* ctx1 = &context1;

  Context context2;
  Context* ctx2 = &context2;

  // We need to test a string that actually gets saved in the table
  // so something longer than a smallstring
  UniqueString s1 = UniqueString::get(ctx1, "borrowed");
  UniqueString s2 = UniqueString::get(ctx2, "borrowed");

  // For use with debug tracing
  UniqueString::get(ctx1, "i will get garbage collected");
  UniqueString::get(ctx2, "i will get garbage collected");

  assert(s1.c_str() == USTR("borrowed").c_str());
  assert(s2.c_str() == USTR("borrowed").c_str());
  assert(s1.c_str() == s2.c_str());

  context1.advanceToNextRevision(true);
  context1.collectGarbage();

  context2.advanceToNextRevision(true);
  context2.collectGarbage();

  // Global string doesn't get collected, should get same pointer
  UniqueString s3 = UniqueString::get(ctx1, "borrowed");
  UniqueString s4 = UniqueString::get(ctx2, "borrowed");

  assert(s1.c_str() == s3.c_str());
  assert(s2.c_str() == s4.c_str());

  // Small string should still be equal (even though inlined)
  UniqueString s5 = UniqueString::get(ctx1, "align");
  UniqueString s6 = UniqueString::get(ctx2, "align");

  assert(s5 == USTR("align"));
  assert(s6 == USTR("align"));

  // Typo in "align" is a compile time error
  // UniqueString s7 = USTR("algin");
}

int main(int argc, char** argv) {
  const char* inputFile = "moby.txt";
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
  test2();

  Context context;
  Context* ctx = &context;

  // Next, measure performance
  testPerformance(ctx, inputFile, printTiming);
  return 0;
}

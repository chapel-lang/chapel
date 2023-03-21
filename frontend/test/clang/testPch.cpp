/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/util/clang-integration.h"

// helper functions


static void testIt(const char* testName,
                   const char* program,
                   std::vector<const char*> expectedNames,
                   std::vector<const char*> unexpectedNames) {
  printf("test %s\n", testName);

  std::string chpl_home;
  if (const char* chpl_home_env = getenv("CHPL_HOME")) {
    chpl_home = chpl_home_env;
  } else {
    printf("CHPL_HOME must be set");
    exit(1);
  }
  Context::Configuration config;
  config.chplHome = chpl_home;
  Context ctx(config);
  Context* context = &ctx;

  context->setQueryTimingFlag(true);

  ErrorGuard guard(context);
  auto path = UniqueString::get(context, testName);
  std::string contents = program;
  setFileText(context, path, contents);
  // parse it so that Context knows about the IDs
  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* M = vec[0];

  const ExternBlock* externBlock = nullptr;

  for (auto stmt : M->stmts()) {
    if (auto eb = stmt->toExternBlock()) {
      externBlock = eb;
    }
  }
  assert(externBlock);

  auto tpch =
    util::createClangPrecompiledHeader(context, externBlock->id()).get();
  for (auto name : expectedNames) {
    auto uname = UniqueString::get(context, name);
    bool got = util::precompiledHeaderContainsName(context, tpch, uname);
    if (got) printf("found expected %s\n", uname.c_str());
    else     printf("MISSING expected %s\n", uname.c_str());
    assert(got == true);
  }
  for (auto name : unexpectedNames) {
    auto uname = UniqueString::get(context, name);
    bool got = util::precompiledHeaderContainsName(context, tpch, uname);
    if (got) printf("found UNEXPECTED %s\n", uname.c_str());
    else     printf("did not find, as expected %s\n", uname.c_str());
    assert(got == false);
  }

  context->queryTimingReport(std::cout);
}


static void test1() {
  testIt("test1.chpl",
         R""""(
            module M {
              extern { int x; }
            }
         )"""",
         {"x"},
         {"y"});
}
static void test2() {
  testIt("test2.chpl",
         R""""(
            module M {
              extern {
                #define FOO
                static void bar(void);
                static void bar(void) { }
              }
            }
         )"""",
         {"FOO", "bar"},
         {"missing"});
}
static void test3() {
  testIt("test3.chpl",
         R""""(
            module M {
              extern {
                #include <stdio.h>
                static void bar(void);
                static void bar(void) { }
              }
            }
         )"""",
         {"bar", // directly defined
          // functions defined in stdio.h
          "printf", "fprintf", "sprintf", "snprintf",
          "sscanf", "fscanf",
          "fgetc", "getc", "getchar", "ungetc",
          "fread", "fwrite",
          // types defined in stdio.h
          "FILE",
          // macros defined in stdio.h
          "stdin", "stdout", "stderr",
          "BUFSIZ", "EOF", "FILENAME_MAX"},
         {"missing1", "missing2", "missing3", "missing4"});
}


int main() {
  test1();
  test2();
  test3();

  return 0;
}

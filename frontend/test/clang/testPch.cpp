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

#include "chpl/framework/TemporaryFileResult.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/util/clang-integration.h"

#include <unistd.h>

// helper functions

#ifdef HAVE_LLVM
bool gSaveTemp = false;

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
  if (gSaveTemp) {
    config.tmpDir = "tmp";
    config.keepTmpDir = true;
  }
  Context ctx(config);
  Context* context = &ctx;
  context->setQueryTimingFlag(true);

  const TemporaryFileResult* lastResult = nullptr;

  for (int revision = 1; revision <= 2; revision++) {
    ErrorGuard guard(context);
    printf("revision %i\n", revision);
    context->advanceToNextRevision(false);
    auto path = UniqueString::get(context, testName);
    std::string contents = program;
    // replace REVISION with some different code
    {
      std::string find = "REVISION";
      std::string replace;
      if (revision == 2) {
        replace = "var x: int; var y: int;";
      }
      auto index = contents.find(find);
      if (index != std::string::npos) {
        contents.replace(index, find.length(), replace);
        if (revision != 1) {
          // make sure there is no modification time issue
          sleep(1);
        }
      }
    }
    printf("Input %s\n", contents.c_str());
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
    if (lastResult != nullptr) {
      assert(tpch == lastResult && "or else .ast hash changed unexpectedly");
    }
    lastResult = tpch;

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
    guard.printErrors();
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
              REVISION // replaced with some different code in revision 2
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


int main(int argc, char** argv) {
  if (argc != 1) {
    gSaveTemp = true;
  }

  test1();
  test2();
  test3();

  return 0;
}

#else // doesn't HAVE_LLVM

int main() {
  printf("Skipping this test since LLVM/clang support is not enabled\n");
  return 0;
}

#endif // end if HAVE_LLVM

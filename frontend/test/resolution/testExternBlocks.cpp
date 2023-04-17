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
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

#ifdef HAVE_LLVM

// expects program has 1 module and last statement in it is an
// Identifier to check
static void testIt(const char* testName,
                   const char* program,
                   bool identifierIsFromExternBlock) {
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

  auto path = UniqueString::get(context, testName);
  std::string contents = program;
  setFileText(context, path, contents);
  // parse it so that Context knows about the IDs
  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() >= 1);
  const Module* M = vec[vec.size()-1];
  const Identifier* lastIdent = nullptr;

  for (auto stmt : M->stmts()) {
    if (auto ident = stmt->toIdentifier()) {
      lastIdent = ident;
    }
  }
  assert(lastIdent);

  const auto& rrbpid = scopeResolveModule(context, M->id());
  assert(rrbpid.hasAst(lastIdent));
  assert(!rrbpid.byAst(lastIdent).toId().isEmpty());
  ID foundId = rrbpid.byAst(lastIdent).toId();

  if (identifierIsFromExternBlock) {
    assert(foundId.isFabricatedId());
    assert(foundId.fabricatedIdKind() == ID::ExternBlockElement);
  } else {
    assert(!foundId.isFabricatedId());
  }
}

static void test1() {
  testIt("test1.chpl",
         R""""(
            module M {
              extern { int x; }
              x;
            }
         )"""",
         true /* x comes from the extern block */);
}
static void test2() {
  testIt("test2.chpl",
         R""""(
            module M {
              var x: real;
              extern { int x; }
              x;
            }
         )"""",
         false /* x does not come from the extern block */);
}
static void test3() {
  testIt("test3.chpl",
         R""""(
            module M {
              extern { #define FOO 1 }
              FOO;
            }
         )"""",
         true /* FOO comes from the extern block */);
}
static void test4() {
  testIt("test4.chpl",
         R""""(
            module M {
              extern { struct MyStruct { int field; }; }
              MyStruct;
            }
         )"""",
         true /* MyStruct comes from the extern block */);
}
static void test5() {
  testIt("test5.chpl",
         R""""(
            module M {
              extern {
                static void foo(void);
                static void foo(void) { }
              }
              foo;
            }
         )"""",
         true /* foo comes from the extern block */);
}
static void test6() {
  testIt("test6.chpl",
         R""""(
            module Lib {
              extern { static int x; }
            }
            module M {
              use Lib;
              x;
            }
         )"""",
         true /* foo comes from the extern block */);
}
static void test7() {
  testIt("test7.chpl",
         R""""(
            module Lib {
              extern { static int x; }
            }
            module M {
              use Lib only x as y;
              y;
            }
         )"""",
         true /* foo comes from the extern block */);
}




int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();

  return 0;
}
#else // doesn't HAVE_LLVM

int main() {
  printf("Skipping this test since LLVM/clang support is not enabled\n");
  return 0;
}

#endif // end if HAVE_LLVM

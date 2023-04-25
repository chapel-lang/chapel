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
#include "chpl/types/CompositeType.h"

// helper functions



/* Check that when resolving program, catchIdStr maps to the catch block,
    the catch block's error variable has the argIdStr given, and the error
    variable maps to the type given by varIdStr. If expectedErrorCount is given,
    check that the number of errors is as expected. If validateClassType is
    true, check that the error variable's type is a class type with the given
    name in errorTypeName.
*/
static void testIt(const char* testName,
                   const char* program,
                   const char* errorTypeName,
                   const char* catchIdStr,
                   const char* argIdStr,
                   const char* varIdStr,
                   int expectedErrorCount=0,
                   bool validateClassType=true) {
  printf("test %s\n", testName);
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);
  auto path = UniqueString::get(context, testName);
  std::string contents = program;
  setFileText(context, path, contents);
  // parse it so that Context knows about the IDs
  const ModuleVec& vec = parseToplevel(context, path);

  for (auto m : vec) {
    m->dump();
  }

  // TODO: what else, if anything, to verify for the catch block's resolution?
  ID catchId = ID::fromString(context, catchIdStr);
  auto catchAst = parsing::idToAst(context, catchId);
  assert(catchAst);
  assert(catchAst->isCatch());

  if (argIdStr) {
    ID argId = ID::fromString(context, argIdStr);
    auto argAst = parsing::idToAst(context, argId);
    assert(argAst);
    assert(argAst->isVariable());

    auto argRes = resolvedExpressionForAst(context, argAst, nullptr, false);
    auto argType = argRes->type().type();
    assert(argType);
    if (validateClassType) {
      assert(argType->isClassType());
      auto argName = argType->toClassType()->basicClassType()->name();
      assert(argName == UniqueString::get(context, errorTypeName));
    }
  }

  if (varIdStr) {
    ID varId = ID::fromString(context, varIdStr);
    auto varAst = parsing::idToAst(context, varId);
    assert(varAst);
    assert(varAst->isIdentifier());
    auto varRes = resolvedExpressionForAst(context, varAst, nullptr, false);
    auto varType = varRes->type().type();
    assert(varType);
    if (validateClassType) {
      assert(varType->isClassType());
      auto varName = varType->toClassType()->basicClassType()->name();
      assert(varName == UniqueString::get(context, errorTypeName));
    }
  }

  if (expectedErrorCount > 0) {
    assert(guard.realizeErrors() == expectedErrorCount);
  }
}


static void test1() {
  // test for a catch block with explicit error variable that inherits from
  // Error.
  testIt("test1.chpl",
         R""""(
            module M {
              class SomeErrorName: Error { }
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e : SomeErrorName { }
              }
            }
         )"""",
         "SomeErrorName",
         "M.main@6",
         "M.main@4",
         "M.main@3");
}

static void test2() {
  // test for a catch block with explicit error variable that inherits from
  // Error. Includes an assignment of the variable to a new variable in the block
  testIt("test2.chpl",
         R""""(
            module M {
              class SomeErrorName: Error { }
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e : SomeErrorName {
                  var j = e;
                }
              }
            }
         )"""",
         "SomeErrorName",
         "M.main@8",
         "M.main@4",
         "M.main@3");
}

static void test3() {
  // test for a catch-all block with explicit error variable but no type
  testIt("test3.chpl",
         R""""(
            module M {
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e {  }
              }
            }
         )"""",
         "Error",
         "M.main@5",
         "M.main@3",
         nullptr);
}

static void test4() {
  // test for a catch-all block with explicit error variable and type
  testIt("test4.chpl",
         R""""(
            module M {
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e : Error {  }
              }
            }
         )"""",
         "Error",
         "M.main@6",
         "M.main@4",
         "M.main@3");
}

static void test5() {
  // test for a catch-all block with no error variable
  testIt("test5.chpl",
         R""""(
            module M {
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch {  }
              }
            }
         )"""",
         nullptr,
         "M.main@4",
         nullptr,
         nullptr);
}

static void test6() {
// test for a bad error type (int) in the catch block
  std::string testName="test6.chpl";
  auto program =
         R""""(
            module M {
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e : int {  }
              }
            }
         )"""";
         testIt(testName.c_str(), program,
                "int",
                "M.main@6",
                "M.main@4",
                "M.main@3",
                1,
                false);
}

static void test7() {
  // test for a bad error type (MyClass doesn't inherit from Error) in the catch block
  std::string testName="test7.chpl";
  auto program =
         R""""(
            module M {
              proc foo() throws { }
              class MyClass {}
              proc main() {
                try {
                  foo();
                } catch e : MyClass {  }
              }
            }
         )"""";
    testIt(testName.c_str(), program,
        "MyClass",
        "M.main@6",
        "M.main@4",
        "M.main@3",
        1,
        true);
}

static void test8() {
  // test nested try-catch, call testIt twice and pass values for outer and
  // inner try-catch, respectively
  testIt("test8a.chpl",
         R""""(
          module M {
          proc foo() throws { }
          proc main() {
            try {
              try {
                foo();
              } catch e : Error {
                throw e;
              }
              foo();
            } catch e {

            }
          }
         }
         )"""",
         "Error",
         "M.main@8",
         "M.main@4",
         "M.main@3");

  testIt("test8b.chpl",
         R""""(
          module M {
          proc foo() throws { }
          proc main() {
            try {
              try {
                foo();
              } catch e : Error {
                throw e;
              }
              foo();
            } catch e {

            }
          }
         }
         )"""",
         "Error",
         "M.main@15",
         "M.main@13",
         nullptr);
}

static void test9() {
  // test for a catch block with explicit error variable with multiple
  // layers of inheritance before Error.
  testIt("test9.chpl",
         R""""(
            module M {
              class SomeErrorName: Error { }
              class AnotherErrorName: SomeErrorName { }
              proc foo() throws { }
              proc main() {
                try {
                  foo();
                } catch e : AnotherErrorName { }
              }
            }
         )"""",
         "AnotherErrorName",
         "M.main@6",
         "M.main@4",
         "M.main@3");
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();

  return 0;
}

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
#include "chpl/resolution/copy-elision.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/resolution/split-init.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

// resolves the last function, or module if testModule=true
// checks that the copy elision points match the string IDs provided
static void testCopyElision(const char* test,
                            const char* program,
                            std::vector<const char*> expectedPoints,
                            bool testModule=false,
                            bool expectErrors=false) {
  printf("%s\n", test);

  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string testname = test;
  testname += ".chpl";
  auto path = UniqueString::get(context, testname);
  std::string contents = program;
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* M = vec[0]->toModule();
  assert(M);
  assert(M->numStmts() >= 1);

  const Function* func = M->stmt(M->numStmts()-1)->toFunction();

  if (func) {
    func->dump();
  } else {
    M->dump();
  }

  std::set<ID> splitIds;
  std::set<ID> elisionPoints;

  if (testModule == false) {
    assert(func);
    printf("Resolving function %s\n", func->name().c_str());
    const ResolvedFunction* r = resolveConcreteFunction(context, func->id());
    assert(r);

    splitIds = computeSplitInits(context, func, r->resolutionById());

    elisionPoints = computeElidedCopies(context, func,
                                        r->resolutionById(),
                                        /* poiScope */ nullptr,
                                        splitIds,
                                        r->returnType());
  } else {
    printf("Resolving module\n");
    // check the module init function
    const ResolutionResultByPostorderID& rr = resolveModule(context, M->id());

    splitIds = computeSplitInits(context, M, rr);

    elisionPoints = computeElidedCopies(context, M,
                                        rr,
                                        /* poiScope */ nullptr,
                                        splitIds,
                                        QualifiedType(QualifiedType::VAR,
                                                      VoidType::get(context)));
  }

  std::set<std::string> pointNames;
  for (auto id : elisionPoints) {
    pointNames.insert(id.str());
  }

  std::set<std::string> expectNames;
  for (auto cstr : expectedPoints) {
    expectNames.insert(std::string(cstr));
  }

  // check that each thing in expectNames is in splitNames
  for (auto expectName : expectNames) {
    if (pointNames.count(expectName) == 0) {
      printf("%s: missing expected point for '%s'\n",
             test, expectName.c_str());
    }
  }

  // check that each thing in splitNames is in expectNames
  for (auto pointName : pointNames) {
    if (expectNames.count(pointName) == 0) {
      printf("%s: found unexpected point for '%s'\n", test, pointName.c_str());
    }
  }

  assert(expectNames == pointNames);

  size_t errCount = guard.realizeErrors();
  if (expectErrors) {
    assert(errCount > 0);
  } else {
    assert(errCount == 0);
  }
}

static void test1() {
  testCopyElision("test1",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int = 0;
        }
      }
    )"""",
    {});
}

static void test2() {
  testCopyElision("test2",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x;
          x = 1;
        }
      }
    )"""",
    {});
}

static void test3() {
  testCopyElision("test3",
    R""""(
      module M {
        proc test() {
          var x:int;
          var y = x;
        }
      }
    )"""",
    {"M.test@3"});
}


static void test4() {
  testCopyElision("test4",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          var y = x;
          x;
        }
      }
    )"""",
    {});
}

static void test5() {
  testCopyElision("test5",
    R""""(
      module M {
        proc test(cond: bool) {
          var x:int;
          if cond {
            var y = x;
            return;
          }
          x;
        }
      }
    )"""",
    {"M.test@6"});
}

static void test6() {
  testCopyElision("test6",
    R""""(
      module M {
        proc test(cond: bool) {
          var x:int;
          var y;
          if cond {
            y = x;
          } else {
            y = x;
          }
        }
      }
    )"""",
    {"M.test@8", "M.test@12"});
}

static void test7() {
  testCopyElision("test7",
    R""""(
      module M {
        proc test() {
          var i: int;
          ref r = i;
        }
      }
    )"""",
    {});
}

static void test8() {
  testCopyElision("test8",
    R""""(
      module M {
        proc test() {
          var i: int;
          ref r = i;
          ref rr = r;
        }
      }
    )"""",
    {});
}

static void test9() {
  testCopyElision("test9",
    R""""(
      module M {
        proc fIn(arg: int) { }
        proc test() {
          var x:int;
          fIn(x);
        }
      }
    )"""",
    {"M.test@3"});
}

static void test10() {
  testCopyElision("test10",
    R""""(
      module M {
        proc fIn(arg: int) { }
        proc test() {
          var x:int;
          fIn(x);
          x;
        }
      }
    )"""",
    {});
}

static void test11() {
  testCopyElision("test11",
    R""""(
      module M {
        proc test() {
          var x:int;
          var y;
          y = x;
        }
      }
    )"""",
    {"M.test@5"});
}

static void test12() {
  testCopyElision("test12",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          var y;
          y = x;
          y = x;
        }
      }
    )"""",
    {});
}

static void test13() {
  testCopyElision("test13",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(cond: bool) {
          var x:int;
          if cond {
            var y = x;
          }
        }
      }
    )"""",
    {});
}

static void test14() {
  testCopyElision("test14",
    R""""(
      module M {
        proc test(cond: bool) {
          var x:int;
          var y;
          if cond {
            return;
          } else {
            y = x;
          }
        }
      }
    )"""",
    {"M.test@10"});
}

static void test15() {
  testCopyElision("test15",
    R""""(
      module M {
        proc test(cond: bool) {
          var x:int;
          var y;
          if cond {
            y = x;
          } else {
            return;
          }
        }
      }
    )"""",
    {"M.test@8"});
}

static void test16() {
  testCopyElision("test16",
    R""""(
      module M {
        proc test() {
          var x:int;
          try {
            var y = x;
          }
        }
      }
    )"""",
    {"M.test@3"});
}

static void test17() {
  testCopyElision("test17",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          var y;
          try {
            y = x;
          } catch {
          }
        }
      }
    )"""",
    {});
}

static void test18() {
  testCopyElision("test18",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          var y;
          try {
            y = x;
          } catch {
            y;
          }
        }
      }
    )"""",
    {});
}

static void test19() {
  testCopyElision("test19",
    R""""(
      module M {
        proc test() {
          var x:int;
          var y;
          try {
            y = x;
          } catch {
            return;
          }
        }
      }
    )"""",
    {"M.test@5"});
}

static void test20() {
  testCopyElision("test20",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          var y;
          try {
            y = x;
          } catch {
            y = x;
          }
        }
      }
    )"""",
    {});
}

static void test21() {
  testCopyElision("test21",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          var y;
          try {
          } catch {
            y = x;
          }
        }
      }
    )"""",
    {});
}

static void test22() {
  testCopyElision("test22",
    R""""(
      module M {
        proc test() {
          try {
          } catch {
            var x:int;
            var y = x;
          }
        }
      }
    )"""",
    {"M.test@4"});
}

static void test23() {
  testCopyElision("test23",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        record R {
          var field: int;
        }
        proc R.test() {
          var x = field;
        }
      }
    )"""",
    {});
}

static void test24() {
  // module scope variables aren't subject to copy elision
  testCopyElision("test24",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        var x: int;
        var y = x;
      }
    )"""",
    {},
    /* resolveModule */ true);
}

static void test25() {
  // module scope variables aren't subject to copy elision
  testCopyElision("test25",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        var x: int;
        proc test() {
          var y = x;
        }
      }
    )"""",
    {});
}

// test that copy elision only applies to the first assignment
// when using split init
static void test26() {
  testCopyElision("test26",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x: int;
          var y: int;
          var z;
          z = x;
          z = y;
        }
      }
    )"""",
    {"M.test@7"});
}
// including with an inner block
static void test27() {
  testCopyElision("test27",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x: int;
          var y: int;
          var z;
          {
            z = x;
          }
          z = y;
        }
      }
    )"""",
    {"M.test@7"});
}
// including with a conditional
static void test28() {
  testCopyElision("test28",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        config const cond = true;

        proc test() {
          var a: int;
          var z;
          if cond {
            z = a;
          } else {
            z = a;
          }
          z = c;
        }
      }
    )"""",
    {"M.test@6", "M.test@10"});
}
// including with a try/catch
static void test29() {
  testCopyElision("test29",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        config const cond = true;

        proc test() {
          var a: int;
          var b: int;
          var z;
          try {
            z = a;
          } catch {
            return;
          }
          z = b;
        }
      }
    )"""",
    {"M.test@7"});
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
  test10();
  test11();
  test12();
  test13();
  test14();
  test15();
  test16();
  test17();
  test18();
  test19();
  test20();
  test21();
  test22();
  test23();
  test24();
  test25();
  test26();
  test27();
  test28();
  test29();

  return 0;
}

/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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
  std::string contents = "module M {\n";
  contents += "operator =(ref lhs: int, const rhs: int){}\n";
  contents += program;
  contents += "\n}";
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

        proc test() {
          var x:int = 0;
        }
    )"""",
    {});
}

static void test2() {
  testCopyElision("test2",
    R""""(

        proc test() {
          var x;
          x = 1;
        }
    )"""",
    {});
}

static void test3() {
  testCopyElision("test3",
    R""""(
        proc test() {
          var x:int;
          var y = x;
        }
    )"""",
    {"M.test@3"});
}


static void test4() {
  testCopyElision("test4",
    R""""(

        proc test() {
          var x:int;
          var y = x;
          x;
        }
    )"""",
    {});
}

static void test5() {
  testCopyElision("test5",
    R""""(
        proc test(cond: bool) {
          var x:int;
          if cond {
            var y = x;
            return;
          }
          x;
        }
    )"""",
    {"M.test@6"});
}

static void test6() {
  testCopyElision("test6",
    R""""(
        proc test(cond: bool) {
          var x:int;
          var y;
          if cond {
            y = x;
          } else {
            y = x;
          }
        }
    )"""",
    {"M.test@8", "M.test@12"});
}

static void test7() {
  testCopyElision("test7",
    R""""(
        proc test() {
          var i: int;
          ref r = i;
        }
    )"""",
    {});
}

static void test8() {
  testCopyElision("test8",
    R""""(
        proc test() {
          var i: int;
          ref r = i;
          ref rr = r;
        }
    )"""",
    {});
}

static void test9() {
  testCopyElision("test9",
    R""""(
        proc fIn(arg: int) { }
        proc test() {
          var x:int;
          fIn(x);
        }
    )"""",
    {"M.test@3"});
}

static void test10() {
  testCopyElision("test10",
    R""""(
        proc fIn(arg: int) { }
        proc test() {
          var x:int;
          fIn(x);
          x;
        }
    )"""",
    {});
}

static void test11() {
  testCopyElision("test11",
    R""""(
        proc test() {
          var x:int;
          var y;
          y = x;
        }
    )"""",
    {"M.test@5"});
}

static void test12() {
  testCopyElision("test12",
    R""""(

        proc test() {
          var x:int;
          var y;
          y = x;
          y = x;
        }
    )"""",
    {});
}

static void test13() {
  testCopyElision("test13",
    R""""(

        proc test(cond: bool) {
          var x:int;
          if cond {
            var y = x;
          }
        }
    )"""",
    {});
}

static void test14() {
  testCopyElision("test14",
    R""""(
        proc test(cond: bool) {
          var x:int;
          var y;
          if cond {
            return;
          } else {
            y = x;
          }
        }
    )"""",
    {"M.test@10"});
}

static void test15() {
  testCopyElision("test15",
    R""""(
        proc test(cond: bool) {
          var x:int;
          var y;
          if cond {
            y = x;
          } else {
            return;
          }
        }
    )"""",
    {"M.test@8"});
}

static void test16() {
  testCopyElision("test16",
    R""""(
        proc test() throws {
          var x:int;
          try {
            var y = x;
          }
        }
    )"""",
    {"M.test@3"});
}

static void test17() {
  testCopyElision("test17",
    R""""(

        proc test() {
          var x:int;
          var y;
          try {
            y = x;
          } catch {
          }
        }
    )"""",
    {});
}

static void test18() {
  testCopyElision("test18",
    R""""(

        proc test() {
          var x:int;
          var y;
          try {
            y = x;
          } catch {
            y;
          }
        }
    )"""",
    {});
}

static void test19() {
  testCopyElision("test19",
    R""""(
        proc test() {
          var x:int;
          var y;
          try {
            y = x;
          } catch {
            return;
          }
        }
    )"""",
    {"M.test@5"});
}

static void test20() {
  testCopyElision("test20",
    R""""(

        proc test() {
          var x:int;
          var y;
          try {
            y = x;
          } catch {
            y = x;
          }
        }
    )"""",
    {});
}

static void test21() {
  testCopyElision("test21",
    R""""(

        proc test() {
          var x:int;
          var y;
          try {
          } catch {
            y = x;
          }
        }
    )"""",
    {});
}

static void test22() {
  testCopyElision("test22",
    R""""(
        proc test() {
          try {
          } catch {
            var x:int;
            var y = x;
          }
        }
    )"""",
    {"M.test@4"});
}

static void test23() {
  testCopyElision("test23",
    R""""(
        record R {
          var field: int;
        }
        proc R.test() {
          var x = field;
        }
    )"""",
    {});
}

static void test24() {
  // module scope variables aren't subject to copy elision
  testCopyElision("test24",
    R""""(
        var x: int;
        var y = x;
    )"""",
    {},
    /* resolveModule */ true);
}

static void test25() {
  // module scope variables aren't subject to copy elision
  testCopyElision("test25",
    R""""(
        var x: int;
        proc test() {
          var y = x;
        }
    )"""",
    {});
}

// test that copy elision only applies to the first assignment
// when using split init
static void test26() {
  testCopyElision("test26",
    R""""(

        proc test() {
          var x: int;
          var y: int;
          var z;
          z = x;
          z = y;
        }
    )"""",
    {"M.test@7"});
}
// including with an inner block
static void test27() {
  testCopyElision("test27",
    R""""(

        proc test() {
          var x: int;
          var y: int;
          var z;
          {
            z = x;
          }
          z = y;
        }
    )"""",
    {"M.test@7"});
}
// including with a conditional
static void test28() {
  testCopyElision("test28",
    R""""(

        config const cond = true;

        proc test() {
          var a: int;
          var c: int;
          var z;
          if cond {
            z = a;
          } else {
            z = a;
          }
          z = c;
        }
    )"""",
    {"M.test@8", "M.test@12"});
}
// including with a try/catch
static void test29() {
  testCopyElision("test29",
    R""""(

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
    )"""",
    {"M.test@7"});
}

// out variable can't be copy elided from b/c it is mentioned by return
static void test30() {
  testCopyElision("test30",
    R""""(
        proc test(out x: int) {
          var y = x;
          return;
        }
    )"""",
    {});
}
static void test31() {
  testCopyElision("test31",
    R""""(
        proc test(out x: int) {
          var y = x;
        }
    )"""",
    {});
}
static void test32() {
  testCopyElision("test32",
    R""""(
        proc test(inout x: int) {
          var y = x;
          return;
        }
    )"""",
    {});
}
static void test33() {
  testCopyElision("test33",
    R""""(
        proc test(inout x: int) {
          var y = x;
        }
    )"""",
    {});
}

static void test34() {
  testCopyElision("test34",
    R""""(

        config const cond = true;

        proc test() {
          var a: int;
          var c: int;
          var z;
          if true {
            z = a;
          } else {
            z = a;
          }
          z = c;
        }
    )"""",
    {"M.test@8"});
}

static void test35() {
  testCopyElision("test35a1",
    R""""(
        config const cond = true;
        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          {x;}
        }
    )"""",
    {});
  testCopyElision("test35a2",
    R""""(
        config const cond = true;
        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          if cond {
            y;
          } else {
            x;
          }
        }
    )"""",
    {});
  
  testCopyElision("test35a",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          if T == int {
            y;
          } else {
            x;
          }
        }
    )"""",
    {});
  testCopyElision("test35b",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = int;
          if T == int {
            y;
          } else {
            x;
          }
        }
    )"""",
    {"M.test@4"});
}

static void test36() {
  testCopyElision("test36a",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          if T == int {
            y;
          }
        }
    )"""",
    {"M.test@4"});
  testCopyElision("test36b",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          if T == int {
            x;
          }
        }
    )"""",
    {"M.test@4"});
  testCopyElision("test36c",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = int;
          if T == int {
            y;
          }
        }
    )"""",
    {"M.test@4"});
  testCopyElision("test36d",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = int;
          if T == int {
            x;
          }
        }
    )"""",
    {});
}

static void test37() {
  testCopyElision("test37a",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          select T {
            when int do x;
            when real do y;
            otherwise do x;
          }
        }
    )"""",
    {"M.test@4"});
  testCopyElision("test37b",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = int;
          select T {
            when int do x;
            when real do y;
            otherwise do x;
          }
        }
    )"""",
    {});
  testCopyElision("test37c",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = string;
          select T {
            when int do x;
            when real do y;
            otherwise do x;
          }
        }
    )"""",
    {});
}

static void test38() {
  testCopyElision("test38a",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          select T {
            when int do x;
            when real do y;
          }
        }
    )"""",
    {"M.test@4"});
  testCopyElision("test38b",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = int;
          select T {
            when int do x;
            when real do y;
          }
        }
    )"""",
    {});
  testCopyElision("test38c",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = string;
          select T {
            when int do x;
            when real do y;
          }
        }
    )"""",
    {"M.test@4"});
}

static void test39() {
  testCopyElision("test39a",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = int;
          select T {
            when int {
              var x: int;
              x;
            }
            when real do y;
          }
        }
    )"""",
    {"M.test@4"});
  testCopyElision("test39b",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          select T {
            when int {
              var x;
              x;
            }
            when real do y;
          }
        }
    )"""",
    {"M.test@4"});
  testCopyElision("test39c",
    R""""(
        config const cond = true;
        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          select T {
            when int {
              var x;
              x;
            }
            when real {
              if cond {
                x;
              } else {
                y;
              }
            }
          }
        }
    )"""",
    {});
  testCopyElision("test39d",
    R""""(
        config const cond = true;
        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          select T {
            when int {
              var x;
              x;
            }
            when real {
              if false {
                x;
              } else {
                y;
              }
            }
          }
        }
    )"""",
    {"M.test@4"});
}

static void test40() {
  testCopyElision("test40a",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          try {
            y;
          } catch {
            x;
          }
        }
    )"""",
    {});
  testCopyElision("test40b",
    R""""(

        proc test() {
          var x: int = 0;
          var y = x;
          try {
            y;
          } catch {
            var x: int;
          }


        }
    )"""",
    {"M.test@4"});
  testCopyElision("test40c",
    R""""(

        proc test() {
          var x: int = 0;
          try {
            var y = x;
            y;
          } catch {
            var x: int;
            x;
            return;
          }

        }
    )"""",
    {"M.test@4"});
  testCopyElision("test40d",
    R""""(

        proc test() {
          var x: int = 0;
          try {
            var y = x;
            y;
          } catch {
            x;
            return;
          }
        }
    )"""",
    {});
  testCopyElision("test40e",
    R""""(

        proc test() {
          var x: int = 0;
          try {
            var y = x;
            y;
          } catch {
            x;
          }

        }
    )"""",
    {});
  testCopyElision("test40e",
    R""""(

        proc test() {
          var x: int = 0;
          try {
            var y = x;
            y;
          } catch {
            return;
          }

        }
    )"""",
    {"M.test@4"});
  testCopyElision("test40f",
    R""""(

        proc test() {
          var x: int = 0;
          try {
            var y = x;
            y;
          } catch {
            return;
          }
          x;
          }
    )"""",
    {});
}

static void test41() {
  testCopyElision("test41a",
    R""""(
        operator ==(ref lhs: int, rhs: int) {
          __primitive("==", lhs, rhs);
        }
        config const cond = 2;
        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          select cond {
            when 1 {
              x;
            }
            when 2 {
              y;
            }
          }
        }
    )"""",
    {});
  testCopyElision("test41b",
    R""""(
        operator ==(ref lhs: int, rhs: int) {
          __primitive("==", lhs, rhs);
        }
        config const cond = 2;
        proc test() {
          var x: int = 0;
          var y = x;
          type T = real;
          select cond {
            when 1 {
              y;
            }
            when 2 {
              y;
            }
            otherwise {
              x;
            }
          }
        }
    )"""",
    {});
}

static void test42() {
  testCopyElision("test42a",
    R""""(
        operator ==(ref lhs: int, rhs: int) {
          __primitive("==", lhs, rhs);
        }

        proc test() {
          var c: int = 0;
          var x: int = 0;
          var y: int;
          select 2 {
            when c {
              y=x;
            }
            when 2 {

            }
          }
        }
    )"""",
    {});
  testCopyElision("test42b",
    R""""(
        operator ==(ref lhs: int, rhs: int) {
          __primitive("==", lhs, rhs);
        }

        proc test() {
          var c: int = 0;
          var x: int = 0;
          var y: int;
          select 2 {
            when c {
              y=x;
            }
            when 2 {
              y=x;
            }
          }
        }
    )"""",
    {"M.test@12","M.test@18"});
}

static void test43() {
  testCopyElision("test43a",
    R""""(
        operator ==(ref lhs: int, rhs: int) {
          __primitive("==", lhs, rhs);
        }

        proc test() {
          var c: int = 0;
          var x: int = 0;
          var y = x;
          select 2 {
            when c {
              y;
            }
            when 2 {
              x;
            }
          }
        }
    )"""",
    {});
  testCopyElision("test43b",
    R""""(
        operator ==(ref lhs: int, rhs: int) {
          __primitive("==", lhs, rhs);
        }

        proc test() {
          var c: int = 0;
          var x: int = 0;
          var y = x;
          select 2 {
            when c {
              y;
            }
            when 2 {
              y;
            }
            when 3 {
              x;
            }
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
  test30();
  test31();
  test32();
  test33();
  test34();
  test35();
  test36();
  test37();
  test38();
  test39();
  test40();
  test41();
  test42();
  test43();
  return 0;
}

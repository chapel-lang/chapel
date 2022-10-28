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

#include "test-resolution.h"

#include "chpl/resolution/split-init.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

#include "./ErrorGuard.h"

// resolves the last function
// checks that the set of split init variables matches
// the array of variable names
static void testSplitInit(const char* test,
                          const char* program,
                          std::vector<const char*> expectedSplitInits) {
  printf("%s\n", test);

  Context ctx;
  Context* context = &ctx;

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
  assert(func);

  // resolve runM1
  const ResolvedFunction* r = resolveConcreteFunction(context, func->id());
  assert(r);

  std::set<ID> splitIds = computeSplitInits(context, func, r->resolutionById());
  std::set<std::string> splitNames;
  for (auto id : splitIds) {
    auto ast = idToAst(context, id);
    auto vl = ast->toVarLikeDecl();
    splitNames.insert(vl->name().str());
  }

  std::set<std::string> expectNames;
  for (auto cstr : expectedSplitInits) {
    expectNames.insert(std::string(cstr));
  }

  // check that each thing in expectNames is in splitNames
  for (auto expectName : expectNames) {
    if (splitNames.count(expectName) == 0) {
      printf("%s: missing expected split init for '%s'\n",
             test, expectName.c_str());
    }
  }

  // check that each thing in splitNames is in expectNames
  for (auto splitName : splitNames) {
    if (expectNames.count(splitName) == 0) {
      printf("%s: unexpected split init for '%s'\n", test, splitName.c_str());
    }
  }

  assert(expectNames == splitNames);

  // check that all split-inited variables have type 'int'
  auto intType = IntType::get(context, 0);
  for (auto id : splitIds) {
    QualifiedType varType = r->byId(id).type();
    if (varType.type() != intType) {
      auto ast = idToAst(context, id);
      auto vl = ast->toVarLikeDecl();
      auto name = vl->name().str();
      printf("%s: variable '%s' with id %s has unexpected type ",
             test, name.c_str(), id.str().c_str());
      varType.dump();
      printf(" rather than type 'int'\n");
    }
    assert(varType.type() == IntType::get(context, 0));
  }
}

static void test1() {
  testSplitInit("test1",
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
  testSplitInit("test2",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var yes1;
          yes1 = 1;
        }
      }
    )"""",
    {"yes1"});
}

static void test3() {
  testSplitInit("test3",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var yes1:int;
          yes1 = 1;
        }
      }
    )"""",
    {"yes1"});
}


static void test4() {
  testSplitInit("test4",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int = 0;
          var yes2;
          {
            x = 24;
            yes2 = 2;
          }
        }
      }
    )"""",
    {"yes2"});
}

static void test5() {
  testSplitInit("test5",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(cond: bool) {
          var x:int = 0;
          var yes3;
          var no: int;
          {
            if cond {
              yes3 = 3;
            } else {
              x = 99;
              x = 123;
              {
                yes3 = 3;
              }
            }
          }
          if cond {
            no = 22;
          }
        }
      }
    )"""",
    {"yes3"});
}

static void test6() {
  testSplitInit("test6",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(cond: bool, otherCond: bool) {
          var yes5;
          if cond {
            yes5 = 5;
          } else if otherCond {
            yes5 = 55;
          } else {
            yes5 = 555;
          }
        }
      }
    )"""",
    {"yes5"});
}

static void test7() {
  testSplitInit("test7",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var no1 = 4;
          no1 = 5;
        }
      }
    )"""",
    {});
}

static void test8() {
  testSplitInit("test8",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          {
            var no2:int;
            var tmp = no2;
            no2 = 57;
          }
        }
      }
    )"""",
    {});
}

static void test9() {
  testSplitInit("test9",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          {
            var no3:int;
            var tmp:no3.type;
            no3 = 57;
          }
        }
      }
    )"""",
    {});
}

static void test10() {
  testSplitInit("test10",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(cond: bool) {
          var x;
          if cond then
            return;
          x = 11;
        }
      }
    )"""",
    {"x"});
}

static void test11() {
  testSplitInit("test11",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(cond: bool) {
          var x;
          if cond then
            throw nil;
          x = 11;
        }
      }
    )"""",
    {"x"});
}

static void test12() {
  testSplitInit("test12",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          try {
            {
              x = 1;
            }
          } catch {
            return;
          }
        }
      }
    )"""",
    {"x"});
}

static void test13() {
  testSplitInit("test13",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          try {
            {
              x = 1;
            }
          } catch {
          }
        }
      }
    )"""",
    {});
}

static void test14() {
  testSplitInit("test14",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          try {
            {
              x = 1;
            }
          } catch {
            x = 1;
          }
        }
      }
    )"""",
    {});
}

static void test15() {
  testSplitInit("test15",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          try {
          } catch {
            x = 1;
          }
        }
      }
    )"""",
    {});
}

static void test16() {
  testSplitInit("test16",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(out formal: int) {
          formal = 4;
        }
      }
    )"""",
    {"formal"});
}

static void test17() {
  testSplitInit("test17",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc fOut(out formal: int) { formal = 4; }
        proc test() {
          var x:int;
          fOut(x);
        }
      }
    )"""",
    {"x"});
}

static void test18() {
  testSplitInit("test18",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc fOut(out formal: int) { formal = 4; }
        proc test() {
          var x;
          fOut(x);
        }
      }
    )"""",
    {"x"});
}

static void test19() {
  testSplitInit("test19",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc int.fOut(out formal: int) { formal = 4; }
        proc test() {
          var myInt = 4;
          var x;
          myInt.fOut(x);
        }
      }
    )"""",
    {"x"});
}

// test out intent combined with varargs formals oh my!
static void test20() {
  testSplitInit("test20",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        operator =(ref lhs: (int,), rhs: (int,)) {
        }

        proc fOut(out formals:int...) {
          formals = (4,);
        }
        proc test() {
          var x;
          fOut(x);
        }
      }
    )"""",
    {"x"});
}
static void test21() {
  testSplitInit("test21",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        operator =(ref lhs: (int,int), rhs: (int,int)) {
        }

        proc fOut(out formals:int...) {
          formals = (4,5);
        }
        proc test() {
          var x;
          var y;
          fOut(x, y);
        }
      }
    )"""",
    {"x", "y"});
}
static void test22() {
  testSplitInit("test22",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        operator =(ref lhs: (int,), rhs: (int,)) {
        }

        proc fOut(out formals...) {
          formals = (4,);
        }
        proc test() {
          var x:int;
          fOut(x);
        }
      }
    )"""",
    {"x"});
}
static void test23() {
  testSplitInit("test23",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        operator =(ref lhs: (int,int), rhs: (int,int)) {
        }

        proc fOut(out formals...) {
          formals = (4,5);
        }
        proc test() {
          var x:int;
          var y:int;
          fOut(x, y);
        }
      }
    )"""",
    {"x", "y"});
}
/*static void test24() {
  testSplitInit("test24",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        operator =(ref lhs: (int,), rhs: (int,)) {
        }

        proc fOut(out formals...) {
          formals = (4,);
        }
        proc test() {
          var x;
          fOut(x);
        }
      }
    )"""",
    {"x"});
}
static void test25() {
  testSplitInit("test25",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        operator =(ref lhs: (int,int), rhs: (int,int)) {
        }

        proc fOut(out formals...) {
          formals = (4,5);
        }
        proc test() {
          var x;
          var y;
          fOut(x, y);
        }
      }
    )"""",
    {"x", "y"});
}*/

static void test26a() {
  // check that this program generates an error
  const char* test = "test26a";
  const char* program =
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        proc test(r: bool) {
          var x;
          if r {
            x = 2;
          } else {
            x = 3.0i;
          }
        }
      }
    )"""";

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
  assert(func);

  // resolve runM1
  const ResolvedFunction* r = resolveConcreteFunction(context, func->id());
  assert(r);

  assert(guard.errors().size() > 0);
  guard.realizeErrors();
}

static void test26b() {
  // check that this program generates an error
  const char* test = "test26b";
  const char* program =
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        operator =(ref lhs: int(8), rhs: int(8)) {
          __primitive("=", lhs, rhs);
        }

        proc test(r: bool) {
          var x;
          if r {
            var myInt8: int(8);
            x = myInt8;
          } else {
            x = 3;
          }
        }
      }
    )"""";

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
  assert(func);

  // resolve runM1
  const ResolvedFunction* r = resolveConcreteFunction(context, func->id());
  assert(r);

  assert(guard.errors().size() > 0);
  guard.realizeErrors();
}

static void test26c() {
  // check that this program generates an error
  const char* test = "test26c";
  const char* program =
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        operator =(ref lhs: int(8), rhs: int(8)) {
          __primitive("=", lhs, rhs);
        }

        proc test(r: bool) {
          var x;
          if r {
            x = 2;
          } else {
            var myInt8: int(8);
            x = myInt8;
          }
        }
      }
    )"""";

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
  assert(func);

  // resolve runM1
  const ResolvedFunction* r = resolveConcreteFunction(context, func->id());
  assert(r);

  assert(guard.errors().size() > 0);
  guard.realizeErrors();
}



static void test27() {
  testSplitInit("test27",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(ref arg: int) {
          var x:int;
          on arg {
            x = 52;
          }
        }
      }
    )"""",
    {});
}

static void test28() {
  testSplitInit("test28",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(ref arg: int) {
          var x:int;
          local {
            x = 52;
          }
        }
      }
    )"""",
    {"x"});
}

static void test29() {
  testSplitInit("test29",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(ref arg: int) {
          var x:int;
          serial {
            x = 52;
          }
        }
      }
    )"""",
    {"x"});
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
  //test24(); TODO
  //test25(); TODO
  test26a();
  test26b();
  test26c();
  test27();
  test28();
  test29();

  return 0;
}

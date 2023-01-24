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
#include "chpl/resolution/split-init.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

static const bool ERRORS_EXPECTED=true;

// resolves the last function
// checks that the set of split init variables matches
// the array of variable names
static void testSplitInit(const char* test,
                          const char* program,
                          std::vector<const char*> expectedSplitInits,
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

  size_t errCount = guard.realizeErrors();
  if (expectErrors) {
    assert(errCount > 0);
  } else {
    assert(errCount == 0);
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
    {"x"},
    /* expect errors for now as a temporary workaround */ ERRORS_EXPECTED);
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
    {"x", "y"},
    /* expect errors for now as a temporary workaround */ ERRORS_EXPECTED);
}

/* not tested -- "ount intent varargs are not supported"
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

static void test24() {
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
  testSplitInit("test26a",
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
    )"""",
    {"x"}, ERRORS_EXPECTED);
}

static void test26b() {
  testSplitInit("test26b",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }
        operator =(ref lhs: real, rhs: real) {
          __primitive("=", lhs, rhs);
        }
        operator =(ref lhs: int(8), rhs: int(8)) {
          __primitive("=", lhs, rhs);
        }

        proc test(r: bool) {
          var x;
          if r {
            x = 1;
          } else {
            x = 3.0;
          }
        }
      }
    )"""",
    {"x"}, ERRORS_EXPECTED);
}

static void test26c() {
  testSplitInit("test26c",
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
    )"""",
    {"x"}, ERRORS_EXPECTED);
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

        proc test() {
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

        proc test() {
          var x:int;
          serial {
            x = 52;
          }
        }
      }
    )"""",
    {"x"});
}

static void test30() {
  testSplitInit("test30",
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
          var x, y;
          if r {
            x = 2;
            y = 3;
          } else {
            y = 5;
            x = 6;
          }
        }
      }
    )"""",
    {"x", "y"}, ERRORS_EXPECTED);
}

/* Uncomment these tests after we have nested functions implemented
static void test31() {
  testSplitInit("test31",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x: int;
          inner1();
          x = 1;
          proc inner1() {
            x;
          }
        }
      }
    )"""",
    {});
}

static void test32() {
  testSplitInit("test32",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x: int;
          proc inner1() {
            x;
          }
          x = 1;
          inner1();
        }
      }
    )"""",
    {});
}

static void test33() {
  testSplitInit("test33",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x: int;
          inner1();
          x;
          proc inner1() {
            x = 44;
          }
        }
      }
    )"""",
    {});
}

static void test34() {
  testSplitInit("test34",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x: int;
          proc inner1() {
            x = 44;
          }
          x;
          inner1();
        }
      }
    )"""",
    {});
}
*/

/* Uncomment these tests after we have return intent overloading implemented
static void test35() {
  testSplitInit("test35",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        var g: int;
        proc f(out x: int) const ref { return g; }
        proc f(    x: int)       ref { return g; }
        proc test() {
          var x;
          f(x);
        }
      }
    )"""",
    {});
}

static void test36() {
  testSplitInit("test36",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        var g: int;
        proc f(out x: int,     y: int) const ref { return g; }
        proc f(    x: int, out y: int)       ref { return g; }
        proc test() {
          var x, y;
          f(x, y);
        }
      }
    )"""",
    {});
}
*/

static void test37() {
  testSplitInit("test37",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(cond: bool) {
          var x;
          if cond then
            return x;
          x = 11;
        }
      }
    )"""",
    {});
}

static void test38() {
  testSplitInit("test38",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          if x {
            x = 11;
          } else {
            x = 11;
          }
        }
      }
    )"""",
    {});
}

static void test39() {
  testSplitInit("test39",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          var y = x;
          x = 11;
        }
      }
    )"""",
    {});
}

static void test40() {
  testSplitInit("test40",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(cond: bool) {
          var x:int;
          if cond {
            x;
          }
          x = 11;
        }
      }
    )"""",
    {});
}

static void test41() {
  testSplitInit("test41",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          {
            x;
          }
          x = 11;
        }
      }
    )"""",
    {});
}

static void test42() {
  testSplitInit("test42",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test() {
          var x:int;
          try {
            x;
          }
          x = 11;
        }
      }
    )"""",
    {});
}

static void test43() {
  testSplitInit("test43",
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
            x;
          }
          x = 11;
        }
      }
    )"""",
    {});
}

static void test44() {
  testSplitInit("test44",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(cond: bool) {
          var x:int;
          if cond {
            x = 12;
          } else {
            if cond {
              return;
            } else {
              return;
            }
          }
        }
      }
    )"""",
    {"x"});
}

static void test45() {
  testSplitInit("test45",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(cond: bool) {
          var x:int;
          if cond {
            x = 12;
          } else {
            while true {
              return;
            }
          }
        }
      }
    )"""",
    {});
}

static void test46() {
  testSplitInit("test46",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(cond: bool) {
          var x:int;
          if cond {
            x = 12;
          } else {
            try {
              return;
            } catch {
              return;
            }
          }
        }
      }
    )"""",
    {"x"});
}

// not split inited because otherwise 'x' would be uninitialized
// when the conditional returns
// (alternative design would be to default-init it for the return,
//  but that is not what we have)
static void test47() {
  testSplitInit("test47",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        config var cond = true;

        proc test(out x: int) {
          if cond {
            return;
          } else {
            x = 5;
          }
        }
      }
    )"""",
    {});
}

// similar to the above but with try/catch
static void test48() {
  testSplitInit("test48",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(out x: int) {
          try {
          } catch {
            return;
          }
        }
      }
    )"""",
    {});
}

static void test49() {
  testSplitInit("test49",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(out x: int) {
          return;
          x = 23;
        }
      }
    )"""",
    {});
}

// the next three are similar to the previous three, but with
// throwing instead of returning.
//
// initializing an 'out' formal in a conditional is OK if the
// other branch returns or throws
static void test50() {
  testSplitInit("test50",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        config var cond = false;

        proc test(out x: int) {
          if cond {
            throw nil;
          } else {
            x = 5;
          }
        }
      }
    )"""",
    {"x"});
}
static void test51() {
  testSplitInit("test51",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(out x: int) {
          try {
            x = 5;
          } catch {
            throw nil;
          }
        }
      }
    )"""",
    {"x"});
}

static void test52() {
  testSplitInit("test52",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        proc test(out x: int) {
          throw nil;
          x = 23;
        }
      }
    )"""",
    {});
}

static void test53() {
  testSplitInit("test53",
    R""""(
      module M {
        // this would be in the standard library...
        operator =(ref lhs: int, rhs: int) {
          __primitive("=", lhs, rhs);
        }

        config var cond = false;

        proc test() {
          var x: int;
          if cond {
            throw nil;
            x = 5;
          } else {
          }
          x;
        }
      }
    )"""",
    {});
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
  //test22(); out intent varargs
  //test23(); out intent varargs
  //test24(); out intent varargs
  //test25(); out intent varargs
  test26a();
  test26b();
  test26c();
  test27();
  test28();
  test29();
  test30();
  //test31(); TODO
  //test32(); TODO
  //test33(); TODO
  //test34(); TODO
  //test35(); TODO
  //test36(); TODO
  test37();
  test38();
  test39();
  test40();
  test41();
  test42();
  test43();
  test44();
  test45();
  test46();
  test47();
  test48();
  test49();
  test50();
  test51();
  test52();
  test53();

  return 0;
}

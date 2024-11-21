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
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/resolution/split-init.h"
#include "chpl/uast/Call.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

#include "./ErrorGuard.h"

using ActionElt = std::tuple<AssociatedAction::Action,
                             std::string, /* ID where action occurs */
                             std::string /* ID acted upon or "" */ >;
using Actions = std::vector<ActionElt>;

static std::string idToStr(Context* context, ID id) {
  std::string name = id.str();
  if (const AstNode* ast = idToAst(context, id)) {
    if (auto nd = ast->toNamedDecl()) {
      name = nd->name().str();
    }
  }

  return name;
}

static void gatherActions(Context* context,
                          const AstNode* ast,
                          const ResolvedFunction* r,
                          Actions& actions) {

  // gather actions for child nodes
  for (auto child : ast->children()) {
    gatherActions(context, child, r, actions);
  }

  // gather actions for this node
  const ResolvedExpression* re = r->resolutionById().byAstOrNull(ast);
  if (re != nullptr) {
    for (auto act: re->associatedActions()) {
      if (act.action() == AssociatedAction::DEINIT) {
        actions.push_back(std::make_tuple(act.action(),
                                          idToStr(context, ast->id()),
                                          idToStr(context, act.id())));
      } else {
        // ignore acted-upon ID expect for DEINIT
        actions.push_back(std::make_tuple(act.action(),
                                          idToStr(context, ast->id()),
                                          ""));
      }
    }
  }
}


static void printAction(const ActionElt& a) {
  AssociatedAction::Action gotAction;
  std::string gotInId;
  std::string gotActId;

  gotAction = std::get<0>(a);
  gotInId = std::get<1>(a);
  gotActId = std::get<2>(a);

  printf("  %s in %s",
         AssociatedAction::kindToString(gotAction),
         gotInId.c_str());

  if (!gotActId.empty()) {
    printf(" for id %s", gotActId.c_str());
  }
  printf("\n");
}

static void printActions(const Actions& actions) {
  for (auto act : actions) {
    printAction(act);
  }
}

// resolves the last function
// checks that the actions match the passed ones
static void testActions(const char* test,
                        const char* program,
                        Actions expected,
                        bool expectErrors=false) {
  printf("### %s\n\n", test);

  Context* context = buildStdContext();
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

  printf("uAST:\n");
  func->dump();

  // resolve runM1
  const ResolvedFunction* r = resolveConcreteFunction(context, func->id());
  assert(r);

  size_t errCount = guard.realizeErrors();
  if (expectErrors) {
    assert(errCount > 0);
  } else {
    assert(errCount == 0);
  }

  Actions actions;
  gatherActions(context, func, r, actions);

  printf("Expecting:\n");
  printActions(expected);
  printf("Got:\n");
  printActions(actions);
  printf("\n");

  size_t i = 0;
  size_t j = 0;
  while (i < actions.size() && j < expected.size()) {
    AssociatedAction::Action gotAction, expectAction;
    std::string gotInId, expectInId;
    std::string gotActId, expectActId;

    gotAction = std::get<0>(actions[i]);
    gotInId = std::get<1>(actions[i]);
    gotActId = std::get<2>(actions[i]);

    expectAction = std::get<0>(expected[i]);
    expectInId = std::get<1>(expected[i]);
    expectActId = std::get<2>(expected[i]);

    if (gotAction != expectAction) {
      assert(false && "Failure: mismatched action type");
    }

    if (gotInId != expectInId) {
      assert(false && "Failure: mismatched containing ID");
    }
    if (gotActId != expectActId) {
      assert(false && "Failure: mismatched acted upon ID");
    }

    i++;
    j++;
  }

  if (i < actions.size()) {
    assert(false && "Failure: extra action");
  }

  if (j < expected.size()) {
    assert(false && "Failure: expected action is missing");
  }


}

// test very basic default init & deinit
static void test1() {
  testActions("test1",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.deinit() { }
        proc test() {
          var x:R;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",        ""},
      {AssociatedAction::DEINIT,       "M.test@2", "x"}
    });
}

// test deinit order when split initing & move from value call
static void test2a() {
  testActions("test2a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R;
          var y:R;
          y = makeR();
          x = makeR();
        }
      }
    )"""",
    {
      {AssociatedAction::DEINIT, "M.test@12", "x"},
      {AssociatedAction::DEINIT, "M.test@12", "y"}
    });
}

// test deinit order when split initing
static void test2b() {
  testActions("test2b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R;
          var y:R;
          x = makeR();
          y = makeR();
        }
      }
    )"""",
    {
      {AssociatedAction::DEINIT, "M.test@12", "y"},
      {AssociatedAction::DEINIT, "M.test@12", "x"}
    });
}

// test deinit order when split initing
static void test2c() {
  testActions("test2c",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R;
          var y:R;
          {
            x = makeR();
            y = makeR();
          }
        }
      }
    )"""",
    {
      {AssociatedAction::DEINIT, "M.test@13", "y"},
      {AssociatedAction::DEINIT, "M.test@13", "x"}
    });
}

// test assignment between values
// this one has no split init and no copy elision
static void test3a() {
  testActions("test3a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R;
          var y:R;
          x; // no split init
          x = y; // assignment -- not a copy so no elision
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",        ""},
      {AssociatedAction::DEFAULT_INIT, "y",        ""},
      // assignment x=y is resolved but not as an associated action
      {AssociatedAction::DEINIT,       "M.test@8", "y"},
      {AssociatedAction::DEINIT,       "M.test@8", "x"}
    });
}

static void test3b() {
  testActions("test3b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R;
          var y:R;
          x = y; // split init
          y; // no copy elision
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "y",        ""},
      {AssociatedAction::COPY_INIT,    "M.test@6", ""},
      {AssociatedAction::DEINIT,       "M.test@8", "x"},
      {AssociatedAction::DEINIT,       "M.test@8", "y"}
    });
}

static void test3c() {
  testActions("test3c",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R;
          var y:R;
          x = y; // split init + copy elision
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "y",        ""},
      {AssociatedAction::DEINIT,       "M.test@7", "x"}
    });
}

static void test3d() {
  testActions("test3d",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R;
          var y:R;
          {
            x = y; // split init + copy elision
          }
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "y",        ""},
      {AssociatedAction::DEINIT,       "M.test@8", "x"}
    });
}

// test copy-initialization from variable decl with init
// variable initialization from a value call
static void test4a() {
  testActions("test4a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R = makeR();
        }
      }
    )"""",
    {
      {AssociatedAction::DEINIT,       "M.test@4", "x"}
    });
}

// variable initialization from a local var mentioned again
static void test4b() {
  testActions("test4b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R = makeR();
          var y:R = x;
          x; // prevent copy elision
        }
      }
    )"""",
    {
      {AssociatedAction::COPY_INIT,    "y",        ""},
      {AssociatedAction::DEINIT,       "M.test@8", "y"},
      {AssociatedAction::DEINIT,       "M.test@8", "x"}
    });
}

// variable initialization from a local var last mention
static void test4c() {
  testActions("test4c",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R = makeR();
          var y:R = x; // copy is elided
        }
      }
    )"""",
    {
      {AssociatedAction::DEINIT,       "M.test@7", "y"}
    });
}

// test cross-type variable init from an integer
static void test5a() {
  testActions("test5a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: int) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R = 4;
        }
      }
    )"""",
    {
      {AssociatedAction::INIT_OTHER,   "x",        ""},
      {AssociatedAction::DEINIT,       "M.test@3", "x"}
    });
}

static void test5b() {
  testActions("test5b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: int) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var x:R;
          x = 4; // split init
        }
      }
    )"""",
    {
      {AssociatedAction::INIT_OTHER,   "M.test@4", ""},
      {AssociatedAction::DEINIT,       "M.test@5", "x"},
    });
}

static void test5c() {
  testActions("test5c",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: int) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }
        proc test() {
          var i = 4;
          var x:R = i;
        }
      }
    )"""",
    {
      {AssociatedAction::INIT_OTHER,   "x",        ""},
      {AssociatedAction::DEINIT,       "M.test@5", "x"},
    });
}

static void test5d() {
  testActions("test5d",
    R""""(
      module M {
        record R { type T; var field : T; }
        proc R.init(type T, field = 0) {
          this.T = T;
          this.field = field;
        }
        proc R.init=(other: ?) {
          this.T = other.type;
          this.field = other;
        }
        proc R.deinit() { }
        proc test() {
          var i = 4;
          var x:R(?) = i;
          var y:R(?) = 42.0;
        }
      }
    )"""",
    {
      {AssociatedAction::INIT_OTHER,   "x",        ""},
      {AssociatedAction::INIT_OTHER,   "y",        ""},
      {AssociatedAction::DEINIT,       "M.test@12", "y"},
      {AssociatedAction::DEINIT,       "M.test@12", "x"},
    });
}


// test cross-type variable init from another record
static void test6a() {
  testActions("test6a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        proc R.init=(other: U) { }
        proc R.deinit() { }
        record U { }
        proc U.init() { }
        proc U.init=(other: U) { }
        proc U.deinit() { }

        proc makeU() {
          return new U();
        }
        proc test() {
          var x:R = makeU();
        }
      }
    )"""",
    {
      {AssociatedAction::INIT_OTHER,   "x",        ""},
      {AssociatedAction::DEINIT,       "M.test@4", "M.test@2"},
      {AssociatedAction::DEINIT,       "M.test@4", "x"}
    });
}

static void test6b() {
  testActions("test6b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        proc R.init=(other: U) { }
        proc R.deinit() { }
        record U { }
        proc U.init() { }
        proc U.init=(other: U) { }
        proc U.deinit() { }

        proc makeU() {
          return new U();
        }
        proc test() {
          var x:R;
          var y:U;
          x = y;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "y",        ""},
      {AssociatedAction::INIT_OTHER,   "M.test@6", ""},
      {AssociatedAction::DEINIT,       "M.test@7", "x"},
      {AssociatedAction::DEINIT,       "M.test@7", "y"}
    });
}

static void test6c() {
  testActions("test6c",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        proc R.init=(other: U) { }
        proc R.deinit() { }
        record U { }
        proc U.init() { }
        proc U.init=(other: U) { }
        proc U.deinit() { }

        proc makeU() {
          return new U();
        }
        proc test() {
          var x:R;
          x = makeU();
        }
      }
    )"""",
    {
      {AssociatedAction::INIT_OTHER,   "M.test@5", ""},
      {AssociatedAction::DEINIT,       "M.test@5", "M.test@4"},
      {AssociatedAction::DEINIT,       "M.test@6", "x"}
    });
}



// testing cross-type init= with 'in' intent
static void test7a() {
  testActions("test7a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        proc R.init=(in other: U) { }
        proc R.deinit() { }
        record U { }
        proc U.init() { }
        proc U.init=(other: U) { }
        proc U.deinit() { }

        proc makeU() {
          return new U();
        }
        proc test() {
          var x:R = makeU();
        }
      }
    )"""",
    {
      {AssociatedAction::INIT_OTHER,   "x",        ""},
      {AssociatedAction::DEINIT,       "M.test@4", "x"}
    });
}

static void test7b() {
  testActions("test7b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        proc R.init=(in other: U) { }
        proc R.deinit() { }
        record U { }
        proc U.init() { }
        proc U.init=(other: U) { }
        proc U.deinit() { }

        proc makeU() {
          return new U();
        }
        proc test() {
          var y:U;
          var x:R = y; // the copy to 'init=(in)' is elided
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "y",        ""},
      {AssociatedAction::INIT_OTHER,   "x",        ""},
      {AssociatedAction::DEINIT,       "M.test@5", "x"},
    });
}

static void test7c() {
  testActions("test7c",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        proc R.init=(in other: U) { }
        proc R.deinit() { }
        record U { }
        proc U.init() { }
        proc U.init=(other: U) { }
        proc U.deinit() { }

        proc makeU() {
          return new U();
        }
        proc test() {
          var y:U;
          var x:R = y;
          y;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "y",        ""},
      {AssociatedAction::COPY_INIT,    "x",        ""},
      {AssociatedAction::INIT_OTHER,   "x",        ""},
      {AssociatedAction::DEINIT,       "M.test@6", "M.test@3"},
      {AssociatedAction::DEINIT,       "M.test@6", "x"},
      {AssociatedAction::DEINIT,       "M.test@6", "y"}
    });
}

// variable initialization from a module-scope variable
static void test8a() {
  testActions("test8a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        var x:R;
        proc test() {
          var y:R = x;
        }
      }
    )"""",
    {
      {AssociatedAction::COPY_INIT,    "y",        ""},
      {AssociatedAction::DEINIT,       "M.test@3", "y"}
    });
}
// variable initialization from a ref variable
static void test8b() {
  testActions("test8b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test() {
          var x:R;
          ref r = x;
          var y:R = r;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",        ""},
      // no associated action recorded to initialize r
      {AssociatedAction::COPY_INIT,    "y",        ""},
      {AssociatedAction::DEINIT,       "M.test@7", "y"},
      {AssociatedAction::DEINIT,       "M.test@7", "x"}
    });
}

// value return from a value call (no return type declared)
static void test9a() {
  testActions("test9a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc makeR() {
          return new R();
        }

        proc test() {
          return makeR();
        }
      }
    )"""",
    { });
}

// value return from a value call (return type declared)
static void test9b() {
  testActions("test9b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc makeR() {
          return new R();
        }

        proc test() : R {
          return makeR();
        }
      }
    )"""",
    { });
}

// value return from a 'new' record construction call
static void test9c() {
  testActions("test9c",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test() : R {
          return new R();
        }
      }
    )"""",
    {
      {AssociatedAction::NEW_INIT, "M.test@3", ""}
    });
}

// value return from a local variable's last mention (no return type)
static void test10a() {
  testActions("test10a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test() {
          const x:R;
          return x;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",        ""},
      {AssociatedAction::DEINIT,       "M.test@4", "x"} // but dead code
    });
}

// value return from a local variable's last mention (declared return type)
static void test10b() {
  testActions("test10b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test() : R {
          const x:R;
          return x;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",        ""},
      // end of block actions -- note that these are dead code
      {AssociatedAction::DEINIT,       "M.test@5", "x"}
    });
}

// similar, but checks that other locals are deinited at return
static void test10c() {
  testActions("test10c",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test() : R {
          var x:R;
          var y:R;
          return x;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",        ""},
      {AssociatedAction::DEFAULT_INIT, "y",        ""},
      {AssociatedAction::DEINIT,       "M.test@6", "y"}, // at return
      // end of block actions -- note that these are dead code
      {AssociatedAction::DEINIT,       "M.test@7", "y"},
      {AssociatedAction::DEINIT,       "M.test@7", "x"}
    });
}

// similar, but returns in a conditional
static void test10d() {
  testActions("test10d",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        config const cond = true;

        proc test() : R {
          var x:R;
          var y:R;
          if cond {
            return x;
          }
          return new R();
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",         ""},
      {AssociatedAction::DEFAULT_INIT, "y",         ""},
      // for 'return x'
      {AssociatedAction::DEINIT,       "M.test@7",  "y"},
      // after that
      {AssociatedAction::NEW_INIT,     "M.test@12", ""},
      // for 'return new R()'
      {AssociatedAction::DEINIT,       "M.test@13", "y"},
      {AssociatedAction::DEINIT,       "M.test@13", "x"},
      // end of block actions -- note that these are dead code
      {AssociatedAction::DEINIT,       "M.test@14", "y"},
      {AssociatedAction::DEINIT,       "M.test@14", "x"}
    });
}

// value return from a local variable that isn't last mention
static void test10e() {
  testActions("test10e",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test() {
          const x:R;
          return x;
          x; // this is dead code
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",        ""},
      {AssociatedAction::DEINIT,       "M.test@5", "x"} // but dead code
    });
}


// return a module-scope variable, inferred return type
static void test11a() {
  testActions("test11a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        var modScope: R;

        proc test() {
          return modScope;
        }
      }
    )"""",
    {
      {AssociatedAction::COPY_INIT,    "M.test@1",  ""}
    });
}

// return a module-scope variable, declared return type
static void test11b() {
  testActions("test11b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        var modScope: R;

        proc test() : R {
          return modScope;
        }
      }
    )"""",
    {
      {AssociatedAction::COPY_INIT,    "M.test@2",  ""}
    });
}

// return a reference variable by value
static void test12a() {
  testActions("test12a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test() : R {
          var value : R;
          ref myref = value;
          return myref;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "value",     ""},
      {AssociatedAction::COPY_INIT,    "M.test@6",  ""},
      {AssociatedAction::DEINIT,       "M.test@6",  "value"},
      {AssociatedAction::DEINIT,       "M.test@7",  "value"}
    });
}
static void test12b() {
  testActions("test12b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test() : R {
          var value : R;
          const ref myref = value;
          return myref;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "value",     ""},
      {AssociatedAction::COPY_INIT,    "M.test@6",  ""},
      {AssociatedAction::DEINIT,       "M.test@6",  "value"},
      {AssociatedAction::DEINIT,       "M.test@7",  "value"}
    });
}
// and with inferred return type
static void test12c() {
  testActions("test12c",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test() {
          var value : R;
          ref myref = value;
          return myref;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "value",     ""},
      {AssociatedAction::COPY_INIT,    "M.test@5",  ""},
      {AssociatedAction::DEINIT,       "M.test@5",  "value"},
      {AssociatedAction::DEINIT,       "M.test@6",  "value"}
    });
}


// test with a return before a split-inited var is initialized
static void test13a() {
  testActions("test13a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        config const cond = true;

        proc test() {
          var x : R;
          if cond {
            return;
          }
          x = new R();
        }
      }
    )"""",
    {
      {AssociatedAction::NEW_INIT,     "M.test@9",   ""},
      {AssociatedAction::DEINIT,       "M.test@11",  "x"}
    });
}

// test a few patterns with 'yield'
static void test14a() {
  testActions("test14a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        config const cond = true;

        iter test() {
          var x : R;
          yield x;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",          ""}
      // x not deinited because it was copy elided to return it
    });
}
static void test14b() {
  testActions("test14b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        config const cond = true;

        iter test() {
          var x : R;
          yield x;
          x;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",          ""},
      {AssociatedAction::COPY_INIT,    "M.test@3",   ""},
      {AssociatedAction::DEINIT,       "M.test@5",   "x"},
    });
}
static void test14c() {
  testActions("test14c",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        config const cond = true;

        iter test() {
          var x : R;
          ref myref = x;
          yield myref;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",          ""},
      {AssociatedAction::COPY_INIT,    "M.test@5",   ""},
      {AssociatedAction::DEINIT,       "M.test@6",   "x"},
    });
}

// returning by 'ref'
static void test15a() {
  testActions("test15a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        var moduleVar : R;

        proc test() ref {
          return moduleVar;
        }
      }
    )"""",
    {
    });
}

// yielding by 'ref'
static void test15b() {
  testActions("test15b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        var moduleVar : R;

        iter test() ref {
          yield moduleVar;
        }
      }
    )"""",
    {
    });
}

// 'in' intent : formal handling
static void test16a() {
  testActions("test16a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test(in arg: R) {
        }
      }
    )"""",
    {
      {AssociatedAction::DEINIT,       "M.test@2",   "arg"},
    });
}
static void test16b() {
  testActions("test16b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test(const in arg: R) {
        }
      }
    )"""",
    {
      {AssociatedAction::DEINIT,       "M.test@2",   "arg"},
    });
}
// calling a function using 'in' intent with copy elision
static void test16c() {
  testActions("test16c",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc acceptsIn(in arg: R) { }

        proc test() {
          var x: R;
          acceptsIn(x);
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",          ""},
      // everything else is copy elided
    });
}
// calling a function using 'in' intent without copy elision
static void test16d() {
  testActions("test16d",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc acceptsIn(in arg: R) { }

        proc test() {
          var x: R;
          acceptsIn(x);
          x;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",          ""},
      {AssociatedAction::COPY_INIT,    "M.test@3",   ""},
      {AssociatedAction::DEINIT,       "M.test@6",   "x"},
    });
}
// calling a function using 'in' intent with nested call
static void test16e() {
  testActions("test16e",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc makeR(): R { var x: R; return x; }
        proc acceptsIn(in arg: R) { }

        proc test() {
          acceptsIn(makeR());
        }
      }
    )"""",
    {
    });
}
// calling a function using 'in' intent with nested call returning ref
static void test16f() {
  testActions("test16f",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        var modVar : R;
        proc getRef() ref : R { return modVar; }
        proc acceptsIn(in arg: R) { }

        proc test() {
          acceptsIn(getRef());
        }
      }
    )"""",
    {
      {AssociatedAction::COPY_INIT,    "M.test@2",   ""},
    });
}
// 'in' intent argument passed to 'in' intent function
static void test16g() {
  testActions("test16g",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc acceptsIn(in arg: R) { }

        proc test(in x: R) {
          acceptsIn(x);
        }
      }
    )"""",
    {
    });
}
// 'in' intent argument returned
static void test16h() {
  testActions("test16h",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test(in x: R) {
          return x;
        }
      }
    )"""",
    {
      // note: this is dead code
      {AssociatedAction::DEINIT,       "M.test@4",   "x"},
    });
}

// 'out' intent: formal not deinitialized (deinited at call site)
static void test17a() {
  testActions("test17a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test(out x: R) {
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",          ""},
    });
}
static void test17b() {
  testActions("test17b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }
        proc makeR() {
          return new R();
        }

        proc test(out x: R) {
          x = makeR();
        }
      }
    )"""",
    {
    });
}

static void test18a() {
  testActions("test18a",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test(out x: R) {
          return x;
        }
      }
    )"""",
    {
      {AssociatedAction::DEFAULT_INIT, "x",          ""},
    });
}

static void test18b() {
  testActions("test18b",
    R""""(
      module M {
        record R { }
        proc R.init() { }
        proc R.init=(other: R) { }
        operator R.=(ref lhs: R, rhs: R) { }
        proc R.deinit() { }

        proc test(inout x: R) {
          return x;
        }
      }
    )"""",
    {
    });
}

static void test19() {
  testActions("test19",
      R"""(
      record G {
        type T;
        var y : T;
      }

      record R {
        type T;
        var x : G(T);
      }

      proc R.init(type T) {
        this.T=T;
      }

      proc test() {
        var r : R(int);
      }
      )""", {
        {AssociatedAction::DEFAULT_INIT, "r",          ""},
        {AssociatedAction::DEINIT,       "test19.test@4",   "r"}
      } );
}

// Returning a non-nilable 'new' local variable
static void test20a() {
  testActions("test20a",
    R""""(
      module M {
        class C {}
        proc C.init() {}
        operator C.=(ref lhs: C, rhs: C) { }
        proc C.deinit() { }

        proc foo() {
          var x : owned C = new C();
          return x;
        }
      }
    )"""",
    {
      {AssociatedAction::NEW_INIT, "M.foo@5",          ""},
      {AssociatedAction::DEINIT,   "M.foo@9",          "x"},
    });
}

// Generic instantiation version
// TODO: Uncomment once we support init of generic types. May need to update
// expected associated actions as well.
/* static void test20b() { */
/*   testActions("test20b", */
/*     R""""( */
/*       module M { */
/*         class C { */
/*           var x; */
/*         } */
/*         proc C.init(x) {this.x = x;} */
/*         operator C.=(ref lhs: C(?), rhs: C(?)) { } */
/*         proc C.deinit() { } */

/*         proc foo() { */
/*           var x : owned C = new C(3); */
/*           return x; */
/*         } */
/*       } */
/*     )"""", */
/*     { */
/*       {AssociatedAction::NEW_INIT,   "M.foo@6",    ""}, */
/*       {AssociatedAction::INIT_OTHER, "x",          ""}, */
/*       {AssociatedAction::DEINIT,     "M.foo@10",   "x"}, */
/*     }); */
/* } */

// Nilable version
static void test20c() {
  testActions("test20c",
    R""""(
      module M {
        class C {}
        proc C.init() {}
        operator C.=(ref lhs: C, rhs: C) { }
        proc C.deinit() { }

        proc foo() {
          var x : owned C? = new C();
          return x;
        }
      }
    )"""",
    {
      {AssociatedAction::NEW_INIT,   "M.foo@6",    ""},
      {AssociatedAction::DEINIT,     "M.foo@10",   "x"},
    });
}

static void test21() {
  // Make sure primitive/builtin types don't trigger dead-variable tracking
  testActions("test21",
      R"""(
      module M {
        proc take(arg: int) {
          return 5;
        }
        proc take2(arg: int) {
          return 5;
        }

        proc test() {
          var cond = true;
          var arg : int;

          var ret : int;
          if cond then ret = take(arg);
          else ret = take2(arg);
        }
      }
      )""", {} );
}

static void test22() {
  // Make sure that call-init-deinit doesn't try to process an initialization
  // by mistakenly passing 'R' to 'helper' instead of passing the forwarded
  // value 'R.c'
  testActions("test22",
      R"""(
      module M {
        // call-init-deinit wants this to transmute R.c into the reciever of C.helper

        class C {
          var x : int;

          proc helper() {
            return x;
          }
        }

        record R {
          var c = new unmanaged C(5);

          forwarding c;

          proc wrapper() {
            return this.helper();
          }
        }

        proc test() {
          var r : R;
          var x = r.wrapper();
        }
      }
      )""", {
        {AssociatedAction::DEFAULT_INIT, "r",          ""},
        {AssociatedAction::DEINIT,       "M.test@6",   "r"}
      });
}

static void test23a() {
  // Ensure the copy-elided in formal doesn't error for its final use.
  testActions("test23a",
      R"""(
      module M {
        record Foo {}

        proc doSomething(in x) {
          return doSomethingElse(x);
        }

        proc doSomethingElse(in x) {
          return 1;
        }

        proc test() {
          var f : Foo;
          var x = doSomething(f);
        }
      }
      )""", {
        {AssociatedAction::DEFAULT_INIT, "f",          ""}
      });
}

static void test23b() {
  // Ensure the copy-elided in formal doesn't error for multiple uses in the
  // same call.
  testActions("test23b",
      R"""(
      module M {
        record Foo {}

        proc doSomething(in x) {
          return doSomethingElse(x, x);
        }

        proc doSomethingElse(in x, in y) {
          return 1;
        }

        proc test() {
          var f : Foo;
          var x = doSomething(f);
        }
      }
      )""", {
        {AssociatedAction::DEFAULT_INIT, "f",          ""}
      });
}

static void test23c() {
  // Ensure the copy-elided in formal doesn't error for uses across multiple
  // calls in the same statement.
  testActions("test23c",
      R"""(
      module M {
        // necessary to resolve + operator
        operator +(a: int, b: int) do return __primitive("+", a, b);

        record Foo {}

        proc doSomething(in x) {
          return doSomethingElse(x) + doSomethingElse(x);
        }

        proc doSomethingElse(in x) {
          return 1;
        }

        proc test() {
          var f : Foo;
          var x = doSomething(f);
        }
      }
      )""", {
        {AssociatedAction::DEFAULT_INIT, "f",          ""}
      });
}

static void test23d() {
  // Ensure the copy-elided in formal doesn't error when used in nested function
  // calls.
  testActions("test23d",
      R"""(
      module M {
        record Foo {}

        proc returnConstRef(const ref x) {
          return x;
        }

        proc returnIn(in x) {
          return x;
        }

        proc doSomething(in x) {
          return doSomethingElse(returnConstRef(returnIn(x)));
        }

        proc doSomethingElse(in x) {
          return 1;
        }

        proc test() {
          var f : Foo;
          var x = doSomething(f);
        }
      }
      )""", {
        {AssociatedAction::DEFAULT_INIT, "f",          ""}
      });
}

// calling function with 'out' intent formal

// calling functions with 'inout' intent formal

// defer

// call-expr temporaries not passed by 'in' intent need deinit

int main() {
  test1();

  test2a();
  test2b();
  test2c();

  test3a();
  test3b();
  test3c();
  test3d();

  test4a();
  test4b();
  test4c();

  test5a();
  test5b();
  test5c();
  test5d();

  test6a();
  test6b();
  test6c();

  test7a();
  test7b();
  test7c();

  test8a();
  test8b();

  test9a();
  test9b();
  test9c();

  test10a();
  test10b();
  test10c();
  test10d();
  test10e();

  test11a();
  test11b();

  test12a();
  test12b();
  test12c();

  test13a();

  test14a();
  test14b();
  test14c();

  test15a();
  test15b();

  test16a();
  test16b();
  test16c();
  test16d();
  test16e();
  test16f();
  test16g();
  test16h();

  test17a();
  test17b();

  test18a();
  test18b();

  test19();

  test20a();
  /* test20b(); */
  test20c();

  test21();

  test22();

  test23a();
  test23b();
  test23c();
  test23d();

  return 0;
}

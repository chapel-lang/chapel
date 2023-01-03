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

  printf("  %s %s %s\n",
         AssociatedAction::kindToString(gotAction),
         gotInId.c_str(),
         gotActId.c_str());
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

  printf("uAST:\n");
  func->dump();

  // resolve runM1
  const ResolvedFunction* r = resolveConcreteFunction(context, func->id());
  assert(r);

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

  if (j < actions.size()) {
    assert(false && "Failure: expected action is missing");
  }

  size_t errCount = guard.realizeErrors();
  if (expectErrors) {
    assert(errCount > 0);
  } else {
    assert(errCount == 0);
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
      {AssociatedAction::ASSIGN,       "M.test@7", ""},
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


int main() {
  test1();
  test2a();
  test2b();
  test2c();
  test3a();
  test3b();
  test3c();
  test3d();

  return 0;
}

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

static const bool ERRORS_EXPECTED=true;

using ActionElt = std::tuple<AssociatedAction::Action,
                             std::string, /* ID where action occurs */
                             std::string /* ID acted upon or "" */ >;
using Actions = std::vector<ActionElt>;

static void gatherActions(const AstNode* ast,
                          const ResolvedFunction* r,
                          Actions& actions) {

  // gather actions for child nodes
  for (auto child : ast->children()) {
    gatherActions(child, r, actions);
  }

  // gather actions for this node
  const ResolvedExpression* re = r->resolutionById().byAstOrNull(ast);
  if (re != nullptr) {
    for (auto act: re->associatedActions()) {
      // ignore acted-upon ID expect for DEINIT
      if (act.action() != AssociatedAction::DEINIT) {
        actions.push_back(std::make_tuple(act.action(), ast->id().str(), ""));
      } else {
        actions.push_back(
            std::make_tuple(act.action(), ast->id().str(), act.id().str()));
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
  gatherActions(func, r, actions);

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
      {AssociatedAction::DEFAULT_INIT, "M.test@1", ""},
      {AssociatedAction::DEINIT, "M.test@2", "M.test@1"}
    });
}

static void test2() {
  testActions("test2",
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
      {AssociatedAction::DEINIT, "M.test@12", "M.test@1"},
      {AssociatedAction::DEINIT, "M.test@12", "M.test@3"}
    });
}

static void test3() {
  testActions("test3",
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
      {AssociatedAction::DEINIT, "M.test@12", "M.test@3"},
      {AssociatedAction::DEINIT, "M.test@12", "M.test@1"}
    });
}

static void test4() {
  testActions("test4",
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
      {AssociatedAction::DEINIT, "M.test@13", "M.test@3"},
      {AssociatedAction::DEINIT, "M.test@13", "M.test@1"}
    });
}





int main() {
  test1();
  test2();
  test3();
  test4();

  return 0;
}

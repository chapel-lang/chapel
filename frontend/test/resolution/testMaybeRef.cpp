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

static void
testMaybeRef(const char* test,
             const char* program,
             // vector of ID of Formal, isRef (vs const ref)
             std::vector<std::pair<const char*, bool>> expectedRefs,
             // vector of ID of call, ID of called function  
             std::vector<std::pair<const char*, const char*>> expectedCalls,
             bool expectErrors=false) {
  printf("\n### %s\n", test);

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

  M->dump();

  // resolving functions for all of the IDs mentioned in expected vectors
  // (this assumes that they are concrete functions)
  for (auto pair : expectedRefs) {
    ID id = ID::fromString(context, pair.first);
    ID parentId = id.parentSymbolId(context);
    const ResolvedFunction* r = resolveConcreteFunction(context, parentId);
    auto sig = inferRefMaybeConstFormals(context,
                                         r->signature(),
                                         /* poiScope */ nullptr);
    auto ast = idToAst(context, id);
    assert(ast && ast->isFormal());
    auto untyped = sig->untyped();
    for (int i = 0; i < untyped->numFormals(); i++) {
      if (untyped->formalDecl(i) == ast) {
        QualifiedType t = sig->formalType(i);
        // it's formal i
        if (pair.first) { // ref
          assert(t.kind() == QualifiedType::REF);
        } else {
          assert(t.kind() == QualifiedType::CONST_REF);
        }
      }
    }
  }

  for (auto pair : expectedCalls) {
    ID id = ID::fromString(context, pair.first);
    ID expectedCalledFnId = ID::fromString(context, pair.second);
    ID parentId = id.parentSymbolId(context);
    const ResolvedFunction* r = resolveConcreteFunction(context, parentId);
    // what is the called function
    auto ast = idToAst(context, id);
    assert(ast && ast->isCall());
    const ResolvedExpression& re = r->byAst(ast);
    // what function is called?
    const MostSpecificCandidates& candidates = re.mostSpecific();
    assert(candidates.numBest() == 1); // should be resolved by now
    ID calledFnId = candidates.only()->untyped()->id();
    assert(expectedCalledFnId == calledFnId);
  }

  size_t errCount = guard.realizeErrors();
  if (expectErrors) {
    assert(errCount > 0);
  } else {
    assert(errCount == 0);
  }
}

static void test1() {
  testMaybeRef("test1",
    R""""(
      module M {
        proc test() {
        }
      }
    )"""",
    {},
    {});
}

static void test2() {
  testMaybeRef("test2",
    R""""(
      module M {
        proc acceptsRef(ref arg) { }
        record R {
          proc method() {
            acceptsRef(this);
          }
        }

      }
    )"""",
    {{"M.R.method@1", true}},
    {});
}

static void test3() {
  testMaybeRef("test3",
    R""""(
      module M {
        var global: int;
        proc foo() ref { return global; }         // M.foo
        proc foo() const ref { return global; }   // M.foo#1
        proc test() {
          var x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo#1"}});
}

static void test4() {
  testMaybeRef("test4",
    R""""(
      module M {
        var global: int;
        proc foo() const ref { return global; }   // M.foo
        proc foo() ref { return global; }         // M.foo#1
        proc test() {
          var x = foo();
        }
      }
    )"""",
    {},
    {{"M.test@1", "M.foo"}});
}


int main() {
  test1();
  test2();
  test3();
  test4();

  return 0;
}

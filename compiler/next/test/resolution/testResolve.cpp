/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/FnCall.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace parsing;
using namespace resolution;
using namespace uast;

// test resolving a very simple module
static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  {
    context->advanceToNextRevision(true);
    auto path = UniqueString::build(context, "input.chpl");
    std::string contents = "var x: int;\n"
                           "x;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    assert(m->numStmts() == 2);
    const Variable* x = m->stmt(0)->toVariable();
    assert(x);
    const Identifier* xIdent = m->stmt(1)->toIdentifier();
    assert(xIdent);

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

    assert(rr.byAst(x).type().type()->isIntType());
    assert(rr.byAst(xIdent).type().type()->isIntType());
    assert(rr.byAst(xIdent).toId() == x->id());

    context->collectGarbage();
  }
}

// test resolving a module in an incremental manner
static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  {
    printf("part 1\n");
    context->advanceToNextRevision(true);
    auto path = UniqueString::build(context, "input.chpl");
    std::string contents = "";
    setFileText(context, path, contents);

    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    resolveModule(context, m->id());

    context->collectGarbage();
  }

  {
    printf("part 2\n");
    context->advanceToNextRevision(true);
    auto path = UniqueString::build(context, "input.chpl");
    std::string contents = "var x;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    resolveModule(context, m->id());

    context->collectGarbage();
  }

  {
    printf("part 3\n");
    context->advanceToNextRevision(true);
    auto path = UniqueString::build(context, "input.chpl");
    std::string contents = "var x: int;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);

    const Variable* x = m->stmt(0)->toVariable();
    assert(x);

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    assert(rr.byAst(x).type().type()->isIntType());

    context->collectGarbage();
  }


  // Run it a few times to make sure there aren't errors related to
  // collectGarbage being run across multiple revisions.
  for (int i = 0; i < 3; i++) {
    printf("part %i\n", 3+i);
    context->advanceToNextRevision(true);
    auto path = UniqueString::build(context, "input.chpl");
    std::string contents = "var x: int;\n"
                           "x;";
    setFileText(context, path, contents);

    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    assert(m->numStmts() == 2);
    const Variable* x = m->stmt(0)->toVariable();
    assert(x);
    const Identifier* xIdent = m->stmt(1)->toIdentifier();
    assert(xIdent);

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

    assert(rr.byAst(x).type().type()->isIntType());
    assert(rr.byAst(xIdent).type().type()->isIntType());
    assert(rr.byAst(xIdent).toId() == x->id());

    context->collectGarbage();
  }
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "input.chpl");

  {
    context->advanceToNextRevision(true);
    std::string contents = "proc foo(arg: int) {\n"
                           "  return arg;\n"
                           "}\n"
                           "var y = foo(1);";
    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    for (const Module* mod : vec) {
      ASTNode::dump(mod);
    }
    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    const Function* procfoo = m->stmt(0)->toFunction();
    assert(procfoo && procfoo->name() == "foo");
    const Variable* y = m->stmt(1)->toVariable();
    assert(y);
    const Expression* rhs = y->initExpression();
    assert(rhs);
    const FnCall* fnc = rhs->toFnCall();
    assert(fnc);
    const Identifier* foo = fnc->calledExpression()->toIdentifier();
    assert(foo && foo->name() == "foo");

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    auto rrfoo = rr.byAst(foo);
    assert(rrfoo.toId() == procfoo->id());

    auto yt = rr.byAst(y).type().type();
    assert(yt->isIntType());

    context->collectGarbage();
  }

  {
    context->advanceToNextRevision(true);
    std::string contents = "var y = foo(1);";
    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    for (const Module* mod : vec) {
      ASTNode::dump(mod);
    }

    assert(vec.size() == 1);
    const Module* m = vec[0]->toModule();
    assert(m);
    const Variable* y = m->stmt(0)->toVariable();
    assert(y);
    const Expression* rhs = y->initExpression();
    assert(rhs);
    const FnCall* fnc = rhs->toFnCall();
    assert(fnc);
    const Identifier* foo = fnc->calledExpression()->toIdentifier();
    assert(foo && foo->name() == "foo");

    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    auto rrfoo = rr.byAst(foo);
    assert(rrfoo.toId().isEmpty());

    auto rry = rr.byAst(y);
    auto yt = rry.type().type();
    assert(yt);
    assert(yt->isErroneousType());

    context->collectGarbage();
  }
}

int main() {
  test1();
  test2();
  test3();

  return 0;
}

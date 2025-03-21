/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/ErrorWriter.h"
#include "chpl/types/all-types.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Module.h"

static const Module* oneModule(const ModuleVec& vec) {
  assert(vec.size() == 1);
  return vec[0];
}

// Reparsing gets updated location information
static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;
  auto path = UniqueString::get(context, "input.chpl");
  UniqueString emptyParentSymbolPath;

  {
    context->advanceToNextRevision(true);
    std::string contents = "var x = 42; /* comment */";
    setFileText(context, path, contents);
    const uast::BuilderResult& p =
      parseFileToBuilderResult(context, path, emptyParentSymbolPath);
    const Module* m = oneModule(parseToplevel(context, path));
    const AstNode *e = m->stmt(0);
    const Comment *c = m->stmt(1)->toComment();
    assert(locateAst(context, e).firstLine() == 1);
    assert(p.commentToLocation(c).firstLine() == 1);
  }

  {
    context->advanceToNextRevision(true);
    std::string contents = "\n\nvar x = 42; /* comment */";
    setFileText(context, path, contents);
    const uast::BuilderResult& p =
      parseFileToBuilderResult(context, path, emptyParentSymbolPath);
    const Module* m = oneModule(parseToplevel(context, path));
    const AstNode *e = m->stmt(0);
    const Comment *c = m->stmt(1)->toComment();
    assert(locateAst(context, e).firstLine() == 3);
    assert(p.commentToLocation(c).firstLine() == 3);
  }
}

// Error messages get updated location information
static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");

  {
    ErrorGuard guard(context);
    context->advanceToNextRevision(true);
    std::string contents = "var x:int = 3.14;";
    setFileText(context, path, contents);
    const ModuleVec& vec = parseToplevel(context, path);
    for (const Module* mod : vec) {
      mod->stringify(std::cout, chpl::StringifyKind::DEBUG_DETAIL);
    }
    const Module* m = oneModule(vec);
    const AstNode *e = m->stmt(0);
    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    (void)rr; // use result

    auto l = locateAst(context, e);
    printf("e loc is line %d\n", l.firstLine());
    assert(l.firstLine() == 1);

    guard.printErrors();
    assert(guard.errors().size() == 1);
    assert(guard.errors()[0]->location(context).firstLine() == 1);
    assert(guard.realizeErrors());
  }

  {
    ErrorGuard guard(context);
    context->advanceToNextRevision(true);
    std::string contents = "\n\nvar x:int = 3.14;";
    setFileText(context, path, contents);
    const ModuleVec& vec = parseToplevel(context, path);
    for (const Module* mod : vec) {
      mod->stringify(std::cout, chpl::StringifyKind::DEBUG_DETAIL);
    }
    const Module* m = oneModule(vec);
    const AstNode *e = m->stmt(0);
    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    (void)rr; // use result

    auto l = locateAst(context, e);
    printf("e loc is line %d\n", l.firstLine());
    fflush(stdout);
    assert(l.firstLine() == 3);

    guard.printErrors();
    assert(guard.errors().size() == 1);
    printf("%d\n", guard.errors()[0]->location(context).firstLine());
    assert(guard.errors()[0]->location(context).firstLine() == 3);
    assert(guard.realizeErrors());
  }
}

int main() {
  test1();
  test2();

  return 0;
}

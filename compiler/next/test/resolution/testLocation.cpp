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

#include "chpl/types/all-types.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Module.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace parsing;
using namespace resolution;
using namespace uast;

static const Module* oneModule(const ModuleVec& vec) {
  assert(vec.size() == 1);
  return vec[0];
}

// TODO maybe we want to support something like this
std::vector<ErrorMessage> errors;
static void collectErrors(const ErrorMessage& err) { errors.push_back(err); }

static void printErrors() {
  for (auto err: errors) {
    printf("%s\n", err.message().c_str());
  }
}

// Reparsing gets updated location information
static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;
  auto path = UniqueString::build(context, "input.chpl");

  {
    context->advanceToNextRevision(true);
    std::string contents = "var x = 42; /* comment */";
    setFileText(context, path, contents);
    const uast::BuilderResult& p = parseFile(context, path);
    const Module* m = oneModule(parse(context, path));
    const Expression *e = m->stmt(0);
    const Comment *c = m->stmt(1)->toComment();
    assert(locateAst(context, e).firstLine() == 1);
    assert(p.commentToLocation(c).firstLine() == 1);
  }

  {
    context->advanceToNextRevision(true);
    std::string contents = "\n\nvar x = 42; /* comment */";
    setFileText(context, path, contents);
    const uast::BuilderResult& p = parseFile(context, path);
    const Module* m = oneModule(parse(context, path));
    const Expression *e = m->stmt(0);
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

  ctx.setErrorHandler(collectErrors);

  auto path = UniqueString::build(context, "input.chpl");

  {
    errors.clear();
    context->advanceToNextRevision(true);
    std::string contents = "var x:int = 3.14;";
    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    for (const Module* mod : vec) {
      mod->stringify(std::cout, chpl::StringifyKind::DEBUG_DETAIL);
    }
    const Module* m = oneModule(vec);
    const Expression *e = m->stmt(0);
    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    (void)rr; // use result

    auto l = locateAst(context, e);
    printf("e loc is line %d\n", l.firstLine());
    assert(l.firstLine() == 1);

    printErrors();
    assert(errors.size() == 1);
    assert(errors[0].location().firstLine() == 1);
  }

  {
    errors.clear();
    context->advanceToNextRevision(true);
    std::string contents = "\n\nvar x:int = 3.14;";
    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    for (const Module* mod : vec) {
      mod->stringify(std::cout, chpl::StringifyKind::DEBUG_DETAIL);
    }
    const Module* m = oneModule(vec);
    const Expression *e = m->stmt(0);
    const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());
    (void)rr; // use result

    auto l = locateAst(context, e);
    printf("e loc is line %d\n", l.firstLine());
    fflush(stdout);
    assert(l.firstLine() == 3);

    printErrors();
    assert(errors.size() == 1);
    printf("%d\n", errors[0].location().firstLine());
    assert(errors[0].location().firstLine() == 3);
  }
}

int main() {
  test1();
  test2();

  return 0;
}

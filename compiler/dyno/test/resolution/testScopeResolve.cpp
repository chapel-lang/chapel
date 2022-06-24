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

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/Block.h"
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

// helper functions
/*
static const Module* findModule(const AstNode* ast, const char* name) {
  if (auto v = ast->toModule()) {
    if (v->name() == name) {
      return v;
    }
  }

  for (auto child : ast->children()) {
    auto got = findModule(child, name);
    if (got) return got;
  }

  return nullptr;
}

static const Module* findModule(const ModuleVec& vec, const char* name) {
  for (auto mod : vec) {
    auto got = findModule(mod, name);
    if (got) return got;
  }

  return nullptr;
}
*/

static const Variable* findVariable(const AstNode* ast, const char* name) {
  if (auto v = ast->toVariable()) {
    if (v->name() == name) {
      return v;
    }
  }

  for (auto child : ast->children()) {
    auto got = findVariable(child, name);
    if (got) return got;
  }

  return nullptr;
}

static const Variable* findVariable(const ModuleVec& vec, const char* name) {
  for (auto mod : vec) {
    auto got = findVariable(mod, name);
    if (got) return got;
  }

  return nullptr;
}

static const ResolvedExpression&
scopeResolveIt(Context* context, const AstNode* ast) {
  ID mId = idToParentModule(context, ast->id());
  assert(!mId.isEmpty());
  // if scopeResolveModule ever goes away, this can safely
  // be replaced by resolveModule.
  const ResolutionResultByPostorderID& rr = scopeResolveModule(context, mId);
  return rr.byAst(ast);
}

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var y: int;
      }

      module N {
        {
          import M;
          var x = M.y;
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  const ResolvedExpression& re = scopeResolveIt(context, x->initExpression());
  assert(re.toId() == y->id());
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var y: int;
      }

      module N {
        {
          import M.y;
          var x = y;
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  const ResolvedExpression& re = scopeResolveIt(context, x->initExpression());
  assert(re.toId() == y->id());
}

static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var y: int;
      }

      module N {
        {
          import M.{y};
          var x = y;
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  const ResolvedExpression& re = scopeResolveIt(context, x->initExpression());
  assert(re.toId() == y->id());
}

static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var y: int;
      }

      module N {
        {
          use M;
          var x = y;
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  const ResolvedExpression& re = scopeResolveIt(context, x->initExpression());
  assert(re.toId() == y->id());
}

static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var y: int;
      }

      module N {
        {
          use M;
          var x = M.y;
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  const ResolvedExpression& re = scopeResolveIt(context, x->initExpression());
  assert(re.toId() == y->id());
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();

  return 0;
}

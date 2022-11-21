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
#include "chpl/uast/Block.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Include.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"
#include "./ErrorGuard.h"

// helper functions

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

  const ModuleVec& vec = parseToplevel(context, path);
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

  const ModuleVec& vec = parseToplevel(context, path);
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

  const ModuleVec& vec = parseToplevel(context, path);
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

  const ModuleVec& vec = parseToplevel(context, path);
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

  const ModuleVec& vec = parseToplevel(context, path);
  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  const ResolvedExpression& re = scopeResolveIt(context, x->initExpression());
  assert(re.toId() == y->id());
}

static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  auto pathMM = UniqueString::get(context, "MM.chpl");
  std::string contentsMM = R""""(
      module MM {
        include module Sub;
        import this.Sub.y;
        var x = y;
      }
   )"""";
  setFileText(context, pathMM, contentsMM);

  auto pathMMSub = UniqueString::get(context, "MM/Sub.chpl");
  std::string contentsMMSub = R""""(
      module Sub {
        var y: int;
      }
   )"""";
  setFileText(context, pathMMSub, contentsMMSub);

  const ModuleVec& vec = parseToplevel(context, pathMM);
  assert(vec.size() == 1);
  const Module* mm = vec[0];
  assert(mm->name() == "MM");
  assert(mm->id().symbolPath() == "MM");
  assert(mm->numStmts() == 3);
  const Include* inc = mm->stmt(0)->toInclude();
  assert(inc);
  assert(inc->visibility() == Decl::DEFAULT_VISIBILITY);
  assert(inc->isPrototype() == false);
  assert(inc->name() == "Sub");

  const Variable* x = findVariable(vec, "x");
  assert(x);

  const ResolvedExpression& re = scopeResolveIt(context, x->initExpression());
  ID xInitId = re.toId();
  assert(!xInitId.isEmpty());

  const Module* sub = getIncludedSubmodule(context, inc->id());
  assert(sub);
  assert(sub->name() == "Sub");
  assert(sub->id().symbolPath() == "MM.Sub");

  const Variable* y = findVariable(sub, "y");
  assert(y);

  assert(xInitId == y->id());
}

static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        module SiblingModule {
          var y: int;
        }
        module SubModule {
          import super.SiblingModule;
          var x = SiblingModule.y;
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);
  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  const ResolvedExpression& re = scopeResolveIt(context, x->initExpression());
  assert(re.toId() == y->id());
}

// test transitive visibility through public/non-public uses
// TODO: Test that private auto-uses (ChapelStandard) are subject to the
// correct transitive visibility rules. Currently this is not very well
// possible because we cannot change which module is auto-used to one that
// would be conducive to testing -- see private issue #3830 which would
// change this.
static void test8() {
  printf("test8\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module A {
        var x : int;
      }
      module B {
        public use A;
      }
      module C {
        use B;
        var y = x;
      }
      module D {
        use C;
        var a = y;
        var b = x;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);
  const Variable* a = findVariable(vec, "a");
  assert(a);
  const Variable* b = findVariable(vec, "b");
  assert(b);

  assert(x->initExpression() == nullptr);
  const ResolvedExpression& reY = scopeResolveIt(context, y->initExpression());
  assert(reY.toId() == x->id());
  const ResolvedExpression& reA = scopeResolveIt(context, a->initExpression());
  assert(reA.toId() == y->id());
  const ResolvedExpression& reB = scopeResolveIt(context, b->initExpression());
  assert(reB.toId().isEmpty());
}

// test use with except-lists
static void test9() {
  printf("test9\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module A {
        var x : int;
        var y : int;
        var z : int = 4;
      }
      module B {
        public use A except y, z;

        var a : int = x;
        var b : int = y;
        var c : int = A.x;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);
  const Variable* z = findVariable(vec, "z");
  assert(z);
  const Variable* a = findVariable(vec, "a");
  assert(a);
  const Variable* b = findVariable(vec, "b");
  assert(b);
  const Variable* c = findVariable(vec, "c");
  assert(c);

  const ResolvedExpression& reA = scopeResolveIt(context, a->initExpression());
  assert(reA.toId() == x->id());
  const ResolvedExpression& reB = scopeResolveIt(context, b->initExpression());
  assert(reB.toId().isEmpty());
  const ResolvedExpression& reC = scopeResolveIt(context, c->initExpression());
  assert(reC.toId().isEmpty());
}

// test name conflicts can occur with uses
static void test10() {
  printf("test10\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module A {
        var x = 2;
      }
      module B {
        var x = 4;
      }
      module Z {
        use A, B;
        var a = x;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* a = findVariable(vec, "a");
  assert(a);

  const ResolvedExpression& reA = scopeResolveIt(context, a->initExpression());
  assert(reA.toId().isEmpty());
}

// Private modifiers are respected.
static void test11() {
  printf("test11\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module A {
        private var x : int;
      }
      module B {
        public use A;
        private var y : int;
      }
      module C {
        use B;
        private var z : int;
        var a = x;
        var b = y;
        var c = z;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);
  const Variable* z = findVariable(vec, "z");
  assert(z);
  const Variable* a = findVariable(vec, "a");
  assert(a);
  const Variable* b = findVariable(vec, "b");
  assert(b);
  const Variable* c = findVariable(vec, "c");
  assert(c);

  const ResolvedExpression& reA = scopeResolveIt(context, a->initExpression());
  assert(reA.toId().isEmpty());
  const ResolvedExpression& reB = scopeResolveIt(context, b->initExpression());
  assert(reB.toId().isEmpty());
  const ResolvedExpression& reC = scopeResolveIt(context, c->initExpression());
  assert(reC.toId() == z->id());
}

// multiple imports / uses of the same module don't block finding symbols
// via the second visibility statment onwards.
static void test12() {
  printf("test12\n");
  Context ctx;
  Context* context = &ctx;

  // x is imported as usual.
  // y is only ever imported renamed, so we can't find it.
  // When we search for z, we first check 'B except y' and don't find it. We
  // then check 'B only y as z'. The name we're looking for changed, so
  // we have to perform the search again (and should not skip searching in
  // B a second time).

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module A {
        var x : int;
        var y : int;
      }
      module B {
        use A except y;
        use A only y as z;

        var a = x;
        var b = y;
        var c = z;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);
  const Variable* a = findVariable(vec, "a");
  assert(a);
  const Variable* b = findVariable(vec, "b");
  assert(b);
  const Variable* c = findVariable(vec, "c");
  assert(c);

  const ResolvedExpression& reA = scopeResolveIt(context, a->initExpression());
  assert(reA.toId() == x->id());
  const ResolvedExpression& reB = scopeResolveIt(context, b->initExpression());
  assert(reB.toId().isEmpty());
  const ResolvedExpression& reC = scopeResolveIt(context, c->initExpression());
  assert(reC.toId() == y->id());
}

static void test13() {
  printf("test13\n");
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = UniqueString::get(ctx, "Foo.chpl");
  std::string contents = R""""(
      module Foo {
        import super.bar;
        import super.super;
        import super.super.bar;
        use super.bar;
        use super.super;
        use super.super.bar;
        var x: int;
      }
   )"""";

  setFileText(ctx, path, contents);

  const ModuleVec& vec = parseToplevel(ctx, path);

  // Variable triggers resolution of use/import statements.
  const Variable* x = findVariable(vec, "x");
  assert(x);

  std::ignore = scopeResolveIt(ctx, x);
  assert(guard.errors().size() == 6);

  for (int i = 0; i < 3; i++) {
    auto& e = guard.errors()[i];
    assert(e->message() == "invalid use of 'super' in 'import'; 'Foo' is a "
                           "top-level module");
  }
  for (int i = 3; i < 6; i++) {
    auto& e = guard.errors()[i];
    assert(e->message() == "invalid use of 'super' in 'use'; 'Foo' is a "
                           "top-level module");
  }

  assert(guard.realizeErrors());
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

  return 0;
}

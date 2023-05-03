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
#include "chpl/uast/Block.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Include.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

// helper functions

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

// There's specal handling for the rightmost field access. Make sure this
// special handling properly handles super.
static void test14() {
  printf("test14\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module A {
        var x : int;
        module B {
          import this.super as C;
          var y = C.x;
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  const ResolvedExpression& reY = scopeResolveIt(context, y->initExpression());
  assert(reY.toId() == x->id());
}


// Make sure that the dot-expression handling of "this" works in addition
// to the identifier-expression handling of "this". Technically this is
// redundant, but our goal is to issue a warning, not fail to resolve in
// this case.
static void test15() {
  printf("test15\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  // Note this.super.this instead of this.super in the previous test.
  std::string contents = R""""(
      module A {
        var x : int;
        module B {
          import this.super.this as C;
          var y = C.x;
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  const ResolvedExpression& reY = scopeResolveIt(context, y->initExpression());
  assert(reY.toId() == x->id());
}

// Ensures that a module declared private and then imported renamed is
// not accessible externally (renaming doesn't change visibility).
static void test16() {
  printf("test16\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module A {
        private module B {
          var x = 1;
        };
        public use B as C;
      }
      module D {
        use A;
        import C;
        var y = C.x;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  const ResolvedExpression& reY = scopeResolveIt(context, y->initExpression());
  assert(reY.toId().isEmpty());
  assert(guard.realizeErrors() >= 1);
}

// Makes sure a user can't use a module as a variable (like var x = M).
static void test17() {
  printf("test17\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module TopLevel {
        module A {}
        var x = A;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  // Trigger scope resolution
  const ResolvedExpression& reX = scopeResolveIt(context, x->initExpression());
  (void) reX;

  assert(guard.errors().size() == 1);
  auto& e = guard.errors()[0];
  assert(e->message() == "modules (like 'A' here) cannot be "
                         "mentioned like variables");
  guard.realizeErrors();
}

// check that 'private use M' puts its contents in a shadow scope
// further from a 'public use' (which isn't in a shadow scope)
static void test18() {
  printf("test18\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var myvar: int;
      }

      module N {
        var myvar: real;
      }

      module O {
        public use M;
        private use N;

        var x = myvar;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);

  const ResolvedExpression& reY = scopeResolveIt(context, x->initExpression());
  assert(reY.toId().str() == "M@1");
}
// check that 'private use M' puts M in a further shadow scope from contents
static void test19() {
  printf("test19\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var M: int;
      }

      module O {
        use M;

        var x = M; // should refer to var M, not module M
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);

  const ResolvedExpression& reY = scopeResolveIt(context, x->initExpression());
  assert(reY.toId().str() == "M@1");
}
// check that 'public use M' does not bring in a symbol named M
static void test20() {
  printf("test20\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var M: int;
      }

      module O {
        public use M;

        var x = M; // should refer to var M, not module M
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);

  const ResolvedExpression& reY = scopeResolveIt(context, x->initExpression());
  assert(reY.toId().str() == "M@1");
}
static void test21() {
  printf("test21\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module Outer {
        module M {
          var myvar: int;
        }
      }

      module O {
        public use Outer.M;
        use M; // should result in an error
        var x = myvar;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);

  for (auto mod : vec) {
    scopeResolveModule(context, mod->id());
  }
  assert(guard.realizeErrors() == 1);
}
// but 'public use M as M' does bring in module named M
static void test22() {
  printf("test22\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var myvar: int;
      }

      module O {
        public use M as M;

        var x = M.myvar;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);

  const ResolvedExpression& reY = scopeResolveIt(context, x->initExpression());
  assert(reY.toId().str() == "M@1");
}

// Testing errors issued: two conflicting things renamed to the same thing.
static void test23() {
  printf("test23\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var a = "a";
        var b = "b";
      }

      module O {
        import M.{a as c, b as c};
        var x = M.c;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);

  const ResolvedExpression& reMc = scopeResolveIt(context, x->initExpression());
  assert(reMc.toId().isEmpty());

  assert(guard.numErrors() >= 1);
  assert(guard.error(0)->type() == ErrorType::UseImportMultiplyDefined);
  guard.realizeErrors();
}

// Testing warning issued: one variable imported as two different things.
static void test24() {
  printf("test24\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var a = "a";
      }

      module O {
        import M.{a as b, a as c};
        var x = b;
        var y = c;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(x);
  const Variable* a = findVariable(vec, "a");
  assert(a);

  const ResolvedExpression& reB = scopeResolveIt(context, x->initExpression());
  assert(reB.toId() == a->id());

  const ResolvedExpression& reC = scopeResolveIt(context, y->initExpression());
  assert(reC.toId() == a->id());

  assert(guard.numErrors() == 1);
  assert(guard.error(0)->type() == ErrorType::UseImportMultiplyMentioned);
  guard.clearErrors();
}

// Testing errors and warnings issued: a variable transitively renamed;
// did the user mean `a as c` instead of `a as b, b as c`?
static void test25() {
  printf("test24\n");
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var a = "a";
      }

      module O {
        import M.{a as b, b as c};
        var x = c;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);

  const ResolvedExpression& reC = scopeResolveIt(context, x->initExpression());
  assert(reC.toId().isEmpty());

  assert(guard.numErrors() >= 2);
  assert(guard.error(0)->type() == ErrorType::UseImportUnknownSym);
  assert(guard.error(1)->type() == ErrorType::UseImportTransitiveRename);
  guard.realizeErrors();
  guard.clearErrors();
}

// check a more nested module example with privacy
static void test26() {
  printf("test26\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        private var x : int;
        module Sub {
          use M;

          var y = x; // access to 'x' should be OK
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  const ResolvedExpression& reY = scopeResolveIt(context, y->initExpression());
  assert(reY.toId() == x->id());
}
// this version matches the privateToParent test
static void test27() {
  printf("test27\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        private var x : int;
        module Sub {
          proc main() {
            use M;
            var y = x; // access to 'x' should be OK
          }
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  ID fnId = y->id().parentSymbolId(context);
  auto fn = idToAst(context, fnId);
  assert(fn && fn->isFunction());

  const ResolvedFunction* rfn = scopeResolveFunction(context, fn->id());
  const ResolvedExpression& reY = rfn->byAst(y->initExpression());
  assert(reY.toId() == x->id());
}
// this version has two paths to 'x' and the public-only is processed first
static void test28() {
  printf("test28\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module Other {
        public use M;
      }
      module M {
        private var x : int;
        module Sub {
          use M;
          proc main() {
            use Other;

            var y = x; // access to 'x' should be OK
          }
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  ID fnId = y->id().parentSymbolId(context);
  auto fn = idToAst(context, fnId);
  assert(fn && fn->isFunction());

  const ResolvedFunction* rfn = scopeResolveFunction(context, fn->id());
  const ResolvedExpression& reY = rfn->byAst(y->initExpression());
  assert(reY.toId() == x->id());
}
// this one tests if a 'private use' is visible within a submodule
static void test29() {
  printf("test29\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module Other {
        var x;
      }
      module M {
        private use Other;
        module Sub {
          proc main() {
            use M;
            var y = x; // access to 'x' should be OK
          }
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  ID fnId = y->id().parentSymbolId(context);
  auto fn = idToAst(context, fnId);
  assert(fn && fn->isFunction());

  const ResolvedFunction* rfn = scopeResolveFunction(context, fn->id());
  const ResolvedExpression& reY = rfn->byAst(y->initExpression());
  assert(reY.toId().isEmpty());
}

// this one is a regression test for filter/exclude flags and their storage.
// Using just a single Flags bitfield in scope lookup is not enough, we
// need a list, and this test exposes why.
//
// See also https://github.com/chapel-lang/chapel/issues/22217 for an explanation
// of what used to go wrong.
static void test30() {
  printf("test30\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module TopLevel {
          module XContainerUser {
              public use TopLevel.XContainer; // Will search for public, to no avail.
          }
          module XContainer {
              private var x: int;
              record R {} // R is in the same scope as x so it won't set public

              module MethodHaver {
                  use TopLevel.XContainerUser;
                  use TopLevel.XContainer;
                  proc R.foo() {
                      var y = x;
                  }
              }
          }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);
  const Variable* y = findVariable(vec, "y");
  assert(y);

  ID fnId = y->id().parentSymbolId(context);
  auto fn = idToAst(context, fnId);
  assert(fn && fn->isFunction());

  const ResolvedFunction* rfn = scopeResolveFunction(context, fn->id());
  const ResolvedExpression& reY = rfn->byAst(y->initExpression());
  assert(reY.toId() == x->id());
}

// It has been observed that the production compiler finds `x` in test30 even
// when the second use statement is commented out. It shouldn't, because
// parent lookup ought to stop at module boundaries.
static void test30a() {
  printf("test30a\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module TopLevel {
          module XContainerUser {
              public use TopLevel.XContainer; // Will search for public, to no avail.
          }
          module XContainer {
              private var x: int;
              record R {} // R is in the same scope as x so it won't set public

              module MethodHaver {
                  use TopLevel.XContainerUser;
                  proc R.foo() {
                      var y = x;
                  }
              }
          }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* y = findVariable(vec, "y");
  assert(y);

  ID fnId = y->id().parentSymbolId(context);
  auto fn = idToAst(context, fnId);
  assert(fn && fn->isFunction());

  const ResolvedFunction* rfn = scopeResolveFunction(context, fn->id());
  const ResolvedExpression& reY = rfn->byAst(y->initExpression());
  assert(reY.toId().isEmpty());
}

// Production compiler excludes methods from search unless their receiver
// somehow matches. In non-method contexts, Dyno mimics this by ignoring
// all methods. So, a method on int should not get in the way of foo.
//
// At the time of writing, this is locked down in production by:
// test/functions/kbrady/proc_scoping.chpl
static void test31() {
  printf("test31\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      proc foo(arg: int) {
        proc int.arg {}
        var x = arg;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  const Variable* x = findVariable(vec, "x");
  assert(x);

  ID fnId = x->id().parentSymbolId(context);
  auto fn = idToAst(context, fnId);
  assert(fn && fn->isFunction());

  const ResolvedFunction* rfn = scopeResolveFunction(context, fn->id());
  const ResolvedExpression& reX = rfn->byAst(x->initExpression());
  assert(reX.toId() == fn->toFunction()->formal(0)->id());
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
  test22();
  test23();
  test24();
  test25();
  test26();
  test27();
  test28();
  test29();
  test30();
  test30a();
  test31();

  return 0;
}

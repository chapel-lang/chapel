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

// testing a simple variable declaration and subsequent identifier
static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test1.chpl");
  std::string contents = "module M {\n"
                         "  var x;\n"
                         "  x;\n"
                         "}\n";
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


  const Scope* scopeForIdent = scopeForId(context, xIdent->id());
  assert(scopeForIdent);

  const auto& match = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(match.id() == x->id());
  assert(match.found() == InnermostMatch::ONE);
}

// testing no matching variable declaration
static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test2.chpl");
  std::string contents = "module M {\n"
                         "  var y;\n"
                         "  x;\n"
                         "}\n";
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


  const Scope* scopeForIdent = scopeForId(context, xIdent->id());
  assert(scopeForIdent);

  const auto& match = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(match.id() == ID());
  assert(match.found() == InnermostMatch::ZERO);
}

// testing duplicate matching variable declarations
static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test3.chpl");
  std::string contents = "module M {\n"
                         "  var x;\n"
                         "  var x;\n"
                         "  x;\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 3);
  const Variable* x1 = m->stmt(0)->toVariable();
  assert(x1);
  const Variable* x2 = m->stmt(1)->toVariable();
  assert(x2);
  const Identifier* xIdent = m->stmt(2)->toIdentifier();
  assert(xIdent);

  const Scope* scopeForIdent = scopeForId(context, xIdent->id());
  assert(scopeForIdent);

  // design of findInnermostDecl is to return the
  // innermost and first if there is any ambiguity.
  const auto& match = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(match.id() == x1->id());
  assert(match.found() == InnermostMatch::MANY);
}

// testing a simple use statement
static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test4.chpl");
  std::string contents = "module M {\n"
                         "  var x;\n"
                         "}\n"
                         "module N {\n"
                         "  use M;\n"
                         "  x;\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 1);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 2);

  const Variable* x = m->stmt(0)->toVariable();
  assert(x);

  const Identifier* xIdent = n->stmt(1)->toIdentifier();
  assert(xIdent);

  const Scope* scopeForIdent = scopeForId(context, xIdent->id());
  assert(scopeForIdent);

  const auto& match = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(match.found() == InnermostMatch::ONE);
  assert(match.id() == x->id());
}

// testing a simple recursive use statement
static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test5.chpl");
  std::string contents = "module M {\n"
                         "  use N;\n"
                         "  var x;\n"
                         "}\n"
                         "module N {\n"
                         "  use M;\n"
                         "  x;\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 2);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 2);

  const Variable* x = m->stmt(1)->toVariable();
  assert(x);

  const Identifier* xIdent = n->stmt(1)->toIdentifier();
  assert(xIdent);

  const Scope* scopeForIdent = scopeForId(context, xIdent->id());
  assert(scopeForIdent);

  const auto& match = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(match.id() == x->id());
  assert(match.found() == InnermostMatch::ONE);
}

// testing symbol from parent scope
static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test6.chpl");
  std::string contents = "module M {\n"
                         "  var x;\n"
                         "  { { { x; } } }\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 2);
  const Variable* x = m->stmt(0)->toVariable();
  assert(x);
  const Block* b1 = m->stmt(1)->toBlock();
  assert(b1);
  const Block* b2 = b1->stmt(0)->toBlock();
  assert(b2);
  const Block* b3 = b2->stmt(0)->toBlock();
  assert(b3);
  const Identifier* xIdent = b3->stmt(0)->toIdentifier();
  assert(xIdent);

  const Scope* scopeForIdent = scopeForId(context, xIdent->id());
  assert(scopeForIdent);

  const auto& match = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(match.id() == x->id());
  assert(match.found() == InnermostMatch::ONE);
}

// test scope resolution handles incremental changes OK
static void test7() {
  printf("test7\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test7.chpl");
  std::string contents = "";

  const Module* m = nullptr;
  const Module* oldM = nullptr;
  const Scope* mScope = nullptr;
  const Scope* oldMScope = nullptr;
  const Variable* x = nullptr;
  const Variable* oldX = nullptr;
  const Identifier* ident = nullptr;
  const Identifier* oldIdent = nullptr;

  {
    contents = "module M { }\n";

    context->advanceToNextRevision(true);
    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    m = vec[0];
    mScope = scopeForId(context, m->id());
    assert(mScope && mScope->id() == m->id());

    oldM = m;
    oldMScope = mScope;
    oldX = x;
    oldIdent = ident;
  }

  {
    contents = "module M {\n"
               "  var x;\n"
               "  y;\n"
               "}\n";

    context->advanceToNextRevision(true);
    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    m = vec[0];
    assert(m->numStmts() == 2);
    x = m->stmt(0)->toVariable();
    ident = m->stmt(1)->toIdentifier();
    assert(x);
    assert(ident);

    // module contents changed so pointer should change
    assert(m != oldM);
    // lookup scope
    mScope = scopeForId(context, m->id());
    assert(mScope);
    assert(mScope->id() == m->id() && mScope->numDeclared() == 1);
    assert(mScope->containsUseImport() == false);
    // scope contents changed so Scope pointer should change
    assert(mScope != oldMScope);

    const auto& match = findInnermostDecl(context, mScope, ident->name());
    assert(match.found() == InnermostMatch::ZERO);
    assert(match.id() == ID());

    oldM = m;
    oldMScope = mScope;
    oldX = x;
    oldIdent = ident;
  }

  {
    contents = "module M {\n"
               "  var x;\n"
               "  x;\n"
               "}\n";

    context->advanceToNextRevision(true);
    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 1);
    m = vec[0];
    assert(m->numStmts() == 2);
    x = m->stmt(0)->toVariable();
    ident = m->stmt(1)->toIdentifier();
    assert(x);
    assert(ident);

    // module contents changed so pointer should change
    assert(m != oldM);
    // x didn't change so pointer should be the same
    assert(x == oldX);
    // ident changed so pointer should differ
    assert(ident != oldIdent);
    // lookup scope
    mScope = scopeForId(context, m->id());
    assert(mScope);
    assert(mScope->id() == m->id() && mScope->numDeclared() == 1);
    assert(mScope->containsUseImport() == false);
    // scope contents did not change so Scope pointer should be the same
    assert(mScope == oldMScope);

    const auto& match = findInnermostDecl(context, mScope, ident->name());
    assert(match.found() == InnermostMatch::ONE);
    assert(match.id() == x->id());

    oldM = m;
    oldMScope = mScope;
    oldX = x;
    oldIdent = ident;
  }

  {
    contents = "module M {\n"
               "  var x;\n"
               "  x;\n"
               "  use N;\n"
               "}\n"
               "module N { }\n";

    context->advanceToNextRevision(true);
    setFileText(context, path, contents);
    const ModuleVec& vec = parse(context, path);
    assert(vec.size() == 2);
    m = vec[0];
    assert(m->numStmts() == 3);
    x = m->stmt(0)->toVariable();
    ident = m->stmt(1)->toIdentifier();
    assert(x);
    assert(ident);

    // module contents changed so pointer should change
    assert(m != oldM);
    // x didn't change so pointer should be the same
    assert(x == oldX);
    // ident didn't change so pointer should be the same
    assert(ident == oldIdent);
    // lookup scope
    mScope = scopeForId(context, m->id());
    assert(mScope);
    assert(mScope->id() == m->id() && mScope->numDeclared() == 1);
    assert(mScope->containsUseImport() == true);
    // scope contents did change so Scope pointer should differ
    assert(mScope != oldMScope);

    const auto& match = findInnermostDecl(context, mScope, ident->name());
    assert(match.found() == InnermostMatch::ONE);
    assert(match.id() == x->id());

    oldM = m;
    oldMScope = mScope;
    oldX = x;
    oldIdent = ident;
  }
}

// test variable shadowing
static void test8() {
  printf("test8\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test8.chpl");
  std::string contents = "module M {\n"
                         "  var x;\n"
                         "  {\n"
                         "    var x;\n"
                         "    x;\n"
                         "  }\n"
                         "  x;\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 3);
  const Variable* outerX = m->stmt(0)->toVariable();
  assert(outerX);
  const Block* block = m->stmt(1)->toBlock();
  assert(block);
  const Identifier* outerIdent = m->stmt(2)->toIdentifier();
  assert(outerIdent);

  assert(block->numStmts() == 2);
  const Variable* innerX = block->stmt(0)->toVariable();
  assert(innerX);
  const Identifier* innerIdent = block->stmt(1)->toIdentifier();
  assert(innerIdent);

  const Scope* outerScope = scopeForId(context, outerIdent->id());
  assert(outerScope);
  const Scope* innerScope = scopeForId(context, innerIdent->id());
  assert(innerIdent);

  auto xStr = UniqueString::get(context, "x");

  {
    const auto& match = findInnermostDecl(context, outerScope, xStr);
    assert(match.id() == outerX->id());
    assert(match.found() == InnermostMatch::ONE);
  }

  {
    const auto& match = findInnermostDecl(context, innerScope, xStr);
    assert(match.id() == innerX->id());
    assert(match.found() == InnermostMatch::ONE);
  }
}

// testing a simple import statement
static void test9() {
  printf("test9\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test9.chpl");
  std::string contents = "module M {\n"
                         "  import N;\n"
                         "  N;\n"
                         "}\n"
                         "module N {\n"
                         "  var x;\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 2);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 1);

  const Variable* x = n->stmt(0)->toVariable();
  assert(x);

  const Identifier* nIdent = m->stmt(1)->toIdentifier();
  assert(nIdent);

  const Scope* scopeForIdent = scopeForId(context, nIdent->id());
  assert(scopeForIdent);

  const auto& match = findInnermostDecl(context, scopeForIdent, nIdent->name());
  assert(match.id() == n->id());
  assert(match.found() == InnermostMatch::ONE);
}

// testing a dotted import statement
static void test10() {
  printf("test10\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test10.chpl");
  std::string contents = "module M {\n"
                         "  import N.x;\n"
                         "  x;\n"
                         "}\n"
                         "module N {\n"
                         "  var x;\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 2);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 1);

  const Variable* x = n->stmt(0)->toVariable();
  assert(x);

  const Identifier* xIdent = m->stmt(1)->toIdentifier();
  assert(xIdent);

  const Scope* scopeForIdent = scopeForId(context, xIdent->id());
  assert(scopeForIdent);

  const auto& match = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(match.id() == x->id());
  assert(match.found() == InnermostMatch::ONE);
}

// testing a dotted import with braces
static void test11() {
  printf("test11\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test11.chpl");
  std::string contents = "module M {\n"
                         "  import N.{x,y};\n"
                         "  x;\n"
                         "  y;\n"
                         "}\n"
                         "module N {\n"
                         "  var x;\n"
                         "  var y;\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 3);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 2);

  const Variable* x = n->stmt(0)->toVariable();
  assert(x);
  const Variable* y = n->stmt(1)->toVariable();
  assert(y);

  const Identifier* xIdent = m->stmt(1)->toIdentifier();
  assert(xIdent);
  const Identifier* yIdent = m->stmt(2)->toIdentifier();
  assert(xIdent);

  const Scope* scopeForIdent = scopeForId(context, xIdent->id());
  assert(scopeForIdent);

  const auto& mx = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(mx.id() == x->id());
  assert(mx.found() == InnermostMatch::ONE);

  const auto& my = findInnermostDecl(context, scopeForIdent, yIdent->name());
  assert(my.id() == y->id());
  assert(my.found() == InnermostMatch::ONE);

}

// testing import of a nested module
static void test12() {
  printf("test12\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test12.chpl");
  std::string contents = "module M {\n"
                         "  import N.NN.NNN.x;\n"
                         "  x;\n"
                         "}\n"
                         "module N {\n"
                         "  module NN {\n"
                         "    module NNN {\n"
                         "      var x;\n"
                         "    }\n"
                         "  }\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 2);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 1);
  const Module* nn = n->stmt(0)->toModule();
  assert(nn);
  const Module* nnn = nn->stmt(0)->toModule();
  assert(nnn);
  const Variable* x = nnn->stmt(0)->toVariable();
  assert(x);

  const Identifier* xIdent = m->stmt(1)->toIdentifier();
  assert(xIdent);

  const Scope* scopeForIdent = scopeForId(context, xIdent->id());
  assert(scopeForIdent);

  const auto& match = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(match.id() == x->id());
  assert(match.found() == InnermostMatch::ONE);
}

// testing use of a nested module
static void test13() {
  printf("test13\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test13.chpl");
  std::string contents = "module M {\n"
                         "  use N.NN.NNN;\n"
                         "  x;\n"
                         "}\n"
                         "module N {\n"
                         "  module NN {\n"
                         "    module NNN {\n"
                         "      var x;\n"
                         "    }\n"
                         "  }\n"
                         "}\n";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 2);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 1);
  const Module* nn = n->stmt(0)->toModule();
  assert(nn);
  const Module* nnn = nn->stmt(0)->toModule();
  assert(nnn);
  const Variable* x = nnn->stmt(0)->toVariable();
  assert(x);

  const Identifier* xIdent = m->stmt(1)->toIdentifier();
  assert(xIdent);

  const Scope* scopeForIdent = scopeForId(context, xIdent->id());
  assert(scopeForIdent);

  const auto& match = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(match.id() == x->id());
  assert(match.found() == InnermostMatch::ONE);
}

// test import of an ambiguous function name
static void test14() {
  printf("test14\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test14.chpl");
  std::string contents = "module M {\n"
                         "  import N.f;\n"
                         "  f;\n"
                         "}\n"
                         "module N {\n"
                         "  proc f(arg: int) { }\n"
                         "  proc f(arg: string) { }\n"
                         "}\n";

  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 2);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 2);

  const Identifier* fIdent = m->stmt(1)->toIdentifier();
  assert(fIdent);

  const Scope* scopeForIdent = scopeForId(context, fIdent->id());
  assert(scopeForIdent);

  const auto& match = findInnermostDecl(context, scopeForIdent, fIdent->name());
  assert(match.found() == InnermostMatch::MANY);
}

// test parsing a module due to a 'use' statement
static void test15() {
  Context ctx;
  Context* context = &ctx;

  std::vector<UniqueString> searchPath;
  searchPath.push_back(UniqueString::get(context, "/test/path/library"));
  searchPath.push_back(UniqueString::get(context, "/test/path/program/"));

  setModuleSearchPath(context, searchPath);

  setFileText(context, "/test/path/program/Program.chpl",
                       "module Program { use Library; libY; }");
  setFileText(context, "/test/path/library/Library.chpl",
                       "module Library { var libY = 3; }");

  auto Program = UniqueString::get(context, "Program");
  auto Library = UniqueString::get(context, "Library");
  auto pMod = getToplevelModule(context, Program);
  assert(pMod != nullptr);
  assert(pMod->numStmts() == 2);

  const Identifier* libYIdent = pMod->stmt(1)->toIdentifier();
  assert(libYIdent);

  const Scope* scopeForIdent = scopeForId(context, libYIdent->id());
  assert(scopeForIdent);

  const auto& match = findInnermostDecl(context, scopeForIdent,
                                        libYIdent->name());
  assert(match.found() == InnermostMatch::ONE);

  auto lMod = getToplevelModule(context, Library);
  assert(lMod != nullptr);
  assert(lMod->numStmts() == 1);

  // finally check that libY resolved to the right variable
  assert(match.id() == lMod->stmt(0)->id());
}

static void test16() {
  printf("test16\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "test16.chpl");
  std::string contents = R""""(
      module M {
        use N;
        use NN;
        x;
      }

      module N {
        module NN {
          var x: int;
        }
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 2);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 3);
  const Identifier* xIdent = m->stmt(2)->toIdentifier();
  assert(xIdent);
  const Module* n = vec[1]->toModule();
  assert(n);
  assert(n->numStmts() == 1);
  const Module* nn = n->stmt(0)->toModule();
  assert(nn);
  assert(nn->numStmts() == 1);
  const Variable* x = nn->stmt(0)->toVariable();
  assert(x);

  const Scope* scopeForIdent = scopeForId(context, xIdent->id());
  assert(scopeForIdent);

  const auto& m1 = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(m1.id() == x->id());
  assert(m1.found() == InnermostMatch::ONE);
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

  return 0;
}

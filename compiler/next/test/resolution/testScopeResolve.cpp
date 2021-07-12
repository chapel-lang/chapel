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

  auto path = UniqueString::build(context, "test1.chpl");
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
  
  auto pair = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(pair.first == x->id());
  assert(pair.second == 1);
}

// testing no matching variable declaration
static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "test2.chpl");
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
  
  auto pair = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(pair.first == ID());
  assert(pair.second == 0);
}

// testing duplicate matching variable declarations
static void test3() {
  printf("test3\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "test3.chpl");
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
  auto pair = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(pair.first == x1->id());
  assert(pair.second == 2);
}

// testing a simple use statement
static void test4() {
  printf("test4\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "test4.chpl");
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
  
  auto pair = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(pair.first == x->id());
  assert(pair.second == 1);
}

// testing a simple recursive use statement
static void test5() {
  printf("test5\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "test5.chpl");
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
  
  auto pair = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(pair.first == x->id());
  assert(pair.second == 1);
}

// testing symbol from parent scope
static void test6() {
  printf("test6\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::build(context, "test6.chpl");
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
  
  auto pair = findInnermostDecl(context, scopeForIdent, xIdent->name());
  assert(pair.first == x->id());
  assert(pair.second == 1);
}


int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();

  return 0;
}

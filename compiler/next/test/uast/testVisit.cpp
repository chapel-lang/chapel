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

#include "chpl/queries/Context.h"
#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/Location.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/uast/all-uast.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

using namespace chpl;
using namespace uast;

std::vector<std::string> actionsLog;

static Builder::Result makeAST(Context* ctx, const uast::Module*& modOut) {
  auto builder = Builder::build(ctx, "path/to/test.chpl");
  Builder* b   = builder.get();
  Location dummyLoc(UniqueString::build(ctx, "path/to/test.chpl"));

  {
    // Create the AST for { a; b; c; }
    auto strA = UniqueString::build(ctx, "a");
    auto strB = UniqueString::build(ctx, "b");
    auto strC = UniqueString::build(ctx, "c");
    ASTList children;

    children.push_back(Identifier::build(b, dummyLoc, strA));
    children.push_back(Identifier::build(b, dummyLoc, strB));
    children.push_back(Identifier::build(b, dummyLoc, strC));
    auto block = Block::build(b, dummyLoc, std::move(children));
    b->addToplevelExpression(std::move(block));
  }

  Builder::Result r = b->result();
  assert(r.errors.size() == 0);
  assert(r.topLevelExpressions.size() == 1);
  assert(r.topLevelExpressions[0]->isModule());
  auto module = r.topLevelExpressions[0]->toModule();
  assert(0 == module->name().compare("test"));
  assert(r.locations.size() == 5); // +1 module
  assert(module->stmt(0)->isBlock());
  const Block* block = module->stmt(0)->toBlock();
  assert(block);
  assert(block->numStmts() == 3);
  assert(block->stmt(0)->isIdentifier());
  assert(block->stmt(1)->isIdentifier());
  assert(block->stmt(2)->isIdentifier());

  modOut = module;
  return r;
}

static void logAction(const char* note, const uast::ASTNode* ast) {
  actionsLog.push_back(std::string(note) + " " +
                       asttags::tagToString(ast->tag()) + " " +
                       ast->id().toString());
}

struct Visitor1 {
  double visit(const uast::ASTNode* ast) {
    logAction("in visit(ASTNode)", ast);
    return 10.0;
  }
};

static void test1() {
  Context context;
  const uast::Module* mod = nullptr;

  auto result = makeAST(&context, mod);
  const ASTNode* ast = mod;

  {
    actionsLog.clear();
    Visitor1 v;
    double got = mod->dispatch<double>(v);
    assert(got == 10.0);
    assert(actionsLog.size()==1);
    assert(actionsLog[0] == "in visit(ASTNode) Module test@4");
  }

  {
    actionsLog.clear();
    Visitor1 v;
    double got = ast->dispatch<double>(v);
    assert(got == 10.0);
    assert(actionsLog.size()==1);
    assert(actionsLog[0] == "in visit(ASTNode) Module test@4");
  }
}

struct Visitor2 {
  double visit(const uast::NamedDecl* ast) {
    logAction("in visit(NamedDecl)", ast);
    return 10.0;
  }
  double visit(const uast::ASTNode* ast) {
    logAction("in visit(ASTNode)", ast);
    return 11.0;
  }
};

static void test2() {
  Context context;
  const uast::Module* mod = nullptr;

  auto result = makeAST(&context, mod);
  const ASTNode* ast = mod;

  {
    actionsLog.clear();
    Visitor2 v;
    double got = ast->dispatch<double>(v);
    assert(got == 10.0);
    assert(actionsLog.size()==1);
    assert(actionsLog[0] == "in visit(NamedDecl) Module test@4");
  }
}

struct Visitor3 {
  double visit(const uast::Module* ast) {
    logAction("in visit(Module)", ast);
    return 10.0;
  }
  double visit(const uast::ASTNode* ast) {
    logAction("in visit(ASTNode)", ast);
    return 11.0;
  }
};

static void test3() {
  Context context;
  const uast::Module* mod = nullptr;

  auto result = makeAST(&context, mod);
  const ASTNode* ast = mod;

  {
    actionsLog.clear();
    Visitor3 v;
    double got = ast->dispatch<double>(v);
    assert(got == 10.0);
    assert(actionsLog.size()==1);
    assert(actionsLog[0] == "in visit(Module) Module test@4");
  }
}

struct Visitor4 {
  bool enter(const uast::Module* ast) {
    logAction("in enter(Module)", ast);
    return true;
  }
  void exit(const uast::Module* ast) {
    logAction("in exit(Module)", ast);
  }
  bool enter(const uast::ASTNode* ast) {
    logAction("in enter(ASTNode)", ast);
    return false;
  }
  void exit(const uast::ASTNode* ast) {
    logAction("in exit(ASTNode)", ast);
  }
};

static void test4() {
  Context context;
  const uast::Module* mod = nullptr;

  auto result = makeAST(&context, mod);
  const ASTNode* ast = mod;

  {
    actionsLog.clear();
    Visitor4 v;
    ast->traverse(v);
    assert(actionsLog.size()==3);
    assert(actionsLog[0] == "in enter(Module) Module test@4");
    assert(actionsLog[1] == "in enter(ASTNode) Block test@3");
    assert(actionsLog[2] == "in exit(Module) Module test@4");
  }
}

struct Visitor5 {
  bool enter(const uast::Module* ast) {
    logAction("in enter(Module)", ast);
    return true;
  }
  void exit(const uast::Module* ast) {
    logAction("in exit(Module)", ast);
  }
  bool enter(const uast::Identifier* ast) {
    logAction("in enter(Identifier)", ast);
    return false;
  }
  void exit(const uast::Identifier* ast) {
    logAction("in exit(Identifier)", ast);
  }
  bool enter(const uast::ASTNode* ast) {
    logAction("in enter(ASTNode)", ast);
    return true;
  }
  void exit(const uast::ASTNode* ast) {
    logAction("in exit(ASTNode)", ast);
  }
};

static void test5() {
  Context context;
  const uast::Module* mod = nullptr;

  auto result = makeAST(&context, mod);
  const ASTNode* ast = mod;

  {
    actionsLog.clear();
    Visitor5 v;
    ast->traverse(v);
    assert(actionsLog.size()==7);
    assert(actionsLog[0] == "in enter(Module) Module test@4");
    assert(actionsLog[1] == "in enter(ASTNode) Block test@3");
    assert(actionsLog[2] == "in enter(Identifier) Identifier test@0");
    assert(actionsLog[3] == "in enter(Identifier) Identifier test@1");
    assert(actionsLog[4] == "in enter(Identifier) Identifier test@2");
    assert(actionsLog[5] == "in exit(ASTNode) Block test@3");
    assert(actionsLog[6] == "in exit(Module) Module test@4");
  }
}

int main(int argc, char** argv) {
  test1();
  test2();
  test3();
  test4();
  test5();

  return 0;
}

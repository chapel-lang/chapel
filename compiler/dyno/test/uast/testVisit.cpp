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

static BuilderResult makeAST(Context* ctx, const uast::Module*& modOut) {
  auto builder = Builder::build(ctx, "path/to/test.chpl");
  Builder* b   = builder.get();
  Location dummyLoc(UniqueString::get(ctx, "path/to/test.chpl"));

  {
    // Create the AST for { a; b; c; }
    auto strA = UniqueString::get(ctx, "a");
    auto strB = UniqueString::get(ctx, "b");
    auto strC = UniqueString::get(ctx, "c");
    AstList children;

    children.push_back(Identifier::build(b, dummyLoc, strA));
    children.push_back(Identifier::build(b, dummyLoc, strB));
    children.push_back(Identifier::build(b, dummyLoc, strC));
    auto block = Block::build(b, dummyLoc, std::move(children));
    b->addToplevelExpression(std::move(block));
  }

  BuilderResult r = b->result();
  assert(!r.numErrors());
  auto mod = r.singleModule();
  assert(mod);
  assert(0 == mod->name().compare("test"));
  //assert(r.astToLocation.size() == 5); // +1 module
  assert(mod->stmt(0)->isBlock());
  const Block* block = mod->stmt(0)->toBlock();
  assert(block);
  assert(block->numStmts() == 3);
  assert(block->stmt(0)->isIdentifier());
  assert(block->stmt(1)->isIdentifier());
  assert(block->stmt(2)->isIdentifier());

  modOut = mod;
  return r;
}

static void logAction(std::vector<std::string>& actionsLog,
                      const char* note,
                      const uast::AstNode* ast) {
  std::ostringstream ss;
  ss << note << " " << asttags::tagToString(ast->tag()) << " ";
  ast->id().stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
  actionsLog.push_back(ss.str());
}

struct Visitor1 {
  std::vector<std::string> actionsLog;

  double visit(const uast::AstNode* ast) {
    logAction(actionsLog, "in visit(AstNode)", ast);
    return 10.0;
  }
};

static void test1() {
  Context context;
  const uast::Module* mod = nullptr;

  auto result = makeAST(&context, mod);

  Visitor1 v;
  double got = mod->dispatch<double>(v);
  assert(got == 10.0);
  assert(v.actionsLog.size()==1);
  assert(v.actionsLog[0] == "in visit(AstNode) Module test");
}

struct Visitor2 {
  std::vector<std::string> actionsLog;

  double visit(const uast::NamedDecl* ast) {
    logAction(actionsLog, "in visit(NamedDecl)", ast);
    return 10.0;
  }
  double visit(const uast::AstNode* ast) {
    logAction(actionsLog, "in visit(AstNode)", ast);
    return 11.0;
  }
};

static void test2() {
  Context context;
  const uast::Module* mod = nullptr;

  auto result = makeAST(&context, mod);
  const AstNode* ast = mod;

  Visitor2 v;
  double got = ast->dispatch<double>(v);
  assert(got == 10.0);
  assert(v.actionsLog.size()==1);
  assert(v.actionsLog[0] == "in visit(NamedDecl) Module test");
}

struct Visitor3 {
  std::vector<std::string> actionsLog;

  double visit(const uast::Module* ast) {
    logAction(actionsLog, "in visit(Module)", ast);
    return 10.0;
  }
  double visit(const uast::AstNode* ast) {
    logAction(actionsLog, "in visit(AstNode)", ast);
    return 11.0;
  }
};

static void test3() {
  Context context;
  const uast::Module* mod = nullptr;

  auto result = makeAST(&context, mod);
  const AstNode* ast = mod;

  Visitor3 v;
  double got = ast->dispatch<double>(v);
  assert(got == 10.0);
  assert(v.actionsLog.size()==1);
  assert(v.actionsLog[0] == "in visit(Module) Module test");
}

struct Visitor4 {
  std::vector<std::string> actionsLog;

  bool enter(const uast::Module* ast) {
    logAction(actionsLog, "in enter(Module)", ast);
    return true;
  }
  void exit(const uast::Module* ast) {
    logAction(actionsLog, "in exit(Module)", ast);
  }
  bool enter(const uast::AstNode* ast) {
    logAction(actionsLog, "in enter(AstNode)", ast);
    return false;
  }
  void exit(const uast::AstNode* ast) {
    logAction(actionsLog, "in exit(AstNode)", ast);
  }
};

static void test4() {
  Context context;
  const uast::Module* mod = nullptr;

  auto result = makeAST(&context, mod);
  const AstNode* ast = mod;

  Visitor4 v;
  ast->traverse(v);
  assert(v.actionsLog.size()==4);
  assert(v.actionsLog[0] == "in enter(Module) Module test");
  assert(v.actionsLog[1] == "in enter(AstNode) Block test@3");
  assert(v.actionsLog[2] == "in exit(AstNode) Block test@3");
  assert(v.actionsLog[3] == "in exit(Module) Module test");
}

struct Visitor5 {
  std::vector<std::string> actionsLog;

  bool enter(const uast::Module* ast) {
    logAction(actionsLog, "in enter(Module)", ast);
    return true;
  }
  void exit(const uast::Module* ast) {
    logAction(actionsLog, "in exit(Module)", ast);
  }
  bool enter(const uast::Identifier* ast) {
    logAction(actionsLog, "in enter(Identifier)", ast);
    return false;
  }
  void exit(const uast::Identifier* ast) {
    logAction(actionsLog, "in exit(Identifier)", ast);
  }
  bool enter(const uast::AstNode* ast) {
    logAction(actionsLog, "in enter(AstNode)", ast);
    return true;
  }
  void exit(const uast::AstNode* ast) {
    logAction(actionsLog, "in exit(AstNode)", ast);
  }
};

static void test5() {
  Context context;
  const uast::Module* mod = nullptr;

  auto result = makeAST(&context, mod);
  const AstNode* ast = mod;

  Visitor5 v;
  ast->traverse(v);
  assert(v.actionsLog.size()==10);
  assert(v.actionsLog[0] == "in enter(Module) Module test");
  assert(v.actionsLog[1] == "in enter(AstNode) Block test@3");
  assert(v.actionsLog[2] == "in enter(Identifier) Identifier test@0");
  assert(v.actionsLog[3] == "in exit(Identifier) Identifier test@0");
  assert(v.actionsLog[4] == "in enter(Identifier) Identifier test@1");
  assert(v.actionsLog[5] == "in exit(Identifier) Identifier test@1");
  assert(v.actionsLog[6] == "in enter(Identifier) Identifier test@2");
  assert(v.actionsLog[7] == "in exit(Identifier) Identifier test@2");
  assert(v.actionsLog[8] == "in exit(AstNode) Block test@3");
  assert(v.actionsLog[9] == "in exit(Module) Module test");
}

int main(int argc, char** argv) {
  test1();
  test2();
  test3();
  test4();
  test5();

  return 0;
}

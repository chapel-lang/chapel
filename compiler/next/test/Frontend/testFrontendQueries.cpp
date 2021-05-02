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

#include "chpl/Frontend/FrontendQueries.h"

#include "chpl/AST/ModuleDecl.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace uast;
using namespace frontend;

static void test0() {
  printf("test0\n");
  auto context = Context::build();
  Context* ctx = context.get();

  auto path = UniqueString::build(ctx, "input.chpl");
  std::string contents = "/* this is a test */";
  setFileText(ctx, path, contents);
 
  std::string gotContents = fileText(ctx, path);
  assert(gotContents == contents);
}

static void test1() {
  printf("test1\n");
  auto context = Context::build();
  Context* ctx = context.get();

  auto path = UniqueString::build(ctx, "input.chpl");
  std::string contents = "/* this is a test */";
  setFileText(ctx, path, contents);
 
  parse(ctx, path);
}

static const Module* parseOneModule(Context* ctx, UniqueString filepath) {
  const ModuleDeclVec& v = parse(ctx, filepath);
  assert(v.size() == 1);
  return v[0]->module();
}

static void test2() {
  printf("test2\n");
  auto context = Context::build();
  Context* ctx = context.get();

  ctx->advanceToNextRevision(true);
  auto modOnePath = UniqueString::build(ctx, "modOne.chpl");
  std::string modOneContents = "/* this is a test */\n"
                               "a;\n";
  setFileText(ctx, modOnePath, modOneContents);
  auto modTwoPath = UniqueString::build(ctx, "modTwo.chpl");
  std::string modTwoContents = "/* this is a another test */"
                               "a;\n";
  setFileText(ctx, modTwoPath, modTwoContents);
 
  const Module* moduleOne = nullptr;
  const Module* moduleTwo = nullptr;

  moduleOne = parseOneModule(ctx, modOnePath);
  moduleTwo = parseOneModule(ctx, modTwoPath);
  assert(moduleOne->numStmts() == 2);
  assert(moduleTwo->numStmts() == 2);
  ASTBase::dump(moduleOne);
  ASTBase::dump(moduleTwo);

  const Module* oldModuleOne = moduleOne;
  const Module* oldModuleTwo = moduleTwo;
  ctx->collectGarbage();

  printf("test2 changing whitespace in modOne.chpl\n");
  ctx->advanceToNextRevision(true);

  modOneContents = "/* this is a test */\n"
                   "\n"
                   "\n"
                   "a;\n";
  setFileText(ctx, modOnePath, modOneContents);
  setFileText(ctx, modTwoPath, modTwoContents);

  printf("test2 parsing modOne.chpl\n");
  moduleOne = parseOneModule(ctx, modOnePath);
  printf("test2 parsing modTwo.chpl\n");
  moduleTwo = parseOneModule(ctx, modTwoPath);
  assert(moduleOne->numStmts() == 2);
  assert(moduleTwo->numStmts() == 2);
  ASTBase::dump(moduleOne);
  ASTBase::dump(moduleTwo);

  // Check that the pointer values didn't change
  // (they shouldn't because we should have reused the modules).
  assert(moduleOne == oldModuleOne);
  assert(moduleTwo == oldModuleTwo);

  ctx->collectGarbage();
}

static void test3() {
  printf("test3\n");
  auto context = Context::build();
  Context* ctx = context.get();

  auto modulePath = UniqueString::build(ctx, "MyModule.chpl");
  const Module* module = nullptr;
  const Comment* comment = nullptr;
  const Identifier* identifierA = nullptr;
  const Identifier* identifierB = nullptr;
  const Block* block = nullptr;

  std::string moduleContents;

  moduleContents = "/* this is a test */\n"
                   "a;\n"
                   "b;\n";
  ctx->advanceToNextRevision(true);
  setFileText(ctx, modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  ASTBase::dump(module);
  assert(module->numStmts() == 3);
  comment = module->stmt(0)->toComment();
  identifierA = module->stmt(1)->toIdentifier();
  identifierB = module->stmt(2)->toIdentifier();
  assert(comment);
  assert(identifierA);
  assert(identifierB);
  const Comment* oldComment = comment;
  const Identifier* oldIdentifierA = identifierA;
  const Identifier* oldIdentifierB = identifierB;

  printf("test3 adding Blocks\n");
  moduleContents = "/* this is a test */\n"
                   "{ x; }\n"
                   "a;\n"
                   "{ y; }\n"
                   "b;\n";
  ctx->advanceToNextRevision(true);
  setFileText(ctx, modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  ASTBase::dump(module);

  // Check that the pointer values didn't change
  // (they shouldn't because we should have reused the parts that didn't change)
  assert(module->numStmts() == 5);
  comment = module->stmt(0)->toComment();
  block = module->stmt(1)->toBlock();
  identifierA = module->stmt(2)->toIdentifier();
  identifierB = module->stmt(4)->toIdentifier();
  assert(comment);
  assert(identifierA);
  assert(identifierB);
  assert(comment == oldComment);
  assert(identifierA == oldIdentifierA);
  assert(identifierB == oldIdentifierB);
  const Block* oldBlock = block;

  printf("test3 changing Identifier in Blocks\n");
  moduleContents = "/* this is a test */\n"
                   "{ xx; }\n"
                   "a;\n"
                   "{ yy; }\n"
                   "b;\n";
  ctx->advanceToNextRevision(true);
  setFileText(ctx, modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  ASTBase::dump(module);

  // Check that the block and identifiers match
  assert(module->numStmts() == 5);
  comment = module->stmt(0)->toComment();
  block = module->stmt(1)->toBlock();
  identifierA = module->stmt(2)->toIdentifier();
  identifierB = module->stmt(4)->toIdentifier();
  assert(comment);
  assert(identifierA);
  assert(identifierB);
  assert(comment == oldComment);
  assert(block == oldBlock);
  assert(identifierA == oldIdentifierA);
  assert(identifierB == oldIdentifierB);
  oldBlock = nullptr; // it will be invalid after this point

  printf("test3 removing the Blocks\n");
  moduleContents = "/* this is a test */\n"
                   "a;\n"
                   "b;\n";
  ctx->advanceToNextRevision(true);
  setFileText(ctx, modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  ASTBase::dump(module);

  // Check that the comment and identifiers match
  assert(module->numStmts() == 3);
  comment = module->stmt(0)->toComment();
  identifierA = module->stmt(1)->toIdentifier();
  identifierB = module->stmt(2)->toIdentifier();
  assert(comment);
  assert(identifierA);
  assert(identifierB);
  assert(comment == oldComment);
  assert(identifierA == oldIdentifierA);
  assert(identifierB == oldIdentifierB);
  oldIdentifierA = nullptr; // it will be invalid after this point

  printf("test3 replacing first Identifier\n");
  moduleContents = "/* this is a test */\n"
                   "aa;\n"
                   "b;\n";
  ctx->advanceToNextRevision(true);
  setFileText(ctx, modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  ASTBase::dump(module);

  // Check that the comment and identifiers match
  assert(module->numStmts() == 3);
  comment = module->stmt(0)->toComment();
  identifierA = module->stmt(1)->toIdentifier();
  identifierB = module->stmt(2)->toIdentifier();
  assert(comment);
  assert(identifierA);
  assert(identifierB);
  assert(comment == oldComment);
  assert(identifierB == oldIdentifierB);
}

static void test4() {
  printf("test4\n");
  auto context = Context::build();
  Context* ctx = context.get();

  auto modulePath = UniqueString::build(ctx, "MyModule.chpl");
  const Module* module = nullptr;
  const Comment* comment = nullptr;
  const Decl* declA = nullptr;
  const Decl* declB = nullptr;
  const Block* block = nullptr;

  std::string moduleContents;

  moduleContents = "/* this is a test */\n"
                   "var a;\n"
                   "var b;\n";
  ctx->advanceToNextRevision(true);
  setFileText(ctx, modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  ASTBase::dump(module);
  assert(module->numStmts() == 3);
  comment = module->stmt(0)->toComment();
  declA = module->stmt(1)->toDecl();
  declB = module->stmt(2)->toDecl();
  assert(comment);
  assert(declA);
  assert(declB);
  const Comment* oldComment = comment;
  const Decl* oldDeclA = declA;
  const Decl* oldDeclB = declB;

  printf("test4 adding Blocks\n");
  moduleContents = "/* this is a test */\n"
                   "{ var x; }\n"
                   "var a;\n"
                   "{ var y; }\n"
                   "var b;\n";
  ctx->advanceToNextRevision(true);
  setFileText(ctx, modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  ASTBase::dump(module);

  // Check that the pointer values didn't change
  // (they shouldn't because we should have reused the parts that didn't change)
  assert(module->numStmts() == 5);
  comment = module->stmt(0)->toComment();
  block = module->stmt(1)->toBlock();
  declA = module->stmt(2)->toDecl();
  declB = module->stmt(4)->toDecl();
  assert(comment);
  assert(declA);
  assert(declB);
  assert(comment == oldComment);
  assert(declA == oldDeclA);
  assert(declB == oldDeclB);
  const Block* oldBlock = block;

  printf("test4 changing Identifier in Blocks\n");
  moduleContents = "/* this is a test */\n"
                   "{ var xx; }\n"
                   "var a;\n"
                   "{ var yy; }\n"
                   "var b;\n";
  ctx->advanceToNextRevision(true);
  setFileText(ctx, modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  ASTBase::dump(module);

  // Check that the block and identifiers match
  assert(module->numStmts() == 5);
  comment = module->stmt(0)->toComment();
  block = module->stmt(1)->toBlock();
  declA = module->stmt(2)->toDecl();
  declB = module->stmt(4)->toDecl();
  assert(comment);
  assert(declA);
  assert(declB);
  assert(comment == oldComment);
  assert(block == oldBlock);
  assert(declA == oldDeclA);
  assert(declB == oldDeclB);
  oldBlock = nullptr; // it will be invalid after this point

  printf("test4 removing the Blocks\n");
  moduleContents = "/* this is a test */\n"
                   "var a;\n"
                   "var b;\n";
  ctx->advanceToNextRevision(true);
  setFileText(ctx, modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  ASTBase::dump(module);

  // Check that the comment and identifiers match
  assert(module->numStmts() == 3);
  comment = module->stmt(0)->toComment();
  declA = module->stmt(1)->toDecl();
  declB = module->stmt(2)->toDecl();
  assert(comment);
  assert(declA);
  assert(declB);
  assert(comment == oldComment);
  assert(declA == oldDeclA);
  assert(declB == oldDeclB);
  oldDeclA = nullptr; // it will be invalid after this point

  printf("test4 replacing first Decl\n");
  moduleContents = "/* this is a test */\n"
                   "var aa;\n"
                   "var b;\n";
  ctx->advanceToNextRevision(true);
  setFileText(ctx, modulePath, moduleContents);
  module = parseOneModule(ctx, modulePath);
  ctx->collectGarbage();

  ASTBase::dump(module);

  // Check that the comment and identifiers match
  assert(module->numStmts() == 3);
  comment = module->stmt(0)->toComment();
  declA = module->stmt(1)->toDecl();
  declB = module->stmt(2)->toDecl();
  assert(comment);
  assert(declA);
  assert(declB);
  assert(comment == oldComment);
  assert(declB == oldDeclB);
}

// TODO: test locate

int main() {
  test0();
  test1();
  test2();
  test3();
  test4();

  return 0;
}

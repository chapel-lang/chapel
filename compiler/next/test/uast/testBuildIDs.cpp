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
#include "chpl/uast/Block.h"
#include "chpl/uast/Builder.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

using namespace chpl;
using namespace uast;

static void test0() {
  Context context;
  Context* ctx = &context;

  // test some operations on locations
  Location locOne(UniqueString::build(ctx, "a.chpl"));
  Location locTwo(UniqueString::build(ctx, "aVeryLongFilenameIndeed.chpl"));
  Location locOneCopy(locOne.path());
  Location locTwoCopy(locTwo.path());
  Location locOneA(locOne.path(),  1, -1, -1, -1);
  Location locOneB(locOne.path(), -1,  1, -1, -1);
  Location locOneC(locOne.path(), -1, -1,  1, -1);
  Location locOneD(locOne.path(), -1, -1, -1,  1);
  Location emptyLoc;

  assert(emptyLoc == emptyLoc);
  assert(!(emptyLoc != emptyLoc));
  assert(emptyLoc != locOne);
  assert(!(emptyLoc == locOne));
  assert(locOne == locOne);
  assert(!(locOne != locOne));
  assert(locOne != locTwo);
  assert(!(locOne == locTwo));
  assert(locOneCopy == locOne);
  assert(!(locOneCopy != locOne));
  assert(locTwoCopy == locTwo);
  assert(!(locTwoCopy != locTwo));
  assert(locOneA != locOne);
  assert(!(locOneA == locOne));
  assert(locOneB != locOne);
  assert(!(locOneB == locOne));
  assert(locOneC != locOne);
  assert(!(locOneC == locOne));
  assert(locOneD != locOne);
  assert(!(locOneD == locOne));

  Location locA(locOne.path(), 1, 2, 3, 4);
  Location locB(locTwo.path(), 10, 20, 30, 40);
  // test copy init
  Location locACopy = locA;
  Location locBCopy = locB;
  assert(locACopy == locA);
  assert(locBCopy == locB);
  // test =
  Location locAassign;
  Location locBassign;
  locAassign = locA;
  locBassign = locB;
  assert(locAassign == locA);
  assert(locBassign == locB);
  // test swap
  locACopy.swap(locBCopy);
  assert(locACopy == locB);
  assert(locBCopy == locA);

  // test hash compiles and doesn't happen to collide these two
  assert(locOne.hash() != locTwo.hash());
}

static void test1() {
  Context context;
  Context* ctx = &context;

  // test some operations on IDs
  auto shortpath = UniqueString::build(ctx, "a.chpl");
  auto longpath = UniqueString::build(ctx, "aVeryLongFilenameIndeed.chpl");

  ID emptyID;

  assert(emptyID == emptyID);
  assert(!(emptyID != emptyID));

  ID idA(shortpath, 1, 2);
  ID idB(longpath,  1, 2);
  ID idC(shortpath, 9, 2);
  ID idD(shortpath, 1, 9);

  assert(idA != idB);
  assert(!(idA == idB));
  assert(idA != idC);
  assert(!(idA == idC));

  assert(idA == idD);    // idD == idA because the num child ids
  assert(!(idA != idD)); // is not important for comparison

  // test copy init
  ID idACopy = idA;
  ID idBCopy = idB;
  assert(idACopy == idA);
  assert(idBCopy == idB);

  // test assign
  ID idAassign;
  ID idBassign;
  idAassign = idA;
  idBassign = idB;
  assert(idAassign == idA);
  assert(idBassign == idB);

  // test swap
  idACopy.swap(idBCopy);
  assert(idACopy == idB);
  assert(idBCopy == idA);

  // test hash compiles and doesn't happen to collide these two
  assert(idA.hash() != idB.hash());
}

static  void test2() {
  Context context;
  Context* ctx = &context;

  auto builder = Builder::build(ctx, "path/to/test.chpl");
  Builder* b   = builder.get();
  Location emptyLoc;

  {
    // Create the AST for { a; b; c; }
    auto strA = UniqueString::build(ctx, "a");
    auto strB = UniqueString::build(ctx, "b");
    auto strC = UniqueString::build(ctx, "c");
    ASTList children;

    children.push_back(Identifier::build(b, emptyLoc, strA));
    children.push_back(Identifier::build(b, emptyLoc, strB));
    children.push_back(Identifier::build(b, emptyLoc, strC));
    auto block = Block::build(b, emptyLoc, std::move(children));
    b->addToplevelExpression(std::move(block));
  }

  Builder::Result r = b->result();
  assert(r.errors.size() == 0);
  assert(r.topLevelExpressions.size() == 1);
  assert(r.topLevelExpressions[0]->isModule());
  auto module = r.topLevelExpressions[0]->toModule();
  assert(r.locations.size() == 5); // +1 module
  assert(module->stmt(0)->isBlock());
  const Block* block = module->stmt(0)->toBlock();
  assert(block);
  assert(block->numStmts() == 3);
  assert(block->stmt(0)->isIdentifier());
  assert(block->stmt(1)->isIdentifier());
  assert(block->stmt(2)->isIdentifier());

  // now check the IDs
  assert(block->stmt(0)->id().postOrderId() == 0);
  assert(block->stmt(0)->id().numContainedChildren() == 0);
  assert(block->stmt(1)->id().postOrderId() == 1);
  assert(block->stmt(1)->id().numContainedChildren() == 0);
  assert(block->stmt(2)->id().postOrderId() == 2);
  assert(block->stmt(2)->id().numContainedChildren() == 0);
  assert(block->id().postOrderId() == 3);
  assert(block->id().numContainedChildren() == 3);
  assert(module->id().postOrderId() == 4);
  assert(module->id().numContainedChildren() == 4);

  // now check containment on the ids
  assert(block->id().contains(block->id()));
  assert(block->id().contains(block->stmt(0)->id()));
  assert(block->id().contains(block->stmt(1)->id()));
  assert(block->id().contains(block->stmt(2)->id()));
 
  assert(module->id().contains(module->id()));
  assert(module->id().contains(block->id()));
  assert(module->id().contains(block->stmt(0)->id()));
  assert(module->id().contains(block->stmt(1)->id()));
  assert(module->id().contains(block->stmt(2)->id()));

  assert(!block->stmt(0)->id().contains(block->id()));
  assert(!block->stmt(0)->id().contains(block->stmt(1)->id()));
  assert(!block->stmt(0)->id().contains(block->stmt(2)->id()));
  assert(!block->stmt(1)->id().contains(block->id()));
  assert(!block->stmt(1)->id().contains(block->stmt(0)->id()));
  assert(!block->stmt(1)->id().contains(block->stmt(2)->id()));
  assert(!block->stmt(2)->id().contains(block->id()));
  assert(!block->stmt(2)->id().contains(block->stmt(0)->id()));
  assert(!block->stmt(2)->id().contains(block->stmt(1)->id()));

  // now check comparison on the IDs
  assert(block->id().compare(block->id()) == 0);
  assert(block->id() == block->id());
  assert(!(block->id() != block->id()));
  assert(block->id().compare(block->stmt(0)->id()) > 0);
  assert(!(block->id() == block->stmt(0)->id()));
  assert(block->id() != block->stmt(0)->id());
  assert(block->id().compare(block->stmt(1)->id()) > 0);
  assert(!(block->id() == block->stmt(1)->id()));
  assert(block->id() != block->stmt(1)->id());
  assert(block->id().compare(block->stmt(2)->id()) > 0);
  assert(!(block->id() == block->stmt(2)->id()));
  assert(block->id() != block->stmt(2)->id());

  assert(block->stmt(0)->id().compare(block->stmt(0)->id()) == 0);
  assert(block->stmt(0)->id().compare(block->stmt(1)->id()) < 0);
  assert(block->stmt(1)->id().compare(block->stmt(2)->id()) < 0);
}

static void test3() {
  Context context;
  Context* ctx = &context;

  auto builder = Builder::build(ctx, "path/to/test.chpl");
  Builder* b   = builder.get();
  Location emptyLoc;

  {
    // Create the AST for { a; { } { b; } c; }
    auto strA = UniqueString::build(ctx, "a");
    auto strB = UniqueString::build(ctx, "b");
    auto strC = UniqueString::build(ctx, "c");

    ASTList outer;
    outer.push_back(Identifier::build(b, emptyLoc, strA));
    {
      ASTList empty;
      outer.push_back(Block::build(b, emptyLoc, std::move(empty)));
    }
    {
      ASTList block;
      block.push_back(Identifier::build(b, emptyLoc, strB));
      outer.push_back(Block::build(b, emptyLoc, std::move(block)));
    }
    outer.push_back(Identifier::build(b, emptyLoc, strC));
    auto block = Block::build(b, emptyLoc, std::move(outer));
    b->addToplevelExpression(std::move(block));
  }

  Builder::Result r = b->result();
  assert(r.errors.size() == 0);
  assert(r.topLevelExpressions.size() == 1);
  assert(r.topLevelExpressions[0]->isModule());
  auto module = r.topLevelExpressions[0]->toModule();
  assert(r.locations.size() == 7); // +1 module
  assert(module->stmt(0)->isBlock());
  const Block* outer = module->stmt(0)->toBlock();
  assert(outer);
  assert(outer->numStmts() == 4);
  assert(outer->stmt(0)->isIdentifier());
  assert(outer->stmt(1)->isBlock());
  assert(outer->stmt(2)->isBlock());
  assert(outer->stmt(3)->isIdentifier());
  const Block* empty = outer->stmt(1)->toBlock();
  assert(empty->numStmts() == 0);
  const Block* block = outer->stmt(2)->toBlock();
  assert(block->numStmts() == 1);

  // now check the IDs
  assert(outer->stmt(0)->id().postOrderId() == 0);
  assert(outer->stmt(0)->id().numContainedChildren() == 0);
  assert(empty->id().postOrderId() == 1);
  assert(empty->id().numContainedChildren() == 0);
  assert(block->stmt(0)->id().postOrderId() == 2);
  assert(block->stmt(0)->id().numContainedChildren() == 0);
  assert(block->id().postOrderId() == 3);
  assert(block->id().numContainedChildren() == 1);
  assert(outer->stmt(3)->id().postOrderId() == 4);
  assert(outer->stmt(3)->id().numContainedChildren() == 0);

  assert(!outer->stmt(2)->id().contains(outer->stmt(3)->id()));
  assert(!outer->stmt(3)->id().contains(outer->stmt(2)->id()));
  assert(!outer->stmt(3)->id().contains(outer->id()));
}

static void test4() {
  Context context;
  Context* ctx = &context;

  auto builder = Builder::build(ctx, "path/to/test.chpl");
  Builder* b   = builder.get();
  Location emptyLoc;

  {
    /* Create the AST for

      module M {
        module I {
          a;
          b;
          c;
        }
        x;
      }

     */

    auto strA = UniqueString::build(ctx, "a");
    auto strB = UniqueString::build(ctx, "b");
    auto strC = UniqueString::build(ctx, "c");
    auto strX = UniqueString::build(ctx, "x");
    auto strM = UniqueString::build(ctx, "M");
    auto strI = UniqueString::build(ctx, "I");

    ASTList inner;
    {
      ASTList ii;
      ii.push_back(Identifier::build(b, emptyLoc, strA));
      ii.push_back(Identifier::build(b, emptyLoc, strB));
      ii.push_back(Identifier::build(b, emptyLoc, strC));
      inner.push_back(Module::build(b, emptyLoc, strI,
                                    Decl::DEFAULT_VISIBILITY,
                                    Module::DEFAULT_MODULE_KIND,
                                    std::move(ii)));
    }
    inner.push_back(Identifier::build(b, emptyLoc, strX));

    auto mod = Module::build(b, emptyLoc, strM,
                             Decl::DEFAULT_VISIBILITY,
                             Module::DEFAULT_MODULE_KIND,
                             std::move(inner));

    b->addToplevelExpression(std::move(mod));
  }

  Builder::Result r = b->result();
  assert(r.errors.size() == 0);
  assert(r.topLevelExpressions.size() == 1);
  assert(r.topLevelExpressions[0]->isModule());
  auto modM = r.topLevelExpressions[0]->toModule();
  assert(r.locations.size() == 6);
  assert(modM->stmt(0)->isModule());
  auto modI = modM->stmt(0)->toModule();
  assert(modI->numStmts() == 3);
  assert(modM->stmt(1)->isIdentifier());

  // now check the IDs
  assert(modI->id().postOrderId() == 3);
  assert(modI->id().numContainedChildren() == 3);
  assert(modM->stmt(1)->id().postOrderId() == 0);
  assert(modM->stmt(1)->id().numContainedChildren() == 0);
  assert(modM->id().postOrderId() == 1);
  assert(modM->id().numContainedChildren() == 1);

  assert(modI->stmt(0)->id().postOrderId() == 0);
  assert(modI->stmt(0)->id().numContainedChildren() == 0);
  assert(modI->stmt(1)->id().postOrderId() == 1);
  assert(modI->stmt(1)->id().numContainedChildren() == 0);
  assert(modI->stmt(2)->id().postOrderId() == 2);
  assert(modI->stmt(2)->id().numContainedChildren() == 0);

  // check ID containment
  assert(modM->id().contains(modM->id()));
  assert(modM->id().contains(modM->stmt(0)->id()));
  assert(modM->id().contains(modM->stmt(1)->id()));
  assert(modM->id().contains(modI->id()));
  assert(modM->id().contains(modI->stmt(0)->id()));
  assert(modM->id().contains(modI->stmt(1)->id()));
  assert(modM->id().contains(modI->stmt(2)->id()));
  assert(modI->id().contains(modI->id()));
  assert(modI->id().contains(modI->stmt(0)->id()));
  assert(modI->id().contains(modI->stmt(1)->id()));
  assert(modI->id().contains(modI->stmt(2)->id()));

  assert(!modI->id().contains(modM->id()));
  assert(!modI->id().contains(modM->stmt(1)->id()));
}


int main(int argc, char** argv) {
  test0();
  test1();
  test2();
  test3();
  test4();
  return 0;
}

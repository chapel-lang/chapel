#include "chpl/AST/BlockStmt.h"
#include "chpl/AST/Builder.h"
#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Expr.h"
#include "chpl/AST/Identifier.h"
#include "chpl/AST/Location.h"
#include "chpl/AST/UniqueString.h"
#include "chpl/Queries/Context.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

using namespace chpl;
using namespace ast;

void test0() {
  // test some operations on locations
  auto context = Context::build();
  Context* ctx = context.get();
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

void test1() {
  // test some operations on IDs
  auto context = Context::build();
  Context* ctx = context.get();
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

void test2() {
  auto context = Context::build();
  Context* ctx = context.get();
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
    auto block = BlockStmt::build(b, emptyLoc, std::move(children));
    b->addToplevelExpr(std::move(block));
  }

  Builder::Result r = b->result();
  assert(r.topLevelExprs.size() == 1);
  assert(r.errors.size() == 0);
  assert(r.locations.size() == 4);
  BlockStmt* block = r.topLevelExprs[0]->toBlockStmt();
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

  // now check containment on the ids
  assert(block->id().contains(block->id()));
  assert(block->id().contains(block->stmt(0)->id()));
  assert(block->id().contains(block->stmt(1)->id()));
  assert(block->id().contains(block->stmt(2)->id()));
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

void test3() {
  auto context = Context::build();
  Context* ctx = context.get();
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
      outer.push_back(BlockStmt::build(b, emptyLoc, std::move(empty)));
    }
    {
      ASTList block;
      block.push_back(Identifier::build(b, emptyLoc, strB));
      outer.push_back(BlockStmt::build(b, emptyLoc, std::move(block)));
    }
    outer.push_back(Identifier::build(b, emptyLoc, strC));
    auto block = BlockStmt::build(b, emptyLoc, std::move(outer));
    b->addToplevelExpr(std::move(block));
  }

  Builder::Result r = b->result();
  assert(r.topLevelExprs.size() == 1);
  assert(r.errors.size() == 0);
  assert(r.locations.size() == 6);
  BlockStmt* outer = r.topLevelExprs[0]->toBlockStmt();
  assert(outer);
  assert(outer->numStmts() == 4);
  assert(outer->stmt(0)->isIdentifier());
  assert(outer->stmt(1)->isBlockStmt());
  assert(outer->stmt(2)->isBlockStmt());
  assert(outer->stmt(3)->isIdentifier());
  const BlockStmt* empty = outer->stmt(1)->toBlockStmt();
  assert(empty->numStmts() == 0);
  const BlockStmt* block = outer->stmt(2)->toBlockStmt();
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
}

int main(int argc, char** argv) {
  test0();
  test1();
  test2();
  test3();
  return 0;
}

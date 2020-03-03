/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

/*
  This transformation is designed to collapse nested Block Statements e.g.

  {
    var x = 10;

    x = 20;

    {
      var y = 0;

      {
        var z = 20;

        foo(x, y, z);
      }
    }
  }

  can be flattened to

  {
    var x = 10;

    x = 20;

    var y =  0;
    var z = 20;

    foo(x, y, z);
  }

  so long as name resolution has been completed.

  This is achieved by walking the body of a given BlockStmt and

    a) Recursively apply Collapse to each expr in the body

    b) If the expr is a BlockStmt, then copy its body in to the
       outer body and remove the depleted BlockStmt.

  The actual implementation shuffles the input body into a temporary
  Alist and then copies the elements back while applying the transformation.



   NOAKES 2014/11/12

   Currently WhileDo/DoWhile/ForLoop *are* BlockStmts and so those
   cases look a little odd for now.  I am currently working to
   decouple these nodes from BlockStmt.
*/

#include "CollapseBlocks.h"

#include "WhileDoStmt.h"
#include "DoWhileStmt.h"
#include "CForLoop.h"
#include "ForLoop.h"
#include "ParamForLoop.h"
#include "ForallStmt.h"

#include "alist.h"
#include "ImportStmt.h"
#include "stmt.h"

CollapseBlocks::CollapseBlocks()
{

}

CollapseBlocks::~CollapseBlocks()
{

}

bool CollapseBlocks::enterBlockStmt(BlockStmt* node)
{
  AList shuffle;

  // Transfer all of the expressions in to a temporary Alist
  for_alist(expr, node->body)
  {
    expr->remove();

    shuffle.insertAtTail(expr);
  }

  // Copy them back in to the body with recursive collapsing
  for_alist(expr, shuffle)
  {
    BlockStmt* stmt = toBlockStmt(expr);

    // Apply collapse recursively to the expression
    expr->accept(this);

    expr->remove();

    // If the expr is a BlockStmt, collapse during the copy
    if (stmt != 0 && stmt->isLoopStmt() == false && stmt->blockInfoGet() == 0)
    {
      for_alist(subItem, stmt->body)
      {
        subItem->remove();
        node->body.insertAtTail(subItem);
      }
    }
    else
    {
      node->body.insertAtTail(expr);
    }
  }

  removeDeadIterResumeGotos();

  // Do not recurse
  return false;
}

bool CollapseBlocks::enterForallStmt(ForallStmt* node) {
  return enterBlockStmt(node->loopBody());
}


// The c for loop primitive is of the form:
//   __primitive("C for loop", {inits}, {test}, {incrs})
//
// For zippered iterators the inits and incrs can be composed of multiple
// (essentially scopeless) block statements next to each other. We want to
// collapse those into a single block (otherwise codegen would be a nightmare.)
//
bool CollapseBlocks::enterCForLoop(CForLoop* node)
{
  // Handle the init/test/incr fields specially
  if (BlockStmt* init = node->initBlockGet())
    enterBlockStmt(init);

  if (BlockStmt* test = node->testBlockGet())
    enterBlockStmt(test);

  if (BlockStmt* incr = node->incrBlockGet())
    enterBlockStmt(incr);

  // Now simply forward to handle the body
  return enterBlockStmt(node);
}

bool CollapseBlocks::enterForLoop(ForLoop* node)
{
  return enterBlockStmt(node);
}

bool CollapseBlocks::enterParamForLoop(ParamForLoop* node)
{
  return enterBlockStmt(node);
}

// 2014/11/12 Noakes: For now simply act like a BlockStmt
bool CollapseBlocks::enterWhileDoStmt(WhileDoStmt* node)
{
  return enterBlockStmt(node);
}

// 2014/11/12 Noakes: For now simply act like a BlockStmt
bool CollapseBlocks::enterDoWhileStmt(DoWhileStmt* node)
{
  return enterBlockStmt(node);
}

// Recurse into the consequent and alternative
// These are generally BlockStmts
bool CollapseBlocks::enterCondStmt(CondStmt* node)
{
  return true;
}

/************************************ | *************************************
*                                                                           *
* The remaining definitions are simple "default do nothing" definitions      *
*                                                                           *
************************************* | ************************************/

bool CollapseBlocks::enterAggrType(AggregateType* node)
{
  return false;
}

void CollapseBlocks::exitAggrType(AggregateType* node)
{

}

bool CollapseBlocks::enterDecoratedClassType(DecoratedClassType* node)
{
  return false;
}

void CollapseBlocks::exitDecoratedClassType(DecoratedClassType* node)
{

}

bool CollapseBlocks::enterEnumType(EnumType* node)
{
  return false;
}

void CollapseBlocks::exitEnumType(EnumType* node)
{

}

void CollapseBlocks::visitPrimType(PrimitiveType* node)
{

}

bool CollapseBlocks::enterArgSym(ArgSymbol* node)
{
  return false;
}

void CollapseBlocks::exitArgSym(ArgSymbol* node)
{

}

void CollapseBlocks::visitEnumSym(EnumSymbol* node)
{

}

bool CollapseBlocks::enterFnSym(FnSymbol* node)
{
  return false;
}

void CollapseBlocks::exitFnSym(FnSymbol* node)
{

}

void CollapseBlocks::visitLabelSym(LabelSymbol* node)
{

}

bool CollapseBlocks::enterModSym(ModuleSymbol* node)
{
  return false;
}

void CollapseBlocks::exitModSym(ModuleSymbol* node)
{

}

bool CollapseBlocks::enterTypeSym(TypeSymbol* node)
{
  return false;
}

void CollapseBlocks::exitTypeSym(TypeSymbol* node)
{

}

void CollapseBlocks::visitVarSym(VarSymbol* node)
{

}

bool CollapseBlocks::enterCallExpr(CallExpr* node)
{
  return false;
}

void CollapseBlocks::exitCallExpr(CallExpr* node)
{
}

bool CollapseBlocks::enterContextCallExpr(ContextCallExpr* node)
{
  return false;
}

void CollapseBlocks::exitContextCallExpr(ContextCallExpr* node)
{
}

bool CollapseBlocks::enterDefExpr(DefExpr* node)
{
  return false;
}

void CollapseBlocks::exitDefExpr(DefExpr* node)
{

}

bool CollapseBlocks::enterNamedExpr(NamedExpr* node)
{
  return false;
}

void CollapseBlocks::exitNamedExpr(NamedExpr* node)
{

}

bool CollapseBlocks::enterIfExpr(IfExpr* node)
{
  return true;
}

void CollapseBlocks::exitIfExpr(IfExpr* node)
{

}

void CollapseBlocks::visitSymExpr(SymExpr* node)
{

}

void CollapseBlocks::visitUsymExpr(UnresolvedSymExpr* node)
{

}

bool CollapseBlocks::enterLoopExpr(LoopExpr* node)
{
  return true;
}

void CollapseBlocks::exitLoopExpr(LoopExpr* node)
{

}

void CollapseBlocks::visitUseStmt(UseStmt* node)
{

}

void CollapseBlocks::visitImportStmt(ImportStmt* node)
{

}

void CollapseBlocks::exitBlockStmt(BlockStmt* node)
{

}

void CollapseBlocks::exitForallStmt(ForallStmt* node)
{

}

void CollapseBlocks::exitWhileDoStmt(WhileDoStmt* node)
{

}

void CollapseBlocks::exitDoWhileStmt(DoWhileStmt* node)
{

}

void CollapseBlocks::exitCForLoop(CForLoop* node)
{

}

void CollapseBlocks::exitForLoop(ForLoop* node)
{

}

void CollapseBlocks::exitParamForLoop(ParamForLoop* node)
{

}

void CollapseBlocks::exitCondStmt(CondStmt* node)
{

}

void CollapseBlocks::visitEblockStmt(ExternBlockStmt* node)
{

}

bool CollapseBlocks::enterGotoStmt(GotoStmt* node)
{
  return false;
}

void CollapseBlocks::exitGotoStmt(GotoStmt* node)
{

}

bool CollapseBlocks::enterForwardingStmt(ForwardingStmt* node)
{
  return true;
}

void CollapseBlocks::exitForwardingStmt(ForwardingStmt* node)
{

}

bool CollapseBlocks::enterDeferStmt(DeferStmt* node)
{
  // Defer statements really need to be lowered *before*
  // running CollapseBlocks. Otherwise, how can we know
  // what variables or defer blocks are "in scope"?
  INT_ASSERT("Defer statement discovered in CollapseBlocks");
  return true;
}

void CollapseBlocks::exitDeferStmt(DeferStmt* node)
{

}

bool CollapseBlocks::enterTryStmt(TryStmt* node)
{
  return true;
}

void CollapseBlocks::exitTryStmt(TryStmt* node)
{

}

bool CollapseBlocks::enterCatchStmt(CatchStmt* node)
{
  return true;
}

void CollapseBlocks::exitCatchStmt(CatchStmt* node)
{

}

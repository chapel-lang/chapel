/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "AstVisitorTraverse.h"

#include "ImportStmt.h"

AstVisitorTraverse::AstVisitorTraverse()
{

}

AstVisitorTraverse::~AstVisitorTraverse()
{

}

bool AstVisitorTraverse::enterAggrType(AggregateType* node)
{
  return true;
}

void AstVisitorTraverse::exitAggrType(AggregateType* node)
{

}

bool AstVisitorTraverse::enterDecoratedClassType(DecoratedClassType* node)
{
  return true;
}

void AstVisitorTraverse::exitDecoratedClassType(DecoratedClassType* node)
{

}

bool AstVisitorTraverse::enterEnumType(EnumType* node)
{
  return true;
}

void AstVisitorTraverse::exitEnumType(EnumType* node)
{

}

void AstVisitorTraverse::visitPrimType(PrimitiveType* node)
{

}

bool AstVisitorTraverse::enterArgSym(ArgSymbol* node)
{
  return true;
}

void AstVisitorTraverse::exitArgSym(ArgSymbol* node)
{

}

void AstVisitorTraverse::visitEnumSym(EnumSymbol* node)
{

}

bool AstVisitorTraverse::enterFnSym(FnSymbol* node)
{
  return true;
}

void AstVisitorTraverse::exitFnSym(FnSymbol* node)
{

}

void AstVisitorTraverse::visitLabelSym(LabelSymbol* node)
{

}

bool AstVisitorTraverse::enterModSym(ModuleSymbol* node)
{
  return true;
}

void AstVisitorTraverse::exitModSym(ModuleSymbol* node)
{

}

bool AstVisitorTraverse::enterTypeSym(TypeSymbol* node)
{
  return true;
}

void AstVisitorTraverse::exitTypeSym(TypeSymbol* node)
{

}

void AstVisitorTraverse::visitVarSym(VarSymbol* node)
{

}

bool AstVisitorTraverse::enterCallExpr(CallExpr* node)
{
  return true;
}

void AstVisitorTraverse::exitCallExpr(CallExpr* node)
{

}

bool AstVisitorTraverse::enterContextCallExpr(ContextCallExpr* node)
{
  return true;
}

void AstVisitorTraverse::exitContextCallExpr(ContextCallExpr* node)
{

}

bool AstVisitorTraverse::enterDefExpr(DefExpr* node)
{
  return true;
}

void AstVisitorTraverse::exitDefExpr(DefExpr* node)
{
}

bool AstVisitorTraverse::enterNamedExpr(NamedExpr* node)
{
  return true;
}

void AstVisitorTraverse::exitNamedExpr(NamedExpr* node)
{

}

bool AstVisitorTraverse::enterIfExpr(IfExpr* node)
{
  return true;
}

void AstVisitorTraverse::exitIfExpr(IfExpr* node)
{

}

void AstVisitorTraverse::visitSymExpr(SymExpr* node)
{

}

void AstVisitorTraverse::visitUsymExpr(UnresolvedSymExpr* node)
{

}

void AstVisitorTraverse::visitUseStmt(UseStmt* node)
{

}

void AstVisitorTraverse::visitImportStmt(ImportStmt* node)
{

}

bool AstVisitorTraverse::enterLoopExpr(LoopExpr* node)
{
  return true;
}

void AstVisitorTraverse::exitLoopExpr(LoopExpr* node)
{

}

bool AstVisitorTraverse::enterBlockStmt(BlockStmt* node)
{
  return true;
}

void AstVisitorTraverse::exitBlockStmt(BlockStmt* node)
{

}

bool AstVisitorTraverse::enterForallStmt(ForallStmt* node)
{
  return true;
}

void AstVisitorTraverse::exitForallStmt(ForallStmt* node)
{

}

bool AstVisitorTraverse::enterWhileDoStmt(WhileDoStmt* node)
{
  return true;
}

void AstVisitorTraverse::exitWhileDoStmt(WhileDoStmt* node)
{

}

bool AstVisitorTraverse::enterDoWhileStmt(DoWhileStmt* node)
{
  return true;
}

void AstVisitorTraverse::exitDoWhileStmt(DoWhileStmt* node)
{

}

bool AstVisitorTraverse::enterCForLoop(CForLoop* node)
{
  return true;
}

void AstVisitorTraverse::exitCForLoop(CForLoop* node)
{

}

bool AstVisitorTraverse::enterForLoop(ForLoop* node)
{
  return true;
}

void AstVisitorTraverse::exitForLoop(ForLoop* node)
{

}

bool AstVisitorTraverse::enterParamForLoop(ParamForLoop* node)
{
  return true;
}

void AstVisitorTraverse::exitParamForLoop(ParamForLoop* node)
{

}

bool AstVisitorTraverse::enterCondStmt(CondStmt* node)
{
  return true;
}

void AstVisitorTraverse::exitCondStmt(CondStmt* node)
{

}

void AstVisitorTraverse::visitEblockStmt(ExternBlockStmt* node)
{

}

bool AstVisitorTraverse::enterGotoStmt(GotoStmt* node)
{
  return true;
}

void AstVisitorTraverse::exitGotoStmt(GotoStmt* node)
{

}

bool AstVisitorTraverse::enterForwardingStmt(ForwardingStmt* node)
{
  return true;
}

void AstVisitorTraverse::exitForwardingStmt(ForwardingStmt* node)
{

}

bool AstVisitorTraverse::enterDeferStmt(DeferStmt* node)
{
  return true;
}

void AstVisitorTraverse::exitDeferStmt(DeferStmt* node)
{

}

bool AstVisitorTraverse::enterTryStmt(TryStmt* node)
{
  return true;
}

void AstVisitorTraverse::exitTryStmt(TryStmt* node)
{

}

bool AstVisitorTraverse::enterCatchStmt(CatchStmt* node) {
  return true;
}

void AstVisitorTraverse::exitCatchStmt(CatchStmt* node) {

}

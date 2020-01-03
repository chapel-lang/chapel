/*
 * Copyright 2004-2020 Cray Inc.
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

#include "AstCount.h"


AstCount::AstCount() {

// Initialize member variables for each AST node type
// that is handled by the baseAST macro.
#define init_members(type) num##type = 0
  foreach_ast(init_members);
#undef init_members

  // Initialize other member variables for AST types
  // not handled by the baseAST macro.
  numWhileDoStmt = 0;
  numDoWhileStmt = 0;
  numCForLoop = 0;
  numForLoop = 0;
  numParamForLoop = 0;
}

AstCount::~AstCount() {
}

int AstCount::total() {
  int sum = 0;

  // Add those covered by the baseAST macro
#define count_members(type) sum += num##type
foreach_ast(count_members);
#undef count_members

  // and then add those not covered by the baseAST macro
  sum += numWhileDoStmt;
  sum += numDoWhileStmt;
  sum += numCForLoop;
  sum += numForLoop;
  sum += numParamForLoop;

  return sum;
}

bool AstCount::enterDecoratedClassType(DecoratedClassType* node) {
  numDecoratedClassType++;
  return true;
}

void AstCount::exitDecoratedClassType(DecoratedClassType* node) {
}


bool AstCount::enterAggrType(AggregateType* node) {
  numAggregateType++;
  return true;
}

void AstCount::exitAggrType(AggregateType* node) {
}

bool AstCount::enterEnumType(EnumType* node) {
  numEnumType++;
  return true;
}

void AstCount::exitEnumType(EnumType* node) {
}

void AstCount::visitPrimType(PrimitiveType* node) {
  numPrimitiveType++;
}

bool AstCount::enterArgSym(ArgSymbol* node) {
  numArgSymbol++;
  return true;
}

void AstCount::exitArgSym(ArgSymbol* node) {
}

void AstCount::visitEnumSym(EnumSymbol* node) {
  numEnumSymbol++;
}

bool AstCount::enterFnSym(FnSymbol* node) {
  numFnSymbol++;
  return true;
}

void AstCount::exitFnSym(FnSymbol* node) {
}

void AstCount::visitLabelSym(LabelSymbol* node) {
  numLabelSymbol++;
}

bool AstCount::enterModSym(ModuleSymbol* node) {
  numModuleSymbol++;
  return true;
}

void AstCount::exitModSym(ModuleSymbol* node) {
}

bool AstCount::enterTypeSym(TypeSymbol* node) {
  numTypeSymbol++;
  return true;
}

void AstCount::exitTypeSym(TypeSymbol* node) {
}

void AstCount::visitVarSym(VarSymbol* node) {
  numVarSymbol++;
}

bool AstCount::enterCallExpr(CallExpr* node) {
  numCallExpr++;
  return true;
}

void AstCount::exitCallExpr(CallExpr* node) {
}

bool AstCount::enterContextCallExpr(ContextCallExpr* node) {
  numContextCallExpr++;
  return true;
}

void AstCount::exitContextCallExpr(ContextCallExpr* node) {
}

bool AstCount::enterDefExpr(DefExpr* node) {
  numDefExpr++;
  return true;
}

void AstCount::exitDefExpr(DefExpr* node) {
}

bool AstCount::enterNamedExpr(NamedExpr* node) {
  numNamedExpr++;
  return true;
}

void AstCount::exitNamedExpr(NamedExpr* node) {
}

bool AstCount::enterIfExpr(IfExpr* node) {
  numIfExpr++;
  return true;
}

void AstCount::exitIfExpr(IfExpr* node) {
}

void AstCount::visitSymExpr(SymExpr* node) {
  numSymExpr++;
}

void AstCount::visitUsymExpr(UnresolvedSymExpr* node) {
  numUnresolvedSymExpr++;
}

bool AstCount::enterLoopExpr(LoopExpr* node) {
  numLoopExpr++;
  return true;
}

void AstCount::exitLoopExpr(LoopExpr* node) {
}

void AstCount::visitUseStmt(UseStmt* node) {
  numUseStmt++;
}

bool AstCount::enterBlockStmt(BlockStmt* node) {
  numBlockStmt++;
  return true;
}

void AstCount::exitBlockStmt(BlockStmt* node) {
}

bool AstCount::enterForallStmt(ForallStmt* node)
{
  numForallStmt++;
  return true;
}

void AstCount::exitForallStmt(ForallStmt* node) {
}

bool AstCount::enterWhileDoStmt(WhileDoStmt* node) {
  numWhileDoStmt++;
  return true;
}

void AstCount::exitWhileDoStmt(WhileDoStmt* node) {
}

bool AstCount::enterDoWhileStmt(DoWhileStmt* node) {
  numDoWhileStmt++;
  return true;
}

void AstCount::exitDoWhileStmt(DoWhileStmt* node) {
}

bool AstCount::enterCForLoop(CForLoop* node) {
  numCForLoop++;
  return true;
}

void AstCount::exitCForLoop(CForLoop* node) {
}

bool AstCount::enterForLoop(ForLoop* node) {
  numForLoop++;
  return true;
}

void AstCount::exitForLoop(ForLoop* node) {
}

bool AstCount::enterParamForLoop(ParamForLoop* node) {
  numParamForLoop++;
  return true;
}

void AstCount::exitParamForLoop(ParamForLoop* node) {
}

bool AstCount::enterCondStmt(CondStmt* node) {
  numCondStmt++;
  return true;
}

void AstCount::exitCondStmt(CondStmt* node) {
}

void AstCount::visitEblockStmt(ExternBlockStmt* node) {
  numExternBlockStmt++;
}

bool AstCount::enterForwardingStmt(ForwardingStmt* node) {
  numForwardingStmt++;
  return true;
}

void AstCount::exitForwardingStmt(ForwardingStmt* node) {
}

bool AstCount::enterGotoStmt(GotoStmt* node) {
  numGotoStmt++;
  return true;
}

void AstCount::exitGotoStmt(GotoStmt* node) {
}

bool AstCount::enterDeferStmt(DeferStmt* node) {
  numDeferStmt++;
  return true;
}

void AstCount::exitDeferStmt(DeferStmt* node) {
}

bool AstCount::enterTryStmt(TryStmt* node) {
  numTryStmt++;
  return true;
}

void AstCount::exitTryStmt(TryStmt* node) {
}

bool AstCount::enterCatchStmt(CatchStmt* node) {
  numCatchStmt++;
  return true;
}

void AstCount::exitCatchStmt(CatchStmt* node) {
}



/*
 * Copyright 2004-2015 Cray Inc.
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

#include "AstLogger.h"



AstLogger::AstLogger() {

}

AstLogger::~AstLogger() {
}

bool AstLogger::enterAggrType(AggregateType* node) {
  return true;
}

void AstLogger::exitAggrType(AggregateType* node) {
}

bool AstLogger::enterEnumType(EnumType* node) {
  return true;
}

void AstLogger::exitEnumType(EnumType* node) {
}

void AstLogger::visitPrimType(PrimitiveType* node) {
}

bool AstLogger::enterArgSym(ArgSymbol* node) {
  return true;
}

void AstLogger::exitArgSym(ArgSymbol* node) {
}

void AstLogger::visitEnumSym(EnumSymbol* node) {
}

bool AstLogger::enterFnSym(FnSymbol* node) {
  return true;
}

void AstLogger::exitFnSym(FnSymbol* node) {
}

void AstLogger::visitLabelSym(LabelSymbol* node) {
}

bool AstLogger::enterModSym(ModuleSymbol* node) {
  return true;
}

void AstLogger::exitModSym(ModuleSymbol* node) {
}

bool AstLogger::enterTypeSym(TypeSymbol* node) {
  return true;
}

void AstLogger::exitTypeSym(TypeSymbol* node) {
}

void AstLogger::visitVarSym(VarSymbol* node) {
}

bool AstLogger::enterCallExpr(CallExpr* node) {
  return true;
}

void AstLogger::exitCallExpr(CallExpr* node) {
}

bool AstLogger::enterDefExpr(DefExpr* node) {
  return true;
}

void AstLogger::exitDefExpr(DefExpr* node) {
}

bool AstLogger::enterNamedExpr(NamedExpr* node) {
  return true;
}

void AstLogger::exitNamedExpr(NamedExpr* node) {
}

void AstLogger::visitSymExpr(SymExpr* node) {
}

void AstLogger::visitUsymExpr(UnresolvedSymExpr* node) {
}

bool AstLogger::enterBlockStmt(BlockStmt* node) {
  return true;
}

void AstLogger::exitBlockStmt(BlockStmt* node) {
}

bool AstLogger::enterWhileDoStmt(WhileDoStmt* node) {
  return true;
}

void AstLogger::exitWhileDoStmt(WhileDoStmt* node) {
}

bool AstLogger::enterDoWhileStmt(DoWhileStmt* node) {
  return true;
}

void AstLogger::exitDoWhileStmt(DoWhileStmt* node) {
}

bool AstLogger::enterCForLoop(CForLoop* node) {
  return true;
}

void AstLogger::exitCForLoop(CForLoop* node) {
}

bool AstLogger::enterForLoop(ForLoop* node) {
  return true;
}

void AstLogger::exitForLoop(ForLoop* node) {
}

bool AstLogger::enterParamForLoop(ParamForLoop* node) {
  return true;
}

void AstLogger::exitParamForLoop(ParamForLoop* node) {
}

bool AstLogger::enterCondStmt(CondStmt* node) {
  return true;
}

void AstLogger::exitCondStmt(CondStmt* node) {
}

void AstLogger::visitEblockStmt(ExternBlockStmt* node) {
}

bool AstLogger::enterGotoStmt(GotoStmt* node) {
  return true;
}

void AstLogger::exitGotoStmt(GotoStmt* node) {
}

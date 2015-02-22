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

#include <iostream>

#include "AstPrintDocs.h"

#include "symbol.h"
#include "type.h"


AstPrintDocs::AstPrintDocs() {
}


AstPrintDocs::~AstPrintDocs() {
}


bool AstPrintDocs::enterAggrType(AggregateType* node) {
  std::cout << "Entering: " << node->symbol->name << std::endl;
  return true;
}


void AstPrintDocs::exitAggrType(AggregateType* node) {
  std::cout << "Exiting: " << node->symbol->name << std::endl;
}


bool AstPrintDocs::enterEnumType(EnumType* node) {
  return true;
}


void AstPrintDocs::exitEnumType(EnumType* node) {
}


void AstPrintDocs::visitPrimType(PrimitiveType* node) {
}


bool AstPrintDocs::enterArgSym(ArgSymbol* node) {
  return true;
}


void AstPrintDocs::exitArgSym(ArgSymbol* node) {
}


void AstPrintDocs::visitEnumSym(EnumSymbol* node) {
}


bool AstPrintDocs::enterFnSym(FnSymbol* node) {
  return true;
}


void AstPrintDocs::exitFnSym(FnSymbol* node) {
}


void AstPrintDocs::visitIpeSym(IpeSymbol* node) {
}


void AstPrintDocs::visitLabelSym(LabelSymbol* node) {
}


bool AstPrintDocs::enterModSym(ModuleSymbol* node) {
  std::cout << "Entering: " << node->name << std::endl;
  return true;
}


void AstPrintDocs::exitModSym(ModuleSymbol* node) {
  std::cout << "Exiting: " << node->name << std::endl;
}


bool AstPrintDocs::enterTypeSym(TypeSymbol* node) {
  return true;
}


void AstPrintDocs::exitTypeSym(TypeSymbol* node) {
}


void AstPrintDocs::visitVarSym(VarSymbol* node) {
}


bool AstPrintDocs::enterCallExpr(CallExpr* node) {
  return true;
}


void AstPrintDocs::exitCallExpr(CallExpr* node) {
}


bool AstPrintDocs::enterDefExpr(DefExpr* node) {
  return true;
}


void AstPrintDocs::exitDefExpr(DefExpr* node) {
}


bool AstPrintDocs::enterNamedExpr(NamedExpr* node) {
  return true;
}


void AstPrintDocs::exitNamedExpr(NamedExpr* node) {
}


void AstPrintDocs::visitSymExpr(SymExpr* node) {
}


void AstPrintDocs::visitUsymExpr(UnresolvedSymExpr* node) {
}


bool AstPrintDocs::enterBlockStmt(BlockStmt* node) {
  return true;
}


void AstPrintDocs::exitBlockStmt(BlockStmt* node) {
}


bool AstPrintDocs::enterWhileDoStmt(WhileDoStmt* node) {
  return true;
}


void AstPrintDocs::exitWhileDoStmt(WhileDoStmt* node) {
}


bool AstPrintDocs::enterDoWhileStmt(DoWhileStmt* node) {
  return true;
}


void AstPrintDocs::exitDoWhileStmt(DoWhileStmt* node) {
}


bool AstPrintDocs::enterCForLoop(CForLoop* node) {
  return true;
}


void AstPrintDocs::exitCForLoop(CForLoop* node) {
}


bool AstPrintDocs::enterForLoop(ForLoop* node) {
  return true;
}


void AstPrintDocs::exitForLoop(ForLoop* node) {
}


bool AstPrintDocs::enterParamForLoop(ParamForLoop* node) {
  return true;
}


void AstPrintDocs::exitParamForLoop(ParamForLoop* node) {
}


bool AstPrintDocs::enterCondStmt(CondStmt* node) {
  return true;
}


void AstPrintDocs::exitCondStmt(CondStmt* node) {
}


void AstPrintDocs::visitEblockStmt(ExternBlockStmt* node) {
}


bool AstPrintDocs::enterGotoStmt(GotoStmt* node) {
  return true;
}


void AstPrintDocs::exitGotoStmt(GotoStmt* node) {
}

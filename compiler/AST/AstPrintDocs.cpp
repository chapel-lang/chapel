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

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <sys/stat.h>

#include "AstPrintDocs.h"

#include "symbol.h"
#include "type.h"


AstPrintDocs::AstPrintDocs(std::ostream *file) :
  file(file),
  tabs(0)
{
}


AstPrintDocs::~AstPrintDocs() {
}


//
// Methods needed by this visitor.
//


bool AstPrintDocs::enterAggrType(AggregateType* node) {
  node->printDocs(this->file, this->tabs);
  this->tabs++;
  return true;
}


void AstPrintDocs::exitAggrType(AggregateType* node) {
  // TODO: Does it make sense to print inheritance here?
  //       (thomasvandoren, 2015-02-22)
  this->tabs--;
}


bool AstPrintDocs::enterFnSym(FnSymbol* node) {
  node->printDocs(file, this->tabs);
  return false;
}


bool AstPrintDocs::enterModSym(ModuleSymbol* node) {
  // If this is a sub module (i.e. other modules were entered and not yet
  // exited before this one), ensure the docs naming is correct.
  if (!this->moduleNames.empty()) {
    node->addPrefixToName(this->moduleNames.top() + ".");
  }

  node->printDocs(this->file, this->tabs);

  // Record this module's name, so it can be added to any submodules.
  this->moduleNames.push(node->docsName());

  if (fDocsTextOnly) {
    this->tabs++;
  }

  return true;
}


void AstPrintDocs::exitModSym(ModuleSymbol* node) {
  // Remove the current module from stack of names.
  assert(!this->moduleNames.empty());
  this->moduleNames.pop();

  if (fDocsTextOnly) {
    this->tabs++;
  }
}


void AstPrintDocs::visitVarSym(VarSymbol* node) {
  node->printDocs(this->file, this->tabs);
}


//
// Methods *not* actually needed by this visitor, but required to meet the
// AstVisitor interface. Please move above this line, if/when implemented.
//


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


void AstPrintDocs::exitFnSym(FnSymbol* node) {
}


void AstPrintDocs::visitIpeSym(IpeSymbol* node) {
}


void AstPrintDocs::visitLabelSym(LabelSymbol* node) {
}


bool AstPrintDocs::enterTypeSym(TypeSymbol* node) {
  return true;
}


void AstPrintDocs::exitTypeSym(TypeSymbol* node) {
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

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
  // If class/record is not supposed to be documented, do not traverse into it
  // (and skip the documentation).
  if (node->symbol->hasFlag(FLAG_NO_DOC)) {
    return false;
  }

  node->printDocs(this->file, this->tabs);
  this->tabs++;
  return true;
}


void AstPrintDocs::exitAggrType(AggregateType* node) {
  // If class/record is not supposed to be documented, it was not traversed
  // into or documented, so exit early from this method.
  if (node->symbol->hasFlag(FLAG_NO_DOC)) {
    return;
  }

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
    this->tabs--;
  }
}


void AstPrintDocs::visitVarSym(VarSymbol* node) {
  node->printDocs(this->file, this->tabs);
}


bool AstPrintDocs::enterWhileDoStmt(WhileDoStmt* node) {
  return false;
}


bool AstPrintDocs::enterDoWhileStmt(DoWhileStmt* node) {
  return false;
}


bool AstPrintDocs::enterCForLoop(CForLoop* node) {
  return false;
}


bool AstPrintDocs::enterForLoop(ForLoop* node) {
  return false;
}


bool AstPrintDocs::enterParamForLoop(ParamForLoop* node) {
  return false;
}

bool AstPrintDocs::enterCondStmt(CondStmt* node) {
  return false;
}


bool AstPrintDocs::enterGotoStmt(GotoStmt* node) {
  return false;
}

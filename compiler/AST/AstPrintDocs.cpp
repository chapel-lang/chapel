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

#include <cerrno>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

#include "AstPrintDocs.h"

#include "docsDriver.h"
#include "symbol.h"
#include "stringutil.h"
#include "stmt.h"
#include "type.h"


AstPrintDocs::AstPrintDocs(std::string moduleName, std::string path, std::string parentName) :
  file(NULL),
  tabs(0),
  moduleName(moduleName),
  pathWithoutPostfix(""),
  parentName(parentName)
{
  pathWithoutPostfix = path + "/" + moduleName;

  std::string fullname = pathWithoutPostfix;
  if (fDocsTextOnly) {
    fullname = fullname + ".txt";
  } else {
    fullname = fullname + ".rst";
  }
  file = new std::ofstream(fullname.c_str(), std::ios::out);
}


AstPrintDocs::~AstPrintDocs() {
  file->close();
}


//
// Methods needed by this visitor.
//


bool AstPrintDocs::enterAggrType(AggregateType* node) {
  // If class/record is not supposed to be documented, do not traverse into it
  // (and skip the documentation).
  if (node->symbol->noDocGen()) {
    return false;
  }

  node->printDocs(this->file, this->tabs);
  this->tabs++;
  return true;
}


void AstPrintDocs::exitAggrType(AggregateType* node) {
  // If class/record is not supposed to be documented, it was not traversed
  // into or documented, so exit early from this method.
  if (node->symbol->noDocGen()) {
    return;
  }

  // TODO: Does it make sense to print inheritance here?
  //       (thomasvandoren, 2015-02-22)
  this->tabs--;
}


bool AstPrintDocs::enterEnumType(EnumType* node)
{
  node->printDocs(this->file, this->tabs);
  return false;
}


void AstPrintDocs::visitPrimType(PrimitiveType* node) {
  node->printDocs(this->file, this->tabs);
}


bool AstPrintDocs::enterFnSym(FnSymbol* node) {
  node->printDocs(this->file, this->tabs);
  return false;
}


bool AstPrintDocs::enterModSym(ModuleSymbol* node) {
  // If a module is not supposed to be documented, do not traverse into it (and
  // skip the documentation).
  if (node->noDocGen()) {
      return false;
  }

  // If this is a sub module (i.e. other modules were entered and not yet
  // exited before this one), we want to open a new file in a subdirectory.
  if (node->name != this->moduleName) {
    // Create a directory with our module name and store this file in it.
    static const int dirPerms = S_IRWXU | S_IRWXG | S_IRWXO;
    int result = mkdir(this->pathWithoutPostfix.c_str(), dirPerms);
    if (result != 0 && errno != 0 && errno != EEXIST) {
      USR_FATAL(astr("Failed to create directory: ", this->pathWithoutPostfix.c_str(),
                   " due to: ", strerror(errno)));
    }

    std::string parent = "";
    if (this->parentName != "") {
      parent = this->parentName + ".";
    }
    parent = parent + this->moduleName;

    AstPrintDocs *docsVisitor = new AstPrintDocs(node->name,
                                                 this->pathWithoutPostfix,
                                                 parent);
    node->accept(docsVisitor);
    delete docsVisitor;

    return false;
  }

  // Then print our documentation
  node->printDocs(this->file, this->tabs, this->parentName);

  if (fDocsTextOnly) {
    this->tabs++;
  }

  return true;
}


void AstPrintDocs::exitModSym(ModuleSymbol* node) {
  // If module is not supposed to be documented, it was not traversed into or
  // documented, so exit early from this method.
  if (node->noDocGen()) {
    return;
  }

  if (fDocsTextOnly) {
    this->tabs--;
  }
}


void AstPrintDocs::visitVarSym(VarSymbol* node) {
  node->printDocs(this->file, this->tabs);
}


bool AstPrintDocs::enterBlockStmt(BlockStmt* node) {
  // Top level block statements have no parentExpr, only a parentSymbol.
  // Nested block statements have a parentExpr.  We don't want to go into
  // nested block statements, because their contents aren't accessible
  // outside of that scope.
  return node->parentExpr == NULL;
}


bool AstPrintDocs::enterForallStmt(ForallStmt* node) {
  return false;
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

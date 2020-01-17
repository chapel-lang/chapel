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

#ifndef _AST_PRINT_DOCS_H_
#define _AST_PRINT_DOCS_H_

#include <iostream>
#include <stack>
#include <string>

#include "AstVisitorTraverse.h"


class AstPrintDocs : public AstVisitorTraverse {
public:
  AstPrintDocs(std::string moduleName, std::string path, std::string parentName);
                  ~AstPrintDocs();

  virtual bool   enterAggrType    (AggregateType*     node);
  virtual void   exitAggrType     (AggregateType*     node);
  virtual bool   enterEnumType    (EnumType*          node);
  virtual void   visitPrimType    (PrimitiveType*     node);

  virtual bool   enterFnSym       (FnSymbol*          node);
  virtual bool   enterModSym      (ModuleSymbol*      node);
  virtual void   exitModSym       (ModuleSymbol*      node);
  virtual void   visitVarSym      (VarSymbol*         node);

  virtual bool   enterBlockStmt   (BlockStmt*         node);
  virtual bool   enterForallStmt  (ForallStmt*        node);
  virtual bool   enterWhileDoStmt (WhileDoStmt*       node);
  virtual bool   enterDoWhileStmt (DoWhileStmt*       node);
  virtual bool   enterCForLoop    (CForLoop*          node);
  virtual bool   enterForLoop     (ForLoop*           node);
  virtual bool   enterParamForLoop(ParamForLoop*      node);
  virtual bool   enterCondStmt    (CondStmt*          node);
  virtual bool   enterGotoStmt    (GotoStmt*          node);

private:
  std::ofstream*  file;
  unsigned int    tabs;
  std::string     moduleName;
  std::string     pathWithoutPostfix;
  std::string     parentName;
};

#endif

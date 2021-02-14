/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#ifndef _AST_PRINT_DOCS_H_
#define _AST_PRINT_DOCS_H_

#include <iostream>
#include <stack>
#include <string>

#include "AstVisitorTraverse.h"


class AstPrintDocs final : public AstVisitorTraverse {
public:
   AstPrintDocs(std::string moduleName, std::string path, std::string parentName);
  ~AstPrintDocs() override;

  bool   enterAggrType    (AggregateType*     node) override;
  void   exitAggrType     (AggregateType*     node) override;
  bool   enterEnumType    (EnumType*          node) override;
  void   visitPrimType    (PrimitiveType*     node) override;

  bool   enterFnSym       (FnSymbol*          node) override;
  bool   enterInterfaceSym(InterfaceSymbol*   node) override;
  bool   enterModSym      (ModuleSymbol*      node) override;
  void   exitModSym       (ModuleSymbol*      node) override;
  void   visitVarSym      (VarSymbol*         node) override;

  bool   enterBlockStmt   (BlockStmt*         node) override;
  bool   enterForallStmt  (ForallStmt*        node) override;
  bool   enterWhileDoStmt (WhileDoStmt*       node) override;
  bool   enterDoWhileStmt (DoWhileStmt*       node) override;
  bool   enterCForLoop    (CForLoop*          node) override;
  bool   enterForLoop     (ForLoop*           node) override;
  bool   enterParamForLoop(ParamForLoop*      node) override;
  bool   enterCondStmt    (CondStmt*          node) override;
  bool   enterGotoStmt    (GotoStmt*          node) override;

private:
  std::ofstream*  file;
  unsigned int    tabs;
  std::string     moduleName;
  std::string     pathWithoutPostfix;
  std::string     parentName;
};

#endif

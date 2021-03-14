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

#ifndef _AST_DUMP_TO_HTML_H_
#define _AST_DUMP_TO_HTML_H_

#include "AstLogger.h"

#include <cstdio>

class AggregateType;
class BaseAST;
class Expr;
class FnSymbol;
class ModuleSymbol;
class Symbol;

class AstDumpToHtml final : public AstLogger {
public:
  //
  // This is the User interface to the logger.
  //
  // The call to init() is used to set up a master index.html file
  // that will contain links to any per Module per pass files that
  // are generated.
  //
  // A call to view() will dump the AST in every current Module to
  // the log directory and update index.html.
  //
  static  void     init();
  static  void     view(const char* passName);
  static  void     done();

  //
  // These functions are the "implementation" interface for the
  // Visitor pattern.
  //
  // They are logically "internal" to the pattern but must be
  // declared public so that they can be invoked by the AST nodes
  // themselves
  //
  bool     enterCallExpr    (CallExpr*          node) override;
  void     exitCallExpr     (CallExpr*          node) override;

  bool     enterDefExpr     (DefExpr*           node) override;
  void     exitDefExpr      (DefExpr*           node) override;

  bool     enterNamedExpr   (NamedExpr*         node) override;
  void     exitNamedExpr    (NamedExpr*         node) override;

  bool     enterIfExpr      (IfExpr*            node) override;
  void     exitIfExpr       (IfExpr*            node) override;

  void     visitSymExpr     (SymExpr*           node) override;

  void     visitUsymExpr    (UnresolvedSymExpr* node) override;

  bool     enterLoopExpr  (LoopExpr*        node) override;
  void     exitLoopExpr   (LoopExpr*        node) override;

  void     visitUseStmt     (UseStmt*           node) override;

  void     visitImportStmt  (ImportStmt*        node) override;

  bool     enterBlockStmt   (BlockStmt*         node) override;
  void     exitBlockStmt    (BlockStmt*         node) override;

  bool     enterWhileDoStmt (WhileDoStmt*       node) override;
  void     exitWhileDoStmt  (WhileDoStmt*       node) override;

  bool     enterDoWhileStmt (DoWhileStmt*       node) override;
  void     exitDoWhileStmt  (DoWhileStmt*       node) override;

  bool     enterCForLoop    (CForLoop*          node) override;
  void     exitCForLoop     (CForLoop*          node) override;

  bool     enterForLoop     (ForLoop*           node) override;
  void     exitForLoop      (ForLoop*           node) override;

  bool     enterParamForLoop(ParamForLoop*      node) override;
  void     exitParamForLoop (ParamForLoop*      node) override;

  bool     enterCondStmt    (CondStmt*          node) override;
  void     exitCondStmt     (CondStmt*          node) override;

  void     visitEblockStmt  (ExternBlockStmt*   node) override;

  bool     enterGotoStmt    (GotoStmt*          node) override;
  void     exitGotoStmt     (GotoStmt*          node) override;

private:
                   AstDumpToHtml();
                  ~AstDumpToHtml() override;

  bool             open(ModuleSymbol* module, const char* passName);
  bool             close();

  void             writeFnSymbol(FnSymbol* fn);
  void             writeSymbol(Symbol* sym, bool def);

  const char*      html_file_name(int passNum, Symbol*     sym);
  const char*      html_file_name(int passNum, const char* module);
  void             adjacent_passes(Symbol* sym);
  void             printBlockID(Expr* expr);

  bool             hasHref(Symbol* sym);

  static int       sPassIndex;
  static  FILE*    sIndexFP;

  FILE*            mFP;             // The FILE* to the log file if the file is open
};

#endif

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

class AstDumpToHtml : public AstLogger {
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
  virtual bool     enterCallExpr    (CallExpr*          node);
  virtual void     exitCallExpr     (CallExpr*          node);

  virtual bool     enterDefExpr     (DefExpr*           node);
  virtual void     exitDefExpr      (DefExpr*           node);

  virtual bool     enterNamedExpr   (NamedExpr*         node);
  virtual void     exitNamedExpr    (NamedExpr*         node);

  virtual bool     enterIfExpr      (IfExpr*            node);
  virtual void     exitIfExpr       (IfExpr*            node);

  virtual void     visitSymExpr     (SymExpr*           node);

  virtual void     visitUsymExpr    (UnresolvedSymExpr* node);

  virtual bool     enterLoopExpr  (LoopExpr*        node);
  virtual void     exitLoopExpr   (LoopExpr*        node);

  virtual void     visitUseStmt     (UseStmt*           node);

  virtual bool     enterBlockStmt   (BlockStmt*         node);
  virtual void     exitBlockStmt    (BlockStmt*         node);

  virtual bool     enterWhileDoStmt (WhileDoStmt*       node);
  virtual void     exitWhileDoStmt  (WhileDoStmt*       node);

  virtual bool     enterDoWhileStmt (DoWhileStmt*       node);
  virtual void     exitDoWhileStmt  (DoWhileStmt*       node);

  virtual bool     enterCForLoop    (CForLoop*          node);
  virtual void     exitCForLoop     (CForLoop*          node);

  virtual bool     enterForLoop     (ForLoop*           node);
  virtual void     exitForLoop      (ForLoop*           node);

  virtual bool     enterParamForLoop(ParamForLoop*      node);
  virtual void     exitParamForLoop (ParamForLoop*      node);

  virtual bool     enterCondStmt    (CondStmt*          node);
  virtual void     exitCondStmt     (CondStmt*          node);

  virtual void     visitEblockStmt  (ExternBlockStmt*   node);

  virtual bool     enterGotoStmt    (GotoStmt*          node);
  virtual void     exitGotoStmt     (GotoStmt*          node);

private:
                   AstDumpToHtml();
                  ~AstDumpToHtml();

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

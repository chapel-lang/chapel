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

#ifndef _AST_DUMP_H_
#define _AST_DUMP_H_

#include "AstLogger.h"

#include <cstdio>

class AggregateType;
class BaseAST;
class Expr;
class FnSymbol;
class LoopStmt;
class ModuleSymbol;
class Symbol;

class AstDump final : public AstLogger {
public:
  //
  // This is the User interface to the logger.
  //
  // This will dump a view of the AST nodes in Module to a file.
  // There are command line options to control where the log files
  // are written.  The name of each file includes the name of the
  // Module, the number for the pass, and the name for the pass.
  //
  static  void     view(const char* passName, int passNum);

                   AstDump(FILE* fp);
                  ~AstDump() override;

  //
  // These functions are the "implementation" interface for the
  // Visitor pattern.
  //
  // They are logically "internal" to the pattern but must be
  // declared public so that they can be invoked by the AST nodes
  // themselves
  //
  bool     enterArgSym      (ArgSymbol*         node) override;

  bool     enterCallExpr    (CallExpr*          node) override;
  void     exitCallExpr     (CallExpr*          node) override;

  bool     enterDefExpr     (DefExpr*           node) override;

  bool     enterNamedExpr   (NamedExpr*         node) override;
  void     exitNamedExpr    (NamedExpr*         node) override;

  bool     enterIfExpr      (IfExpr*            node) override;
  void     exitIfExpr       (IfExpr*            node) override;

  void     visitSymExpr     (SymExpr*           node) override;

  void     visitUsymExpr    (UnresolvedSymExpr* node) override;

  bool     enterLoopExpr    (LoopExpr*        node) override;
  void     exitLoopExpr     (LoopExpr*        node) override;

  void     visitUseStmt     (UseStmt*           node) override;

  void     visitImportStmt  (ImportStmt*        node) override;

  bool     enterBlockStmt   (BlockStmt*         node) override;
  void     exitBlockStmt    (BlockStmt*         node) override;

  bool     enterForallStmt  (ForallStmt*        node) override;
  void     exitForallStmt   (ForallStmt*        node) override;

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

  void     visitEblockStmt  (ExternBlockStmt*   node) override;

  bool     enterGotoStmt    (GotoStmt*          node) override;

  bool     enterForwardingStmt (ForwardingStmt*     node) override;
  void     exitForwardingStmt  (ForwardingStmt*     node) override;

  bool     enterDeferStmt   (DeferStmt*         node) override;
  void     exitDeferStmt    (DeferStmt*         node) override;

  bool     enterTryStmt     (TryStmt*           node) override;
  void     exitTryStmt      (TryStmt*           node) override;

  bool     enterCatchStmt   (CatchStmt*         node) override;
  void     exitCatchStmt    (CatchStmt*         node) override;

private:
                   AstDump();

  bool             open(const ModuleSymbol* module, const char* passName, int passNum);
  bool             close();

  void             writeFnSymbol(FnSymbol* fn);

  void             writeSymbol(const char* tag, Symbol* sym, bool def);
  void             writeSymbol(Symbol* sym, bool def);

  // This is equivalent to write(true, text, true)
  void             write(const char* text);

  // This prints text with some help to manage leading/trailing whitespace
  void             write(bool spaceBefore, const char* text, bool spaceAfter);

  void             printBlockID(Expr* expr);
  void             printLoopStmtDetails(LoopStmt* loop);
  void             newline();

  const char*      mName;           // The name of the file for the log
  const char*      mPath;           // The full path to the file for the log
  FILE*            mFP;             // The FILE* to the log file if the file is open
  int              mIndent;         // The indentation level.  Increments for each BlockStmt
  bool             mNeedSpace;      // Control inter-token spaces
  bool             mDontCloseFP;    // true if mFP is owned by the user
};

#endif

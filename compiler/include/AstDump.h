/*
 * Copyright 2004-2020 Cray Inc.
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

class AstDump : public AstLogger {
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
                  ~AstDump();

  //
  // These functions are the "implementation" interface for the
  // Visitor pattern.
  //
  // They are logically "internal" to the pattern but must be
  // declared public so that they can be invoked by the AST nodes
  // themselves
  //
  virtual bool     enterArgSym      (ArgSymbol*         node);

  virtual bool     enterCallExpr    (CallExpr*          node);
  virtual void     exitCallExpr     (CallExpr*          node);

  virtual bool     enterDefExpr     (DefExpr*           node);

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

  virtual bool     enterForallStmt  (ForallStmt*        node);
  virtual void     exitForallStmt   (ForallStmt*        node);

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

  virtual void     visitEblockStmt  (ExternBlockStmt*   node);

  virtual bool     enterGotoStmt    (GotoStmt*          node);

  virtual bool     enterForwardingStmt (ForwardingStmt*     node);
  virtual void     exitForwardingStmt  (ForwardingStmt*     node);

  virtual bool     enterDeferStmt   (DeferStmt*         node);
  virtual void     exitDeferStmt    (DeferStmt*         node);

  virtual bool     enterTryStmt     (TryStmt*           node);
  virtual void     exitTryStmt      (TryStmt*           node);

  virtual bool     enterCatchStmt   (CatchStmt*         node);
  virtual void     exitCatchStmt    (CatchStmt*         node);

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

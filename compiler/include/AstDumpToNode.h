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

#ifndef _AST_DUMP_TO_NODE_H_
#define _AST_DUMP_TO_NODE_H_

#include "AstLogger.h"

#include "alist.h"
#include <cstdio>

class BaseAST;
class Expr;
class ModuleSymbol;
class QualifiedType;
class Symbol;
class Type;


class AstDumpToNode : public AstLogger
{
  //
  // Static interface
  //
public:
  //
  // This is the User interface to the logger.
  //
  // This will dump a view of the AST nodes in Module to a file.
  // There are command line options to control where the log files
  // are written.  The name of each file includes the name of the
  // Module, the number for the pass, and the name for the pass.
  //
  static  void         view(const char*   passName,
                            int           passNum);

  static  void         view(const char*   passName,
                            int           passNum,
                            ModuleSymbol* module);

  // These adjust the printing format. Could be per-instance.
  static bool          compact;
  static const char*   delimitEnter;
  static const char*   delimitExit;
  static bool          showNodeIDs;



  //
  // Instance interface
  //
public:
                   AstDumpToNode(FILE* fp, int offset = 0);
  virtual         ~AstDumpToNode();

  void             offsetSet(int offset);

  //
  // These functions are the "implementation" interface for the
  // Visitor pattern.
  //
  // They are logically "internal" to the pattern but must be
  // declared public so that they can be invoked by the AST nodes
  // themselves
  //

  virtual bool     enterAggrType       (AggregateType*     node);
  virtual void     exitAggrType        (AggregateType*     node);

  virtual bool     enterEnumType       (EnumType*          node);

  virtual void     visitPrimType       (PrimitiveType*     node);

  virtual bool     enterArgSym         (ArgSymbol*         node);

  virtual void     visitEnumSym        (EnumSymbol*        node);

  virtual bool     enterFnSym          (FnSymbol*          node);

  virtual void     visitLabelSym       (LabelSymbol*       node);

  virtual bool     enterModSym         (ModuleSymbol*      node);
  virtual void     exitModSym          (ModuleSymbol*      node);

  virtual bool     enterTypeSym        (TypeSymbol*        node);

  virtual void     visitVarSym         (VarSymbol*         node);

  virtual bool     enterCallExpr       (CallExpr*          node);

  virtual bool     enterContextCallExpr(ContextCallExpr*   node);

  virtual bool     enterDefExpr        (DefExpr*           node);

  virtual bool     enterNamedExpr      (NamedExpr*         node);
  virtual void     exitNamedExpr       (NamedExpr*         node);

  virtual bool     enterIfExpr         (IfExpr*            node);
  virtual void     exitIfExpr          (IfExpr*            node);

  virtual void     visitSymExpr        (SymExpr*           node);

  virtual void     visitUsymExpr       (UnresolvedSymExpr* node);

  virtual bool     enterLoopExpr  (LoopExpr*        node);
  virtual void     exitLoopExpr   (LoopExpr*        node);

  virtual void     visitUseStmt        (UseStmt*           node);

  virtual bool     enterBlockStmt      (BlockStmt*         node);

  virtual bool     enterForallStmt     (ForallStmt*        node);

  virtual bool     enterWhileDoStmt    (WhileDoStmt*       node);

  virtual bool     enterDoWhileStmt    (DoWhileStmt*       node);

  virtual bool     enterCForLoop       (CForLoop*          node);

  virtual bool     enterForLoop        (ForLoop*           node);

  virtual bool     enterParamForLoop   (ParamForLoop*      node);

  virtual bool     enterCondStmt       (CondStmt*          node);

  virtual void     visitEblockStmt     (ExternBlockStmt*   node);

  virtual bool     enterGotoStmt       (GotoStmt*          node);

  virtual bool     enterTryStmt        (TryStmt*           node);

  virtual bool     enterCatchStmt      (CatchStmt*         node);

  virtual bool     enterDeferStmt      (DeferStmt*         node);

private:
                   AstDumpToNode();

  bool             open(ModuleSymbol* mod, const char* passName, int passNum);
  bool             close();

  void             ast_symbol(const char* tag, Symbol* sym, bool def);
  void             ast_symbol(Symbol* sym, bool def);

  void             writeSymbol(Symbol* sym)                              const;
  void             writeSymbolCompact(Symbol* sym)                       const;

  int              writeQual(QualifiedType type)                         const;
  int              writeType(Type*         type, bool announce = true)   const;

  void             write(const char* text);
  void             write(bool spaceBefore, const char* text, bool spaceAfter);

  void             newline();

  // enable compact mode
  void             enterNode(BaseAST* node)                             const;
  void             enterNodeSym(Symbol* node, const char* name = 0)     const;
  void             exitNode(BaseAST* node, bool addNewline = false)     const;
  void             writeField(const char* msg, int offset, BaseAST* field);
  void             writeField(const char* msg, AList& list);
  void             writeLongString(const char* msg, const char* arg)    const;
  void             writeNodeID(BaseAST* node,
                               bool     spaceBefore,
                               bool     spaceAfter)                     const;

  const char*      mPath;
  FILE*            mFP;

  int              mOffset;
  bool             mNeedSpace;

  ModuleSymbol*    mModule;
};


#endif

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


class AstDumpToNode final : public AstLogger
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
  ~AstDumpToNode() override;

  void offsetSet(int offset);

  //
  // These functions are the "implementation" interface for the
  // Visitor pattern.
  //
  // They are logically "internal" to the pattern but must be
  // declared public so that they can be invoked by the AST nodes
  // themselves
  //

  bool   enterAggrType       (AggregateType*     node) override;
  void   exitAggrType        (AggregateType*     node) override;

  bool   enterEnumType       (EnumType*          node) override;
  void   visitConstrainedType(ConstrainedType*   node) override;
  void   visitPrimType       (PrimitiveType*     node) override;

  bool   enterArgSym         (ArgSymbol*         node) override;

  void   visitEnumSym        (EnumSymbol*        node) override;

  bool   enterFnSym          (FnSymbol*          node) override;
  bool   enterInterfaceSym   (InterfaceSymbol*   node) override;

  void   visitLabelSym       (LabelSymbol*       node) override;

  bool   enterModSym         (ModuleSymbol*      node) override;
  void   exitModSym          (ModuleSymbol*      node) override;

  bool   enterTypeSym        (TypeSymbol*        node) override;

  void   visitVarSym         (VarSymbol*         node) override;

  bool   enterCallExpr       (CallExpr*          node) override;

  bool   enterContextCallExpr(ContextCallExpr*   node) override;

  bool   enterDefExpr        (DefExpr*           node) override;

  bool   enterNamedExpr      (NamedExpr*         node) override;
  void   exitNamedExpr       (NamedExpr*         node) override;

  bool   enterIfcConstraint  (IfcConstraint*     node) override;

  bool   enterIfExpr         (IfExpr*            node) override;
  void   exitIfExpr          (IfExpr*            node) override;

  void   visitSymExpr        (SymExpr*           node) override;

  void   visitUsymExpr       (UnresolvedSymExpr* node) override;

  bool   enterLoopExpr       (LoopExpr*          node) override;
  void   exitLoopExpr        (LoopExpr*          node) override;

  void   visitUseStmt        (UseStmt*           node) override;

  void   visitImportStmt     (ImportStmt*        node) override;

  bool   enterBlockStmt      (BlockStmt*         node) override;

  bool   enterForallStmt     (ForallStmt*        node) override;

  bool   enterWhileDoStmt    (WhileDoStmt*       node) override;

  bool   enterDoWhileStmt    (DoWhileStmt*       node) override;

  bool   enterCForLoop       (CForLoop*          node) override;

  bool   enterForLoop        (ForLoop*           node) override;

  bool   enterParamForLoop   (ParamForLoop*      node) override;

  bool   enterCondStmt       (CondStmt*          node) override;

  void   visitEblockStmt     (ExternBlockStmt*   node) override;

  bool   enterGotoStmt       (GotoStmt*          node) override;

  bool   enterTryStmt        (TryStmt*           node) override;

  bool   enterCatchStmt      (CatchStmt*         node) override;

  bool   enterDeferStmt      (DeferStmt*         node) override;

  bool   enterImplementsStmt (ImplementsStmt*    node) override;

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

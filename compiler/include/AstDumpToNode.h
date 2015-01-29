#ifndef _AST_DUMP_TO_NODE_H_
#define _AST_DUMP_TO_NODE_H_

#include "AstLogger.h"

#include <cstdio>

class BaseAST;
class Expr;
class ModuleSymbol;
class Symbol;
class Type;

class AstDumpToNode : public AstLogger
{
public:
  //
  // This is the User interface to the logger.
  //
  // This will dump a view of the AST nodes in Module to a file.
  // There are command line options to control where the log files
  // are written.  The name of each file includes the name of the
  // Module, the number for the pass, and the name for the pass.
  //
  static  void     view(const char*   passName,
                        int           passNum);

  static  void     view(const char*   passName,
                        int           passNum,
                        ModuleSymbol* module);

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

  virtual bool     enterAggrType    (AggregateType*     node);
  virtual void     exitAggrType     (AggregateType*     node);

  virtual bool     enterEnumType    (EnumType*          node);
  virtual void     exitEnumType     (EnumType*          node);

  virtual void     visitPrimType    (PrimitiveType*     node);

  virtual bool     enterArgSym      (ArgSymbol*         node);

  virtual void     visitEnumSym     (EnumSymbol*        node);

  virtual bool     enterFnSym       (FnSymbol*          node);

  virtual void     visitIpeSym      (IpeSymbol*         node);

  virtual void     visitLabelSym    (LabelSymbol*       node);

  virtual bool     enterModSym      (ModuleSymbol*      node);
  virtual void     exitModSym       (ModuleSymbol*      node);

  virtual bool     enterTypeSym     (TypeSymbol*        node);

  virtual void     visitVarSym      (VarSymbol*         node);

  virtual bool     enterCallExpr    (CallExpr*          node);

  virtual bool     enterDefExpr     (DefExpr*           node);

  virtual bool     enterNamedExpr   (NamedExpr*         node);
  virtual void     exitNamedExpr    (NamedExpr*         node);

  virtual void     visitSymExpr     (SymExpr*           node);

  virtual void     visitUsymExpr    (UnresolvedSymExpr* node);

  virtual bool     enterBlockStmt   (BlockStmt*         node);

  virtual bool     enterWhileDoStmt (WhileDoStmt*       node);

  virtual bool     enterDoWhileStmt (DoWhileStmt*       node);

  virtual bool     enterCForLoop    (CForLoop*          node);

  virtual bool     enterForLoop     (ForLoop*           node);

  virtual bool     enterParamForLoop(ParamForLoop*      node);

  virtual bool     enterCondStmt    (CondStmt*          node);

  virtual void     visitEblockStmt  (ExternBlockStmt*   node);

  virtual bool     enterGotoStmt    (GotoStmt*          node);
  virtual void     exitGotoStmt     (GotoStmt*          node);

private:
                   AstDumpToNode();

  bool             open(ModuleSymbol* mod, const char* passName, int passNum);
  bool             close();

  void             ast_symbol(const char* tag, Symbol* sym, bool def);
  void             ast_symbol(Symbol* sym, bool def);

  void             writeSymbol(Symbol* sym)                             const;
  void             writeType  (Type*   type)                            const;

  void             write(const char* text);
  void             write(bool spaceBefore, const char* text, bool spaceAfter);

  void             newline();

  void             logEnter(BaseAST* node);
  void             logEnter(BaseAST* node, const char* fmt, ...);
  void             logEnter(const char* fmt, ...);

  void             logExit (BaseAST* node);
  void             logExit (const char* fmt, ...);

  void             logVisit(BaseAST* node);
  void             logVisit(const char* fmt, ...);

  void             logWrite(const char* fmt, ...);

  const char*      mPath;
  FILE*            mFP;

  int              mOffset;
  bool             mNeedSpace;

  ModuleSymbol*    mModule;
};


#endif

#ifndef _AST_DUMP_H_
#define _AST_DUMP_H_

#include "AstLogger.h"

#include <cstdio>

class AggregateType;
class BaseAST;
class Expr;
class FnSymbol;
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


  //
  // These functions are the "implementation" interface for the
  // Visitor pattern.
  //
  // They are logically "internal" to the pattern but must be
  // declared public so that they can be invoked by the AST nodes
  // themselves
  //
  virtual bool     visitEnter(ArgSymbol*         node);

  virtual bool     visitEnter(CallExpr*          node);
  virtual void     visitExit (CallExpr*          node);

  virtual bool     visitEnter(DefExpr*           node);

  virtual bool     visitEnter(NamedExpr*         node);
  virtual void     visitExit (NamedExpr*         node);

  virtual void     visit     (SymExpr*           node);

  virtual void     visit     (UnresolvedSymExpr* node);

  virtual bool     visitEnter(BlockStmt*         node);
  virtual void     visitExit (BlockStmt*         node);

  virtual bool     visitEnter(CondStmt*          node);

  virtual void     visit     (ExternBlockStmt*   node);

  virtual bool     visitEnter(GotoStmt*          node);

private:
                   AstDump();
                  ~AstDump();

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
  void             newline();

  const char*      mName;           // The name of the file for the log
  const char*      mPath;           // The full path to the file for the log
  FILE*            mFP;             // The FILE* to the log file if the file is open
  int              mIndent;         // The indentation level.  Increments for each BlockStmt
  bool             mNeedSpace;      // Control inter-token spaces
};

#endif

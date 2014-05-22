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
  virtual bool     visitEnter(CallExpr*          node);
  virtual void     visitExit (CallExpr*          node);

  virtual bool     visitEnter(DefExpr*           node);
  virtual void     visitExit (DefExpr*           node);

  virtual bool     visitEnter(NamedExpr*         node);
  virtual void     visitExit (NamedExpr*         node);

  virtual void     visit     (SymExpr*           node);

  virtual void     visit     (UnresolvedSymExpr* node);

  virtual bool     visitEnter(BlockStmt*         node);
  virtual void     visitExit (BlockStmt*         node);

  virtual bool     visitEnter(CondStmt*          node);
  virtual void     visitExit (CondStmt*          node);

  virtual void     visit     (ExternBlockStmt*   node);

  virtual bool     visitEnter(GotoStmt*          node);
  virtual void     visitExit (GotoStmt*          node);

private:
                   AstDumpToHtml();
                  ~AstDumpToHtml();

  bool             open(ModuleSymbol* module, const char* passName, int passNum);
  bool             close();
  
  void             writeFnSymbol(FnSymbol* fn);
  void             writeSymbol(Symbol* sym, bool def);

  const char*      html_file_name(int passNum, Symbol*     sym);
  const char*      html_file_name(int passNum, const char* module);
  void             adjacent_passes(Symbol* sym);
  void             printBlockID(Expr* expr);

  bool             hasHref(Symbol* sym);

  static int       sPassIndex;

  int              mPassNum;        // The pass that generated this AST

  const char*      mName;           // The name of the file for the log
  const char*      mPath;           // The full path to the file for the log
  FILE*            mFP;             // The FILE* to the log file if the file is open
};

#endif

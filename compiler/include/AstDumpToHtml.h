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
  virtual bool     enterCallExpr  (CallExpr*          node);
  virtual void     exitCallExpr   (CallExpr*          node);

  virtual bool     enterDefExpr   (DefExpr*           node);
  virtual void     exitDefExpr    (DefExpr*           node);

  virtual bool     enterNamedExpr (NamedExpr*         node);
  virtual void     exitNamedExpr  (NamedExpr*         node);

  virtual void     visitSymExpr   (SymExpr*           node);

  virtual void     visitUsymExpr  (UnresolvedSymExpr* node);

  virtual bool     enterBlockStmt (BlockStmt*         node);
  virtual void     exitBlockStmt  (BlockStmt*         node);

  virtual bool     enterCondStmt  (CondStmt*          node);
  virtual void     exitCondStmt   (CondStmt*          node);

  virtual void     visitEblockStmt(ExternBlockStmt*   node);

  virtual bool     enterGotoStmt  (GotoStmt*          node);
  virtual void     exitGotoStmt   (GotoStmt*          node);

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

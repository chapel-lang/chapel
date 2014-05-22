#ifndef _AST_DUMP_H_
#define _AST_DUMP_H_

#include <cstdio>

class AggregateType;
class BaseAST;
class Expr;
class FnSymbol;
class ModuleSymbol;
class Symbol;

class AstDump {
public:
  static void      view(const char* passName, int passNum);

private:
                   AstDump();
                  ~AstDump();

  bool             open(const ModuleSymbol* module, const char* passName, int passNum);
  void             write(BaseAST* ast);
  bool             close();

  bool             header(BaseAST* ast);
  void             footer(BaseAST* ast);

  void             writeFnSymbol(FnSymbol* fn);

  void             writeSymbol(const char* tag, Symbol* sym, bool def);
  void             writeSymbol(Symbol* sym, bool def);

  void             write(const char* text);
  void             write(bool spaceBefore, const char* text, bool spaceAfter);

  void             printBlockID(Expr* expr);
  void             newline();

  const char*      mName;           // The name of the file for the log
  const char*      mPath;           // The full path to the file for the log
  FILE*            mFP;             // The FILE* to the log file if the file is open
  int              mIndent;         // The indentation level.  Increments for each BlockStmt
  bool             mNeedSpace;      // Control inter-token spaces


  // Temp for call AST_CHILDREN macro
  static void      tempHack(BaseAST* ast, AstDump* handle);
};

#endif

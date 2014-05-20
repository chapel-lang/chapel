#ifndef _AST_DUMP_TO_HTML_H_
#define _AST_DUMP_TO_HTML_H_

#include <cstdio>

class AggregateType;
class BaseAST;
class Expr;
class FnSymbol;
class ModuleSymbol;
class Symbol;

class AstDumpToHtml {
public:
  static void      init();
  static void      view(const char* passName);
  static void      done();

private:
                   AstDumpToHtml();
                  ~AstDumpToHtml();

  bool             open(ModuleSymbol* module, const char* passName, int passNum);
  void             write(BaseAST* ast);
  bool             close();
  
  void             writeFnSymbol(FnSymbol* fn);
  void             writeSymbol(Symbol* sym, bool def);

  const char*      html_file_name(int passNum, Symbol*     sym);
  const char*      html_file_name(int passNum, const char* module);
  void             adjacent_passes(Symbol* sym);
  void             printBlockID(Expr* expr);

  AggregateType*   structuralTypeSymbol(Symbol* s);
  bool             hasHref(Symbol* sym);

  static int       sPassIndex;

  int              mPassNum;        // The pass that generated this AST

  const char*      mName;           // The name of the file for the log
  const char*      mPath;           // The full path to the file for the log
  FILE*            mFP;             // The FILE* to the log file if the file is open

  // Temp for call AST_CHILDREN macro
  static void      tempHack(BaseAST* ast, AstDumpToHtml* handle);
};

#endif

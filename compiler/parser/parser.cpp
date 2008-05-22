#include <stdlib.h>
#include "build.h"
#include "countTokens.h"
#include "files.h"
#include "parser.h"
#include "stringutil.h"
#include "symbol.h"
#include "chapel.tab.h"
#include "yy.h"

BlockStmt* yyblock = NULL;
const char* yyfilename;
int chplLineno;
int yystartlineno;

static const char* filenameToModulename(const char* filename) {
  const char* modulename = astr(filename);
  char* lastslash = strrchr(modulename, '/');
  if (lastslash) {
    modulename = lastslash+1;
  }
  const char* dot = strchr(modulename, '.');
  return asubstr(modulename, dot);
}

static bool
containsOnlyModules(BlockStmt* block) {
  for_alist(stmt, block->body) {
    bool isModuleDef = false;
    if (BlockStmt* block = toBlockStmt(stmt))
      stmt = block->body.first();
    if (DefExpr* defExpr = toDefExpr(stmt))
      if (toModuleSymbol(defExpr->sym))
        isModuleDef = true;
    if (!isModuleDef)
      return false;
  }
  return true;
}


ModuleSymbol* ParseFile(const char* filename, ModTag moduletype) {
  ModuleSymbol* newModule = NULL;
  yyfilename = filename;
  yylloc.first_column = yylloc.last_column = yylloc.first_line = yylloc.last_line = yystartlineno = chplLineno = 0;

  yylloc.first_column = yylloc.last_column = 0;
  yylloc.first_line = yylloc.last_line = yystartlineno = chplLineno = 1;
  yyin = openInputFile(filename);
  
  yyblock = NULL;
  if (moduletype == MOD_USER) {
    startCountingFileTokens(filename);
  }
  yyparse();
  if (moduletype == MOD_USER) {
    stopCountingFileTokens();
  }

  closeInputFile(yyin);

  if (!yyblock->body.head || !containsOnlyModules(yyblock)) {
    const char* modulename = filenameToModulename(filename);
    newModule = buildModule(modulename, moduletype, yyblock);
  }
  if (newModule) {
    theProgram->block->insertAtTail(new DefExpr(newModule));
  } else {
    for_alist(stmt, yyblock->body) {
      if (BlockStmt* block = toBlockStmt(stmt))
        stmt = block->body.first();
      if (DefExpr* defExpr = toDefExpr(stmt))
        if (toModuleSymbol(defExpr->sym)) {
          theProgram->block->insertAtTail(defExpr->remove());
        }
    }
  }
  yyfilename = "<internal>";

  yylloc.first_column = yylloc.last_column = 0;
  yylloc.first_line = yylloc.last_line = yystartlineno = chplLineno = -1;

  return newModule;
}

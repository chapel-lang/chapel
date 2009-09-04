#include <cstdlib>
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
ModTag moduleType;


static Vec<const char*> modNameSet;
static Vec<const char*> modNameList;
static Vec<const char*> modDoneSet;


void addModuleToParseList(const char* name) {
  const char* modName = astr(name);
  if (modDoneSet.set_in(modName) || modNameSet.set_in(modName)) {
    //    printf("We've already seen %s\n", modName);
  } else {
    //    printf("Need to parse %s\n", modName);
    modNameSet.set_add(modName);
    modNameList.add(modName);
  }
}

static void addModuleToDoneList(const char* name) {
  const char* uniqueName = astr(name);
  modDoneSet.set_add(uniqueName);
}


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


static bool firstFile = true;

ModuleSymbol* ParseFile(const char* filename, ModTag modType) {
  ModuleSymbol* newModule = NULL;
  moduleType = modType;
  yyfilename = filename;

  yylloc.first_column = yylloc.last_column = 0;
  yylloc.first_line = yylloc.last_line = yystartlineno = chplLineno = 1;
  yyin = openInputFile(filename);

  if (printModuleFiles && modType != MOD_INTERNAL) {
    if (firstFile) {
      fprintf(stderr, "Parsing module files:\n");
      firstFile = false;
    }
    fprintf(stderr, "  %s\n", filename);
  }
  
  yyblock = NULL;
  if (modType == MOD_MAIN) {
    startCountingFileTokens(filename);
  }
  yyparse();
  if (modType == MOD_MAIN) {
    stopCountingFileTokens();
  }

  closeInputFile(yyin);

  if (!yyblock->body.head || !containsOnlyModules(yyblock)) {
    const char* modulename = filenameToModulename(filename);
    newModule = buildModule(modulename, yyblock, yyfilename);
  }
  if (newModule) {
    theProgram->block->insertAtTail(new DefExpr(newModule));
    addModuleToDoneList(newModule->name);
  } else {
    for_alist(stmt, yyblock->body) {
      if (BlockStmt* block = toBlockStmt(stmt))
        stmt = block->body.first();
      if (DefExpr* defExpr = toDefExpr(stmt)) {
        ModuleSymbol* modsym = toModuleSymbol(defExpr->sym);
        if (modsym) {
          addModuleToDoneList(modsym->name);
          theProgram->block->insertAtTail(defExpr->remove());
        }
      }
    }
  }
  yyfilename = "<internal>";

  yylloc.first_column = yylloc.last_column = 0;
  yylloc.first_line = yylloc.last_line = yystartlineno = chplLineno = -1;

  return newModule;
}


ModuleSymbol* ParseMod(const char* modname, ModTag modType) {
  bool internal = modType == MOD_INTERNAL;
  bool isStandard;

  const char* filename = modNameToFilename(modname, internal, &isStandard);
  if (filename == NULL) {
    return NULL;
  } else {
    if (!internal && isStandard) {
      modType = MOD_STANDARD;
    }
    return ParseFile(filename, modType);
  }
}


void parseDependentModules(ModTag modtype) {
  forv_Vec(const char*, modName, modNameList) {
    if (!modDoneSet.set_in(modName)) {
      if (ParseMod(modName, modtype)) {
        modDoneSet.set_add(modName);
      }
    }
  }
}

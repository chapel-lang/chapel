#include <stdio.h>
#include "astutil.h"
#include "codegen.h"
#include "driver.h"
#include "files.h"
#include "mysystem.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "../traversals/createConfigVarTable.h"


#define STRSUB(x)                               \
  *ch = '\0';                                   \
  sym->cname = stringcat(sym->cname, x, ch+1);  \
  ch = sym->cname

static void legalizeCName(Symbol* sym) {
  for (char* ch = sym->cname; *ch != '\0'; ch++) {
    switch (*ch) {
    case '>': STRSUB("_GREATER_"); break;
    case '<': STRSUB("_LESS_"); break;
    case '=': STRSUB("_EQUAL_"); break;
    case '*': STRSUB("_ASTERISK_"); break;
    case '/': STRSUB("_SLASH_"); break;
    case '+': STRSUB("_PLUS_"); break;
    case '-': STRSUB("_HYPHEN_"); break;
    case '^': STRSUB("_CARET_"); break;
    case '&': STRSUB("_AMPERSAND_"); break;
    case '|': STRSUB("_BAR_"); break;
    case '!': STRSUB("_EXCLAMATION_"); break;
    case '#': STRSUB("_POUND_"); break;
    case '?': STRSUB("_QUESTION_"); break;
    default: break;
    }
  }
}

static void codegen_header(void) {
  ChainHashMap<char*, StringHashFns, int> cnames;
  Vec<TypeSymbol*> typeSymbols;
  Vec<FnSymbol*> fnSymbols;
  Vec<VarSymbol*> varSymbols;

  chpl_main->addPragma("rename _chpl_main");

  cnames.put("main", 1);
  cnames.put("stdin", 1);
  cnames.put("stdout", 1);
  cnames.put("stderr", 1);
  cnames.put("fwrite", 1);
  cnames.put("fread", 1);

  // Remove functions not used (misses mutual recursion)
  compute_call_sites();
  Vec<FnSymbol*> all_functions;
  collect_functions(&all_functions);
  forv_Vec(FnSymbol, fn, all_functions) {
    if (fn->calledBy->n == 0 && chpl_main != fn)
      fn->defPoint->parentStmt->remove();
  }

  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    Symbol* sym = dynamic_cast<Symbol*>(ast);
    if (!sym)
      continue;
    if (sym->name == sym->cname)
      sym->cname = stringcpy(sym->name);

    if (char* pragma = sym->hasPragma("rename"))
      sym->cname = stringcpy(pragma+7);

    if (sym->parentScope->type < SCOPE_MODULE)
      continue;

    legalizeCName(sym);

    // mangle symbol that is neither field nor formal if the symbol's
    // name has already been encountered
    if (!dynamic_cast<ArgSymbol*>(sym) &&
        sym->parentScope->type != SCOPE_CLASS &&
        cnames.get(sym->cname))
      sym->cname = stringcat("_", intstring(sym->id), "_", sym->cname);

    cnames.put(sym->cname, 1);

    if (TypeSymbol* typeSymbol = dynamic_cast<TypeSymbol*>(sym)) {
      typeSymbols.add(typeSymbol);
    } else if (FnSymbol* fnSymbol = dynamic_cast<FnSymbol*>(sym)) {
      fnSymbols.add(fnSymbol);
    } else if (VarSymbol* varSymbol = dynamic_cast<VarSymbol*>(sym)) {
      if (varSymbol->parentScope->type == SCOPE_MODULE)
        varSymbols.add(varSymbol);
    }
  }

  FILE* outfile = openCFile("_chpl_header.h");
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    typeSymbol->codegenPrototype(outfile);
  }
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (dynamic_cast<EnumType*>(typeSymbol->definition) ||
        dynamic_cast<UserType*>(typeSymbol->definition)) {
      typeSymbol->codegenDef(outfile);
    }
  }
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (!dynamic_cast<EnumType*>(typeSymbol->definition) &&
        !dynamic_cast<VariableType*>(typeSymbol->definition) &&
        !dynamic_cast<UserType*>(typeSymbol->definition)) {
      typeSymbol->codegenDef(outfile);
    }
  }
  forv_Vec(FnSymbol, fnSymbol, fnSymbols) {
    fnSymbol->codegenPrototype(outfile);
  }
  forv_Vec(VarSymbol, varSymbol, varSymbols) {
    varSymbol->codegenDef(outfile);
  }
  closeCFile(outfile);
}


void codegen(void) {
  if (no_codegen)
    return;

  codegen_header();

  openMakefile(allModules.v[0]->filename, system_dir);

  CreateConfigVarTable* createConfigVarTable = new CreateConfigVarTable();
  createConfigVarTable->run(&allModules);
  createConfigVarTable->closeCFile();

  forv_Vec(ModuleSymbol, currentModule, allModules) {
    if (currentModule->modtype != MOD_INTERNAL) {
      mysystem(stringcat("# codegen-ing module", currentModule->name),
               "generating comment for --print-commands option");
      currentModule->codegenDef();
    }
  }

  closeMakefile();
  makeBinary();
}

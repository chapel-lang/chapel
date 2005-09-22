#include "codegenOne.h"
#include "symtab.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"
#include "files.h"
#include "stringutil.h"


static void applyRenamePragma(Symbol* sym) {
  if (char* pragma = sym->hasPragma("rename"))
    sym->cname = copystring(pragma+7);
}


static void legalizeCName(Symbol* sym) {
  for (char* ch = sym->cname; *ch != '\0'; ch++) {
    switch (*ch) {
    case '?': *ch = 'Q'; break;
    case '-': *ch = '_'; break;
    default: break;
    };
  }
}


CodegenOne::CodegenOne() {
  FnSymbol::mainFn->addPragma("rename _chpl_main");
  cnames.put("main", 1);
  cnames.put("stdin", 1);
  cnames.put("stdout", 1);
  cnames.put("stderr", 1);
}


void CodegenOne::processSymbol(Symbol* sym) {
  if (sym->name == sym->cname)
    sym->cname = copystring(sym->name);

  applyRenamePragma(sym);

  if (sym->parentScope->type < SCOPE_MODULE)
    return;

  legalizeCName(sym);

  if (!dynamic_cast<ArgSymbol*>(sym) && sym->parentScope->type != SCOPE_CLASS) {
    if (cnames.get(sym->cname)) {
      sym->cname = glomstrings(4, "_", intstring(sym->id), "_", sym->cname);
    } else {
      cnames.put(sym->cname, 1);
    }
  }

  if (TypeSymbol* typeSymbol = dynamic_cast<TypeSymbol*>(sym)) {
    typeSymbols.add(typeSymbol);
  } else if (FnSymbol* fnSymbol = dynamic_cast<FnSymbol*>(sym)) {
    fnSymbols.add(fnSymbol);
  } else if (VarSymbol* varSymbol = dynamic_cast<VarSymbol*>(sym)) {
    if (varSymbol->parentScope->type == SCOPE_MODULE) {
      varSymbols.add(varSymbol);
    }
  }
}


void CodegenOne::run(Vec<ModuleSymbol*>* modules) {

  // Remove functions not used (misses mutual recursion)
  compute_call_sites();
  Vec<FnSymbol*> all_functions;
  collect_functions(&all_functions);
  forv_Vec(FnSymbol, fn, all_functions) {
    if (fn->calledBy->n == 0 && FnSymbol::mainFn != fn)
      fn->defPoint->parentStmt->remove();
  }

  SymtabTraversal::run(modules);
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

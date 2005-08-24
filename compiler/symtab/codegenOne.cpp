#include "codegenOne.h"
#include "symtab.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"
#include "files.h"
#include "stringutil.h"


CodegenOne::CodegenOne() {
  whichModules = MODULES_CODEGEN;
}


void CodegenOne::processSymbol(Symbol* sym) {
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
  bool oneUserModule = (ModuleSymbol::numUserModules(modules) == 1);
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
    ModuleSymbol* parentMod = varSymbol->parentScope->getModule();
    if (parentMod->modtype != MOD_USER || !oneUserModule) {
      // Mangle global variable cname if this is from a chapel module
      // or there is more than one user module
      varSymbol->cname =
        glomstrings(3, parentMod->cname, "_", varSymbol->cname);
    }
    varSymbol->codegenDef(outfile);
  }
  closeCFile(outfile);
}

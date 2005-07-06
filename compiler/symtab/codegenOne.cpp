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


void CodegenOne::run(ModuleList* moduleList) {
  SymtabTraversal::run(moduleList);
  FILE* outfile = openCFile("_chpl_header.h");
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    typeSymbol->codegenPrototype(outfile);
  }
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (dynamic_cast<EnumType*>(typeSymbol->type) ||
        dynamic_cast<UserType*>(typeSymbol->type)) {
      typeSymbol->codegenDef(outfile);
    }
  }
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (!dynamic_cast<EnumType*>(typeSymbol->type) &&
        !dynamic_cast<UserType*>(typeSymbol->type)) {
      typeSymbol->codegenDef(outfile);
    }
  }
  forv_Vec(FnSymbol, fnSymbol, fnSymbols) {
    fnSymbol->codegenPrototype(outfile);
  }
  forv_Vec(VarSymbol, varSymbol, varSymbols) {
    // Mangle global variable cname
    varSymbol->cname =
      glomstrings(3, varSymbol->parentScope->getModule()->cname,
                  "_", varSymbol->cname);
    varSymbol->codegenDef(outfile);
  }
  closeCFile(outfile);
}

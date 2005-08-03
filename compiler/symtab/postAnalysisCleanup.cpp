#include "postAnalysisCleanup.h"
#include "analysis.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/resolveSymbols.h"


static void
resolveDefaultConstructorParameters(Symbol* sym,
                                    FnSymbol* fn,
                                    VarInitExpr* init);


PostAnalysisCleanup::PostAnalysisCleanup() {
  //  whichModules = MODULES_CODEGEN;
}


void PostAnalysisCleanup::processSymbol(Symbol* sym) {
  /***
   ***  Hack: change default constructor parameter types
   ***/
  if (dynamic_cast<NilType*>(sym->type)) {
    if (sym->defPoint && sym->defPoint->init) {
      if (VarInitExpr* init = dynamic_cast<VarInitExpr*>(sym->defPoint->init)) {
        FnSymbol* constructor = dynamic_cast<FnSymbol*>(sym->defPoint->parentSymbol);
        resolveDefaultConstructorParameters(sym, constructor, init);
        sym->defPoint->init->replace(new VarInitExpr(new Variable(sym)));
      }
    }
  }
  if (dynamic_cast<ParamSymbol*>(sym) && sym->defPoint->init) {
    if (VarInitExpr* init = dynamic_cast<VarInitExpr*>(sym->defPoint->init)) {
      FnSymbol* constructor = dynamic_cast<FnSymbol*>(sym->defPoint->parentSymbol);
      resolveDefaultConstructorParameters(sym, constructor, init);
    }
    sym->defPoint->init->remove();
  }
}


static void resolveDefaultConstructorParameters(Symbol* sym,
                                                FnSymbol* fn,
                                                VarInitExpr* init) {
  if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(init->expr)) {
    StructuralType* structuralType = dynamic_cast<StructuralType*>(fn->retType);

    if (!structuralType) {
      INT_FATAL(sym, "Error in default constructor parameter hack");
    }

    forv_Vec(Symbol, field, structuralType->fields) {
      if (!strcmp(memberAccess->member->name, field->name)) {
        sym->type = field->type;
      }
    }
  }
}

#include <typeinfo>
#include "resolveTypes.h"
#include "analysis.h"
#include "stmt.h"
#include "expr.h"


ResolveTypes::ResolveTypes() {
  //  whichModules = MODULES_CODEGEN;
}


static bool types_match(Type* super, Type* sub) {
  if (sub == super) {
    return true;
  } else if (dynamic_cast<ClassType*>(super) && sub == dtNil) {
    return true;
  }
  return false;
}


static bool replaceTypeWithAnalysisType(Symbol* sym) {
  if (sym->type == NULL) {
    // BLC: if there's no type, apparently we can't call into analysis?
    return false;
  }
  if (is_Scalar_Type(sym->type) || sym->type->astType == TYPE_USER) {
    // BLC (quoting from John's 05/10/05 log message): "analysis
    // defers to declared scalar types since unused scalars would
    // otherwise not have a type from analysis (we could handle these
    // cases specially, perhaps emitting a warning)"
    return false;
  }
  // otherwise, use what analysis gives us
  return true;
}


void ResolveTypes::processSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (fn->retType == dtUnknown) {
      fn->retType = return_type_info(fn);
      if (checkAnalysisTypeinfo) {
        if (fn->retType == dtUnknown) {
          INT_FATAL(fn, "Analysis unable to determine return type of '%s'", fn->cname);
        }
      }
    } else {
      Type* analysisRetType = return_type_info(fn);
      if (!types_match(fn->retType, analysisRetType)) {
        if (checkAnalysisTypeinfo) {
          INT_WARNING(fn, "Analysis return type mismatch (%s/%s) of '%s'",
                      fn->retType->symbol->name,
                      analysisRetType->symbol->name,
                      fn->cname);
        }
        fn->retType = analysisRetType;
      } else {
        fn->retType = analysisRetType;
      }
    }
  } else if (sym->type == dtUnknown || replaceTypeWithAnalysisType(sym)) {
    sym->type = type_info(sym);
    if (checkAnalysisTypeinfo) {
      if (sym->type == dtUnknown) {
        INT_FATAL(sym, "Analysis unable to determine type of '%s'", sym->cname);
      }
    }
  } else {
    Type* analysisType = type_info(sym);
    if (!types_match(sym->type, analysisType)) {
      if (checkAnalysisTypeinfo) {
        INT_WARNING(sym, "Analysis type mismatch (%s/%s) of '%s'",
                    sym->type->symbol->name,
                    analysisType->symbol->name,
                    sym->cname);
        if (analysisType != dtUnknown) {
          sym->type = analysisType;
        }
      }
    } else {
      sym->type = analysisType;
    }
  }

  if (sym->type && 
      !dynamic_cast<TypeSymbol*>(sym) &&
      !dynamic_cast<FnSymbol*>(sym)
    ) {
    TypeSymbol* symType = dynamic_cast<TypeSymbol*>(sym->type->symbol);
    if (symType == NULL) {
      INT_FATAL("null symType");
    }
    if (!type_is_used(symType)) {
      INT_FATAL(sym, "type_is_used assertion failure\n"
                "(likely to be due to dead code not being eliminated)");
    }
  }
}

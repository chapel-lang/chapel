#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "stringutil.h"


static void resolve_type(Symbol* sym);


void resolve_analyzed(void) {
  if (no_infer)
    return;
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(ast)) {
      if (!type_is_used(ts))
        ts->defPoint->parentStmt->remove();
    } else if (FnSymbol* fs = dynamic_cast<FnSymbol*>(ast)) {
      if (!function_is_used(fs))
        fs->defPoint->parentStmt->remove();
    }
  }

  asts.clear();
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (ArgSymbol* as = dynamic_cast<ArgSymbol*>(ast)) {
      if (as->defaultExpr)
        as->defaultExpr->remove();
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    if (Symbol* a = dynamic_cast<Symbol*>(ast)) {
      if (!dynamic_cast<LabelSymbol*>(a))
        resolve_type(a);
    }
  }
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


static void resolve_type(Symbol* sym) {
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
      !dynamic_cast<FnSymbol*>(sym)) {
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

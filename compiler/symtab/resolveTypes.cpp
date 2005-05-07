#include "resolveTypes.h"
#include "analysis.h"
#include "stmt.h"
#include "expr.h"


class FindReturn : public Traversal {
 public:
  bool found;
  FindReturn(void);
  void preProcessStmt(Stmt* stmt);
};

FindReturn::FindReturn() {
  found = false;
}

void FindReturn::preProcessStmt(Stmt* stmt) {
  if (dynamic_cast<ReturnStmt*>(stmt)) {
    found = true;
  }
}

ResolveTypes::ResolveTypes() {
  whichModules = MODULES_COMMON_AND_USER;
}


static bool types_match(Type* super, Type* sub) {
  if (sub == super) {
    return true;
  } else if (dynamic_cast<StructuralType*>(super) &&
             dynamic_cast<NilType*>(sub)) {
    return true;
  } else if (ClassType* superClass = dynamic_cast<ClassType*>(super)) {
    if (ClassType* subClass = dynamic_cast<ClassType*>(sub)) {
      if (subClass->parentClasses.in(superClass)) {
        return true;
      }
    }
  }
  return false;
}


void ResolveTypes::processSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (fn->retType == dtUnknown) {
      if (analyzeAST) {
        fn->retType = return_type_info(fn);
        if (checkAnalysisTypeinfo) {
          if (fn->retType == dtUnknown) {
            INT_FATAL(fn, "Analysis unable to determine return type of '%s'", fn->cname);
          }
        }
      } else {
        FindReturn* findReturn = new FindReturn();
        TRAVERSE_LS(fn->body, findReturn, true);
        if (!findReturn->found) {
          fn->retType = dtVoid;
        } else {
          INT_FATAL(fn, "Analysis required to determine return type of '%s'", fn->cname);
        }
      }
    } else if (analyzeAST) {
      Type* analysisRetType = return_type_info(fn);
      if (!types_match(fn->retType, analysisRetType)) {
        if (checkAnalysisTypeinfo) {
          INT_WARNING(fn, "Analysis return type mismatch (%s/%s) of '%s'",
                      fn->retType->symbol->name,
                      analysisRetType->symbol->name,
                      fn->cname);
          if (analysisRetType != dtUnknown) {
            fn->retType = analysisRetType;
          }
        }
      } else {
        fn->retType = analysisRetType;
      }
    }
  } else if (sym->type == dtUnknown) {
    if (analyzeAST) {
      sym->type = type_info(sym);
      if (checkAnalysisTypeinfo) {
        if (sym->type == dtUnknown) {
          INT_FATAL(sym, "Analysis unable to determine type of '%s'", sym->cname);
        }
      }
    } else {
      INT_FATAL(sym, "Analysis required to determine type of '%s'", sym->cname);
    }
  } else if (analyzeAST) {
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

  /***
   ***  Hack: loops over sequences, types of index variables
   ***/
  if (dynamic_cast<VarSymbol*>(sym)) {
    if (sym->type == dtInteger) {
      if (ForLoopStmt* for_loop =
          dynamic_cast<ForLoopStmt*>(sym->defPoint->parentStmt)) {
        if (DefExpr* def_expr = dynamic_cast<DefExpr*>(for_loop->indices)) {
          if (def_expr->sym == sym) {
            if (SeqType* seq_type = dynamic_cast<SeqType*>(for_loop->domain->typeInfo())) {
              sym->type = seq_type->elementType;
            }
          }
        }
      }
    }
  }
}

ResolveTupleTypes::ResolveTupleTypes() {
  whichModules = MODULES_COMMON_AND_USER;
}

void ResolveTupleTypes::processSymbol(Symbol* sym) {
  if (!analyzeAST)
    return;
  if (TypeSymbol *t = dynamic_cast<TypeSymbol*>(sym)) {
    if (TupleType *tt = dynamic_cast<TupleType*>(t->type)) {
      tt->components.clear();
      forv_Vec(VarSymbol, v, tt->fields) {
        Type *t = type_info(v);
        tt->components.add(t);
      }
    }
  }
}

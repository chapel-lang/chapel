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


void ResolveTypes::processSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (fn->retType == dtUnknown) {
      if (analyzeAST) {
        fn->retType = return_type_info(fn);
        if (checkAnalysisTypeinfo) {
          if (fn->retType == dtUnknown) {
            INT_FATAL(fn, "Analysis unable to determine type");
          }
        }
      } else {
        FindReturn* findReturn = new FindReturn();
        TRAVERSE_LS(fn->body, findReturn, true);
        if (!findReturn->found) {
          fn->retType = dtVoid;
        } else {
          INT_FATAL(fn, "Analysis required to determine return type");
        }
      }
    } else if (analyzeAST) {
      if (checkAnalysisTypeinfo) {
        if (fn->retType != return_type_info(fn)) {
          INT_WARNING(fn, "Analysis return type mismatch, using analysis type");
          fn->retType = return_type_info(fn);
          if (fn->retType == dtUnknown) {
            INT_FATAL(fn, "Analysis unable to to determine type");
          }
        }
      }
    }
  } else if (sym->type == dtUnknown) {
    sym->type = type_info(sym);
    if (checkAnalysisTypeinfo) {
      if (sym->type == dtUnknown) {
        INT_FATAL(sym, "Analysis unable to determine type");
      }
    }
  } else if (analyzeAST) {
    if (checkAnalysisTypeinfo) {
      if (sym->type != type_info(sym)) {
        INT_WARNING(sym, "Analysis type mismatch, using analysis type");
      }
      sym->type = type_info(sym);
      if (sym->type == dtUnknown) {
        INT_FATAL(sym, "Analysis unable to determine type");
      }
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

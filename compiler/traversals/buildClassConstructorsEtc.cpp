#include "buildClassConstructorsEtc.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"


static void build_record_equality_function(ClassType* class_type) {
  if (!class_type->value) {
    return;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("=="));
  ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", class_type);
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2", class_type);
  arg1->append(arg2);
  Variable* false_variable = new Variable(Symboltable::lookup("false"));
  ReturnStmt* return_false = new ReturnStmt(false_variable);
  Stmt* body = NULL;
  forv_Vec(VarSymbol, tmp, class_type->fields) {
    MemberAccess* left = new MemberAccess(new Variable(arg1), tmp);
    MemberAccess* right = new MemberAccess(new Variable(arg2), tmp);
    BinOp* cond_expr = new BinOp(BINOP_NEQUAL, left, right);
    CondStmt* cond_stmt = new CondStmt(cond_expr, return_false->copy());
    body = appendLink(body, cond_stmt);
  }
  Variable* true_variable = new Variable(Symboltable::lookup("true"));
  body = appendLink(body, new ReturnStmt(true_variable));
  BlockStmt* block_stmt = new BlockStmt(body);
  DefExpr* def_expr = Symboltable::finishFnDef(fn, arg1, dtBoolean, block_stmt);
  fn->setDefPoint(def_expr);
  DefStmt* def_stmt = new DefStmt(def_expr);
  dynamic_cast<DefExpr*>(class_type->symbol->defPoint)->stmt->insertBefore(def_stmt);
}


BuildClassConstructorsEtc::BuildClassConstructorsEtc(void) {
  processInternalModules = false;
}


void BuildClassConstructorsEtc::postProcessExpr(Expr* expr) {
  DefExpr* def_expr = dynamic_cast<DefExpr*>(expr);

  if (!def_expr) {
    return;
  }

  if (TypeSymbol* type_symbol = dynamic_cast<TypeSymbol*>(def_expr->sym)) {
    if (ClassType* class_type = dynamic_cast<ClassType*>(type_symbol->type)) {
      build_record_equality_function(class_type);
    }
  }
}

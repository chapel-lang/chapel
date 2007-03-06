//
// scalarReplace
//
// This pass implements scalar replacement of aggregates.
//

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"

void scalarReplace() {
  if (fScalarReplaceTuples) {
    forv_Vec(TypeSymbol, ts, gTypes) {
      if (ts->hasPragma("scalar replace tuples")) {
        scalarReplace(dynamic_cast<ClassType*>(ts->type));
      }
    }
  }
  // note: disabled on inlining because scalar replace does not work
  // with inlining, fix when intent_ref and references work
  if (!fDisableScalarReplaceArrayWrappers && !no_inline) {
    forv_Vec(TypeSymbol, ts, gTypes) {
      if (ts->hasPragma("domain") || ts->hasPragma("array")) {
        scalarReplace(dynamic_cast<ClassType*>(ts->type));
      }
    }
  }
}

void scalarReplace(ClassType* ct) {
  Map<Symbol*,Vec<Symbol*>*> sym2syms; // symbol to replacements map
  Map<Symbol*,int> field2id;           // field to number map
  int nfields = 0;                     // number of fields

  //
  // compute field ordering numbers
  //
  for_fields(field, ct) {
    field2id.put(field, nfields++);
  }

  //
  // replace symbol definitions of structural type with multiple
  // symbol definitions of structural field types
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (def->sym->astType != SYMBOL_VAR && def->sym->astType != SYMBOL_ARG)
        continue;
      if (!def->parentSymbol || def->sym->type != ct)
        continue;
      Symbol* sym = def->sym;
      ArgSymbol* arg = dynamic_cast<ArgSymbol*>(sym);
      bool isThis = !strcmp("this", sym->name);
      Vec<Symbol*>* syms = new Vec<Symbol*>();
      FnSymbol* fn = def->getFunction();
      Symbol* ret = fn->getReturnSymbol();
      for_fields(field, ct) {
        Symbol* clone;
        char* name = sym->name;
        if (strcmp("_value", field->name))
          name = astr(name, "_", field->name);
        if (def->sym == ret || arg)
          clone = new ArgSymbol((arg && !isThis) ? arg->intent : INTENT_REF,
                                name, field->type);
        else
          clone = new VarSymbol(name, field->type);
        if (sym->isReference)
          clone->isReference = true;
        syms->add(clone);
        if (def->sym == ret)
          fn->insertFormalAtTail(new DefExpr(clone));
        else
          def->insertBefore(new DefExpr(clone));
      }
      sym2syms.put(def->sym,syms);
      def->remove();
      if (def->sym == fn->_this)
        fn->_this = NULL;
    }
  }

  //
  // expand uses of symbols of structural type with multiple symbols
  // structural field types
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (SymExpr* se = dynamic_cast<SymExpr*>(ast)) {
      if (!se->parentSymbol || se->var->type != ct)
        continue;
      Vec<Symbol*>* syms = sym2syms.get(se->var);
      CallExpr* call = dynamic_cast<CallExpr*>(se->parentExpr);
      if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        if (se == call->get(1)) {
          SymExpr* member = dynamic_cast<SymExpr*>(call->get(2));
          int id = field2id.get(member->var);
          call->replace(new SymExpr(syms->v[id]));
        } else if (se == call->get(2)) {
          SymExpr* base = dynamic_cast<SymExpr*>(call->get(1));
          for (int id = 0; id < nfields; id++) {
            call->insertBefore(
              new CallExpr(PRIMITIVE_GET_MEMBER, base->var, syms->v[id]));
          }
          call->remove();
        }
      } else if (call && call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        if (se == call->get(1)) {
          SymExpr* member = dynamic_cast<SymExpr*>(call->get(2));
          int id = field2id.get(member->var);
          call->primitive = primitives[PRIMITIVE_MOVE];
          call->get(2)->remove();
          call->get(1)->remove();
          call->insertAtHead(syms->v[id]);
        } else {
          for (int id = 0; id < nfields; id++) {
            se->insertBefore(new SymExpr(syms->v[id]));
          }
          se->remove();
        }
      } else if (call && call->isPrimitive(PRIMITIVE_RETURN)) {
        se->replace(new SymExpr(gVoid));
      } else {
        for (int id = 0; id < nfields; id++) {
          se->insertBefore(new SymExpr(syms->v[id]));
        }
        se->remove();
      }
    }
  }
  
  //
  // repair moves
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* move = dynamic_cast<CallExpr*>(ast)) {
      if (!move->parentSymbol)
        continue;
      if (move->isPrimitive(PRIMITIVE_MOVE) ||
          move->isPrimitive(PRIMITIVE_REF)) {
        bool isref = move->isPrimitive(PRIMITIVE_REF);
        if (CallExpr* call = dynamic_cast<CallExpr*>(move->argList->tail)) {
          if (FnSymbol* fn = call->isResolved()) {
            if (fn->retType == ct) {
              call->remove();
              for_actuals(actual, move) {
                actual->remove();
                call->insertAtTail(actual);
              }
              move->replace(call);
              continue;
            }
          }
        }
        int n = move->argList->length();
        while (n > 2) {
          Expr* rhs = move->get(n/2+1)->remove();
          Expr* lhs = move->get(1)->remove();
          move->insertBefore(new CallExpr((isref) ? PRIMITIVE_REF : PRIMITIVE_MOVE, lhs, rhs));
          n -= 2;
        }
      } else if (move->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        int n = move->argList->length();
        while (n > 3) {
          Expr* rhs = move->get(n/2+2)->remove();
          Expr* lhs = move->get(2)->remove();
          move->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, move->get(1)->copy(), lhs, rhs));
          n -= 2;
        }
      }
    }
  }

  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->retType == ct)
      fn->retType = dtVoid;
  }
}

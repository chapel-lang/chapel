/*
 * Copyright 2004-2018 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "postFold.h"

#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "preFold.h"
#include "resolution.h"
#include "stringutil.h"
#include "symbol.h"

#include "../ifa/prim_data.h"

static Expr* postFoldNormal(CallExpr* call);

static Expr* postFoldPrimop(CallExpr* call);

static Expr* postFoldMove(CallExpr* call);

static Expr* postFoldSymExpr(SymExpr* symExpr);

#define FOLD_CALL1(prim)                                                \
  if (SymExpr* se = toSymExpr(call->get(1))) {                          \
    Symbol* sym = se->symbol();                                         \
    if (isEnumSymbol(sym)) {                      \
      ensureEnumTypeResolved(toEnumType(sym->type));                    \
    }                                                                   \
    if (Immediate* imm = getSymbolImmediate(sym)) {                     \
      Immediate i3;                                                     \
                                                                        \
      fold_constant(prim, imm, NULL, &i3);                              \
                                                                        \
      retval = new SymExpr(new_ImmediateSymbol(&i3));                   \
                                                                        \
      call->replace(retval);                                            \
    }                                                                   \
  }

#define FOLD_CALL2(prim)                                                \
  if (SymExpr* lhsSe = toSymExpr(call->get(1))) {                       \
    if (SymExpr* rhsSe = toSymExpr(call->get(2))) {                     \
      Symbol* lhsSym = lhsSe->symbol();                                 \
      Symbol* rhsSym = rhsSe->symbol();                                 \
      if (isEnumSymbol(lhsSym)) {                 \
        ensureEnumTypeResolved(toEnumType(lhsSym->type));               \
      }                                                                 \
      if (isEnumSymbol(rhsSym)) {                 \
        ensureEnumTypeResolved(toEnumType(rhsSym->type));               \
      }                                                                 \
      if (Immediate* lhs = getSymbolImmediate(lhsSym)) {                \
        if (Immediate* rhs = getSymbolImmediate(rhsSym)) {              \
          Immediate i3;                                                 \
                                                                        \
          fold_constant(prim, lhs, rhs, &i3);                           \
                                                                        \
          retval = new SymExpr(new_ImmediateSymbol(&i3));               \
                                                                        \
          call->replace(retval);                                        \
        }                                                               \
      }                                                                 \
    }                                                                   \
  }

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Expr* postFold(Expr* expr) {
  SET_LINENO(expr);

  Expr* retval = expr;

  INT_ASSERT(expr->parentSymbol != NULL);

  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isResolved() == true) {
      retval = postFoldNormal(call);

    } else if (call->isPrimitive() == true) {
      retval = postFoldPrimop(call);
    }

  } else if (SymExpr* sym = toSymExpr(expr)) {
    retval = postFoldSymExpr(sym);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static Expr* postFoldNormal(CallExpr* call) {
  FnSymbol* fn     = call->resolvedFunction();
  Expr*     retval = call;

  if (fn->retTag == RET_PARAM || fn->hasFlag(FLAG_MAYBE_PARAM) == true) {
    VarSymbol* ret = toVarSymbol(fn->getReturnSymbol());

    if (ret != NULL && ret->immediate != NULL) {
      retval = new SymExpr(ret);

      call->replace(retval);

    } else if (EnumSymbol* es = toEnumSymbol(fn->getReturnSymbol())) {
      retval = new SymExpr(es);

      call->replace(retval);

    } else if (ret == gVoid) {
      retval = new SymExpr(gVoid);

      call->replace(retval);
    }
  }

  if (fn->hasFlag(FLAG_MAYBE_TYPE)                       == true &&
      fn->getReturnSymbol()->hasFlag(FLAG_TYPE_VARIABLE) == true) {
    fn->retTag = RET_TYPE;
  }

  if (fn->retTag == RET_TYPE) {
    Symbol* ret = fn->getReturnSymbol();

    if (ret->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) == false) {
      retval = new SymExpr(ret->type->symbol);

      call->replace(retval);
    }
  }

  if (call->isNamedAstr(astrSequals) == true) {
    if (SymExpr* lhs = toSymExpr(call->get(1))) {
      if (lhs->symbol()->hasFlag(FLAG_MAYBE_PARAM) == true ||
          lhs->symbol()->isParameter()             == true) {
        if (paramMap.get(lhs->symbol())) {
          USR_FATAL(call, "parameter set multiple times");
        }
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool  isSubTypeOrInstantiation(Type* sub, Type* super);

static void  insertValueTemp(Expr* insertPoint, Expr* actual);

static Expr* postFoldPrimop(CallExpr* call) {
  Expr* retval = call;

  if (call->isPrimitive(PRIM_MOVE) == true) {
    retval = postFoldMove(call);

  } else if (call->isPrimitive(PRIM_QUERY_TYPE_FIELD)  == true ||
             call->isPrimitive(PRIM_QUERY_PARAM_FIELD) == true) {
    SymExpr* classWrap = toSymExpr(call->get(1));

    if (AggregateType* at = toAggregateType(classWrap->symbol()->type)) {
      const char*  memberName = get_string(call->get(2));
      Vec<Symbol*> keys;

      at->substitutions.get_keys(keys);

      forv_Vec(Symbol, key, keys) {
        if (strcmp(memberName, key->name) == 0) {
          // If there is a substitution for it, replace this call with that
          // substitution
          if (Symbol* value = at->substitutions.get(key)) {
            retval = new SymExpr(value);

            call->replace(retval);
          }
        }
      }

    } else {
      USR_FATAL(call,
                "Attempted to obtain field of a type that "
                "was not a record or class");
    }

  } else if (call->isPrimitive(PRIM_GET_MEMBER) == true) {
    Type*       baseType   = call->get(1)->getValType();
    const char* memberName = get_string(call->get(2));
    Symbol*     sym        = baseType->getField(memberName);
    SymExpr*    left       = toSymExpr(call->get(1));
    VarSymbol*  varSym     = toVarSymbol(sym);

    if (left != NULL && left->symbol()->hasFlag(FLAG_TYPE_VARIABLE) == true &&
        !sym->isParameter()) {
      retval = new SymExpr(sym->type->symbol);

      call->replace(retval);

    } else if (sym->isParameter() == true ||
               (varSym != NULL && varSym->isType() == true)) {
      Vec<Symbol*> keys;

      baseType->substitutions.get_keys(keys);

      forv_Vec(Symbol, key, keys) {
        if (strcmp(sym->name, key->name) == 0) {
          if (Symbol* value = baseType->substitutions.get(key)) {
            retval = new SymExpr(value);

            call->replace(retval);
          }
        }
      }
    }

  } else if (call->isPrimitive(PRIM_IS_SUBTYPE) == true) {
    SymExpr* parentExpr = toSymExpr(call->get(1));
    SymExpr* subExpr    = toSymExpr(call->get(2));
    if (isTypeExpr(parentExpr) == true  ||
        isTypeExpr(subExpr)    == true)  {
      Type* st = subExpr->getValType();
      Type* pt = parentExpr->getValType();

      if (st->symbol->hasFlag(FLAG_DISTRIBUTION) && isDistClass(pt)) {
        AggregateType* ag = toAggregateType(st);
        st = ag->getField("_instance")->type;
      } else {
        // Try to work around some resolution order issues
        st = resolveTypeAlias(subExpr);
        pt = resolveTypeAlias(parentExpr);
      }

      if (st                                != dtUnknown &&
          pt                                != dtUnknown &&

          st                                != dtAny     &&
          pt                                != dtAny     &&

          st->symbol->hasFlag(FLAG_GENERIC) == false) {

        if (isSubTypeOrInstantiation(st, pt) == true) {
          retval = new SymExpr(gTrue);

        } else {
          retval = new SymExpr(gFalse);
        }

        call->replace(retval);
      } else {
        USR_FATAL(call, "Unable to perform subtype query: %s:%s", st->symbol->name, pt->symbol->name);
      }
    } else {
      USR_FATAL(call, "Subtype query requires a type");
    }

  } else if (call->isPrimitive(PRIM_CAST) == true) {
    Type* t= call->get(1)->typeInfo();

    if (t == dtUnknown) {
      INT_FATAL(call, "Unable to resolve type");
    }

    call->get(1)->replace(new SymExpr(t->symbol));

  } else if (call->isPrimitive("string_compare") == true) {
    SymExpr* lhs = toSymExpr(call->get(1));
    SymExpr* rhs = toSymExpr(call->get(2));

    INT_ASSERT(lhs && rhs);

    if (lhs->symbol()->isParameter() && rhs->symbol()->isParameter()) {
      const char* lstr = get_string(lhs);
      const char* rstr = get_string(rhs);
      int         cmp  = strcmp(lstr, rstr);

      retval = new SymExpr(new_IntSymbol(cmp));

      call->replace(retval);
    }

  } else if (call->isPrimitive("string_concat") == true) {
    SymExpr* lhs = toSymExpr(call->get(1));
    SymExpr* rhs = toSymExpr(call->get(2));

    INT_ASSERT(lhs && rhs);

    if (lhs->symbol()->isParameter() && rhs->symbol()->isParameter()) {
      const char* lstr = get_string(lhs);
      const char* rstr = get_string(rhs);

      if (lhs->symbol()->type == dtString) {
        retval = new SymExpr(new_StringSymbol(astr(lstr, rstr)));
      } else {
        retval = new SymExpr(new_CStringSymbol(astr(lstr, rstr)));
      }

      call->replace(retval);
    }

  } else if (call->isPrimitive("string_length") == true) {
    SymExpr* se = toSymExpr(call->get(1));

    INT_ASSERT(se);

    if (se->symbol()->isParameter() == true) {
      const char* str    = get_string(se);
      int         length = unescapeString(str, se).length();

      retval = new SymExpr(new_IntSymbol(length, INT_SIZE_DEFAULT));

      call->replace(retval);
    }

  } else if (call->isPrimitive("ascii") == true) {
    SymExpr* se = toSymExpr(call->get(1));

    INT_ASSERT(se);

    if (se->symbol()->isParameter() == true) {
      const char*       str       = get_string(se);
      const std::string unescaped = unescapeString(str, se);

      retval = new SymExpr(new_UIntSymbol((int)unescaped[0], INT_SIZE_8));

      call->replace(retval);
    }

  } else if (call->isPrimitive("string_contains") == true) {
    SymExpr* lhs = toSymExpr(call->get(1));
    SymExpr* rhs = toSymExpr(call->get(2));

    INT_ASSERT(lhs && rhs);

    if (lhs->symbol()->isParameter() == true &&
        rhs->symbol()->isParameter() == true) {
      const char* lstr = get_string(lhs);
      const char* rstr = get_string(rhs);

      retval = new SymExpr(strstr(lstr, rstr) ? gTrue : gFalse);

      call->replace(retval);
    }

  } else if (call->isPrimitive(PRIM_UNARY_MINUS) == true) {
    FOLD_CALL1(P_prim_minus);

  } else if (call->isPrimitive(PRIM_UNARY_PLUS) == true) {
    FOLD_CALL1(P_prim_plus);

  } else if (call->isPrimitive(PRIM_UNARY_NOT) == true) {
    FOLD_CALL1(P_prim_not);

  } else if (call->isPrimitive(PRIM_UNARY_LNOT) == true) {
    FOLD_CALL1(P_prim_lnot);

  } else if (call->isPrimitive(PRIM_ADD) == true) {
    FOLD_CALL2(P_prim_add);

  } else if (call->isPrimitive(PRIM_SUBTRACT) == true) {
    FOLD_CALL2(P_prim_subtract);

  } else if (call->isPrimitive(PRIM_MULT) == true) {
    FOLD_CALL2(P_prim_mult);

  } else if (call->isPrimitive(PRIM_DIV) == true) {
    FOLD_CALL2(P_prim_div);

  } else if (call->isPrimitive(PRIM_MOD) == true) {
    FOLD_CALL2(P_prim_mod);

  } else if (call->isPrimitive(PRIM_EQUAL) == true) {
    FOLD_CALL2(P_prim_equal);

  } else if (call->isPrimitive(PRIM_NOTEQUAL) == true) {
    FOLD_CALL2(P_prim_notequal);

  } else if (call->isPrimitive(PRIM_LESSOREQUAL) == true) {
    FOLD_CALL2(P_prim_lessorequal);

  } else if (call->isPrimitive(PRIM_GREATEROREQUAL) == true) {
    FOLD_CALL2(P_prim_greaterorequal);

  } else if (call->isPrimitive(PRIM_LESS) == true) {
    FOLD_CALL2(P_prim_less);

  } else if (call->isPrimitive(PRIM_GREATER) == true) {
    FOLD_CALL2(P_prim_greater);

  } else if (call->isPrimitive(PRIM_AND) == true) {
    FOLD_CALL2(P_prim_and);

  } else if (call->isPrimitive(PRIM_OR) == true) {
    FOLD_CALL2(P_prim_or);

  } else if (call->isPrimitive(PRIM_XOR) == true) {
    FOLD_CALL2(P_prim_xor);

  } else if (call->isPrimitive(PRIM_POW) == true) {
    FOLD_CALL2(P_prim_pow);

  } else if (call->isPrimitive(PRIM_LSH) == true) {
    FOLD_CALL2(P_prim_lsh);

  } else if (call->isPrimitive(PRIM_RSH) == true) {
    FOLD_CALL2(P_prim_rsh);

  } else if (call->isPrimitive(PRIM_REQUIRE) == true) {
    Expr*       arg = call->argList.only();
    const char* str = NULL;

    if (get_string(arg, &str)) {
      processStringInRequireStmt(str, false);

    } else {
      USR_FATAL(call, "'require' statements require string arguments");
    }

    retval = new CallExpr(PRIM_NOOP);

    call->replace(retval);

  } else if (call->isPrimitive(PRIM_ARRAY_ALLOC)                == true ||
             strncmp(call->primitive->name, "_fscan", 6)        == 0    ||
             strcmp (call->primitive->name, "_readToEndOfLine") == 0    ||
             strcmp (call->primitive->name, "_now_timer")       == 0)   {
    for_actuals(actual, call) {
      insertValueTemp(call->getStmtExpr(), actual);
    }
  }

  return retval;
}

static bool isSubTypeOrInstantiation(Type* sub, Type* super) {
  bool retval = false;

  if (sub == super) {
    retval = true;

  } else if (AggregateType* at = toAggregateType(sub)) {
    for (int i = 0; i < at->dispatchParents.n && retval == false; i++) {
      retval = isSubTypeOrInstantiation(at->dispatchParents.v[i], super);
    }

    if (retval == false) {
      if (at->instantiatedFrom != NULL) {
        retval = isSubTypeOrInstantiation(at->instantiatedFrom,   super);
      }
    }
  }

  return retval;
}

static void insertValueTemp(Expr* insertPoint, Expr* actual) {
  if (SymExpr* se = toSymExpr(actual)) {
    if (se->symbol()->type->refType == NULL) {
      VarSymbol* tmp = newTemp("_value_tmp_", se->symbol()->getValType());

      insertPoint->insertBefore(new DefExpr(tmp));

      insertPoint->insertBefore(new CallExpr(PRIM_MOVE,
                                             tmp,
                                             new CallExpr(PRIM_DEREF,
                                                          se->symbol())));

      se->setSymbol(tmp);
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool  postFoldMoveUpdateForParam(CallExpr* call, Symbol* lhsSym);

static void  updateFlagTypeVariable(CallExpr* call, Symbol* lhsSym);

static void  postFoldMoveTail(CallExpr* call, Symbol* lhsSym);

static Expr* postFoldMove(CallExpr* call) {
  Symbol* lhsSym = toSymExpr(call->get(1))->symbol();
  Expr*   retval = call;

  if (postFoldMoveUpdateForParam(call, lhsSym) == false) {
    updateFlagTypeVariable(call, lhsSym);

    if (isSymExpr(call->get(2)) == true) {
      postFoldMoveTail(call, lhsSym);

    } else if (CallExpr* rhs = toCallExpr(call->get(2))) {
      FnSymbol* fn = rhs->resolvedFunction();

      if (fn != NULL && fn->name == astrSequals && fn->retType == dtVoid) {
        call->replace(rhs->remove());

        retval = rhs;

      } else {
        postFoldMoveTail(call, lhsSym);
      }

    } else {
      INT_ASSERT(false);
    }
  }

  return retval;
}

static bool postFoldMoveUpdateForParam(CallExpr* call, Symbol* lhsSym) {
  bool retval = false;

  if (lhsSym->hasFlag(FLAG_MAYBE_PARAM) == true ||
      lhsSym->isParameter()             == true) {
    if (paramMap.get(lhsSym) != NULL) {
      INT_FATAL(call, "parameter set multiple times");

    } else if (lhsSym->isImmediate() == true) {
      retval = true;

    } else if (SymExpr* rhs = toSymExpr(call->get(2))) {
      Symbol* rhsSym = rhs->symbol();

      if (rhsSym->isImmediate() == true ||
          isEnumSymbol(rhsSym)  == true) {
        paramMap.put(lhsSym, rhsSym);

        lhsSym->defPoint->remove();

        call->convertToNoop();

        retval = true;
      }
    }

    if (lhsSym->isParameter() == true) {
      if (retval == true) {
        if (lhsSym->hasFlag(FLAG_TEMP)     == false  &&
            isLegalParamType(lhsSym->type) == false) {
          USR_FATAL_CONT(call,
                         "'%s' is not of a supported param type",
                         lhsSym->name);
        }

      } else {
        if (lhsSym->hasFlag(FLAG_TEMP) == false) {
          if (isLegalParamType(lhsSym->type) == false) {
            USR_FATAL_CONT(call,
                           "'%s' is not of a supported param type",
                           lhsSym->name);

          } else {
            USR_FATAL_CONT(call,
                           "Initializing parameter '%s' to value "
                           "not known at compile time",
                           lhsSym->name);

            lhsSym->removeFlag(FLAG_PARAM);
          }

        } else if (lhsSym->hasFlag(FLAG_RVV) == true) {
          USR_FATAL_CONT(call,
                         "'param' functions cannot return non-'param' values");
        }
      }
    }
  }

  return retval;
}

static void updateFlagTypeVariable(CallExpr* call, Symbol* lhsSym) {
  bool isTypeVar = false;

  if        (SymExpr*  rhs = toSymExpr(call->get(2)))  {
    isTypeVar = rhs->symbol()->hasFlag(FLAG_TYPE_VARIABLE);

  } else if (CallExpr* rhs = toCallExpr(call->get(2))) {
    if (FnSymbol* fn = rhs->resolvedFunction()) {
      isTypeVar = fn->retTag == RET_TYPE;

    } else if (rhs->isPrimitive(PRIM_DEREF)  == true) {
      isTypeVar = isTypeExpr(rhs->get(1));

    } else if (rhs->isPrimitive(PRIM_TYPEOF) == true) {
      isTypeVar = true;
    }

  } else {
    INT_ASSERT(false);
  }

  if (isTypeVar == true) {
    lhsSym->addFlag(FLAG_TYPE_VARIABLE);
  }

  lhsSym->removeFlag(FLAG_MAYBE_TYPE);
}

static void postFoldMoveTail(CallExpr* call, Symbol* lhsSym) {
  if (isSymExpr(call->get(2)) == true) {
    if (isReferenceType(lhsSym->type)                          == true  ||
        lhsSym->type->symbol->hasFlag(FLAG_REF_ITERATOR_CLASS) == true  ||
        lhsSym->type->symbol->hasFlag(FLAG_ARRAY)              == true) {
      lhsSym->removeFlag(FLAG_EXPR_TEMP);
    }

  } else if (CallExpr* rhs = toCallExpr(call->get(2))) {
    if (lhsSym->hasFlag(FLAG_EXPR_TEMP)     ==  true  &&
        lhsSym->hasFlag(FLAG_TYPE_VARIABLE) == false  &&
        requiresImplicitDestroy(rhs)        ==  true) {

      if (isUserDefinedRecord(lhsSym->type) == false) {
        lhsSym->addFlag(FLAG_INSERT_AUTO_COPY);
        lhsSym->addFlag(FLAG_INSERT_AUTO_DESTROY);
      } else {
        lhsSym->addFlag(FLAG_INSERT_AUTO_DESTROY);
      }
    }

    if (isReferenceType(lhsSym->type)                          == true  ||
        lhsSym->type->symbol->hasFlag(FLAG_REF_ITERATOR_CLASS) == true  ||
        lhsSym->type->symbol->hasFlag(FLAG_ARRAY)              == true) {
      lhsSym->removeFlag(FLAG_EXPR_TEMP);
    }

    if (rhs->isPrimitive(PRIM_NO_INIT) == true) {
      // If the lhs is a primitive, then we can remove this value.
      // Otherwise retain this statement through resolveRecordInitializers.
      if (isAggregateType(rhs->get(1)->getValType()) == false) {
        call->convertToNoop();
      }
    }

  } else {
    INT_ASSERT(false);
  }
}

bool requiresImplicitDestroy(CallExpr* call) {
  bool retval = false;

  if (FnSymbol* fn = call->resolvedFunction()) {
    FnSymbol* parent = call->getFunction();

    if (parent->hasFlag(FLAG_DONOR_FN)                        == false &&
        isRecord(fn->retType)                                 == true  &&
        fn->hasFlag(FLAG_NO_IMPLICIT_COPY)                    == false &&
        fn->isIterator()                                      == false &&
        fn->retType->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE) == false &&
        fn->hasFlag(FLAG_DONOR_FN)                            == false &&
        fn->hasFlag(FLAG_INIT_COPY_FN)                        == false &&
        fn->hasFlag(FLAG_AUTO_II)                             == false &&
        fn->hasFlag(FLAG_CONSTRUCTOR)                         == false &&
        fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)                    == false &&
        strcmp(fn->name, "=")                                 !=     0 &&
        strcmp(fn->name, "_defaultOf")                        !=     0) {
      retval = true;
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static Expr* postFoldSymExpr(SymExpr* sym) {
  Expr* retval = sym;

  if (Symbol* val = paramMap.get(sym->symbol())) {
    CallExpr* call = toCallExpr(sym->parentExpr);

    if (call && call->get(1) == sym) {
      // This is a place where param substitution has already determined the
      // value of a move or assignment. If it's a RVV, then we should ignore
      // the update because the RVV may have multiple valid defs in the AST
      // that we currently cannot squash if there are multiple return
      // statements. For example, consider the following param function:
      //
      // proc foo(type t) param {
      //   if firstCheck(t) then return true;
      //   if otherCheck(t) then return false;
      //   return true;
      // }
      //
      // The final "return true" can manifest as a PRIM_MOVE into the RVV
      // variable. I think we're currently unable to move the def because
      // of GOTOs.
      //
      // If it's not a RVV, then we might be assigning to a user-defined
      // param multiple times. In that case, we'll just return the result
      // and let resolution catch the problem later.
      //
      // The substitution usually happens before resolution, so for
      // assignment, we key off of the name :-(
      if (call->isPrimitive(PRIM_MOVE) || call->isNamedAstr(astrSequals)) {
        if (sym->symbol()->hasFlag(FLAG_RVV)) {
          call->convertToNoop();
        }

        return retval;
      }
    }

    if (sym->symbol()->type != dtUnknown &&
        sym->symbol()->type != val->type) {
      Symbol* toSym = sym->symbol();

      if (toSym->hasFlag(FLAG_TYPE_VARIABLE) == false) {
        // This assertion is here to ensure compatibility with old code.
        // Todo: remove after 1.15 release.
        Symbol* newToSym = toSym->type->symbol;

        INT_ASSERT(toSym->type == newToSym->type);

        toSym = newToSym;
      }

      INT_ASSERT(toSym->hasFlag(FLAG_TYPE_VARIABLE));

      CallExpr* cast = createCast(val, toSym);

      sym->replace(cast);

      // see whether preFold will fold this _cast call
      Expr* prevResult = retval;

      retval = preFold(cast);

      if (retval == cast) {
        // if it doesn't, put things back as they were:
        cast->replace(sym);

        retval = prevResult;

        // and then do what we would've done if we hadn't used the cast
        sym->setSymbol(val);
      }
    } else {
      sym->setSymbol(val);
    }
  }

  return retval;
}

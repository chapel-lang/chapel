/*
 * Copyright 2004-2020 Cray Inc.
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
#include "DecoratedClassType.h"
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
                                                                        \
    if (isEnumSymbol(sym)) {                                            \
      ensureEnumTypeResolved(toEnumType(sym->type));                    \
    }                                                                   \
                                                                        \
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
                                                                        \
      if (isEnumSymbol(lhsSym)) {                                       \
        if (prim != P_prim_equal) {                                     \
          INT_FATAL("Trying to do a primitive other than '==' on enums"); \
        }                                                               \
        if (!isEnumSymbol(rhsSym)) {                                    \
          INT_FATAL("Trying to do a mixed enum non-enum primitive");    \
        }                                                               \
        Immediate enumcomp;                                             \
        enumcomp = (lhsSym == rhsSym);                                  \
        retval = new SymExpr(new_ImmediateSymbol(&enumcomp));           \
        call->replace(retval);                                          \
      } else {                                                          \
        if (Immediate* lhs = getSymbolImmediate(lhsSym)) {              \
          if (Immediate* rhs = getSymbolImmediate(rhsSym)) {            \
            Immediate i3;                                               \
                                                                        \
            fold_constant(prim, lhs, rhs, &i3);                         \
                                                                        \
            retval = new SymExpr(new_ImmediateSymbol(&i3));             \
                                                                        \
            call->replace(retval);                                      \
          }                                                             \
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

  INT_ASSERT(expr->inTree());

  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isResolved() == true) {
      retval = postFoldNormal(call);

    } else if (call->isPrimitive() == true) {
      retval = postFoldPrimop(call);
    } else if (SymExpr* se = toSymExpr(call->baseExpr)) {
      if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
        retval = se->copy();
        call->replace(retval);
      }
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

  if (fn->retTag == RET_PARAM || fn->hasFlag(FLAG_MAYBE_PARAM)) {
    VarSymbol* ret = toVarSymbol(fn->getReturnSymbol());

    if (ret != NULL && ret->immediate != NULL) {
      retval = new SymExpr(ret);

      call->replace(retval);

    } else if (EnumSymbol* es = toEnumSymbol(fn->getReturnSymbol())) {
      retval = new SymExpr(es);

      call->replace(retval);

    } else if (ret == gVoid) {
      retval = new CallExpr(PRIM_NOOP);
      call->replace(retval);
    } else if (ret == gUninstantiated) {
      retval = new SymExpr(gUninstantiated);
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

      // Put the call back in the AST for better errors unless we're trying
      // to ignore multiple error messages (in which case we hope for a
      // successful compilation).
      if (fatalErrorsEncountered() && !inGenerousResolutionForErrors() && !fIgnoreNilabilityErrors) {
        retval->getStmtExpr()->insertBefore(call);
      }
    }
  }

  if (call->isNamedAstr(astrSassign) == true) {
    if (SymExpr* lhs = toSymExpr(call->get(1))) {
      if (lhs->symbol()->hasFlag(FLAG_MAYBE_PARAM) == true ||
          lhs->symbol()->isParameter()             == true) {
        if (paramMap.get(lhs->symbol())) {
          USR_FATAL(call, "parameter set multiple times");
        }
      }
    }
  }

  if (fn->hasFlag(FLAG_GET_LINE_NUMBER)) {
    retval = new SymExpr(new_IntSymbol(call->linenum()));
    call->replace(retval);
  } else if (fn->hasFlag(FLAG_GET_FILE_NAME)) {
    retval = new SymExpr(new_StringSymbol(call->fname()));
    call->replace(retval);
  } else if (fn->hasFlag(FLAG_GET_FUNCTION_NAME)) {
    retval = new SymExpr(new_StringSymbol(call->getFunction()->name));
    call->replace(retval);
  } else if (fn->hasFlag(FLAG_GET_MODULE_NAME)) {
    retval = new SymExpr(new_StringSymbol(call->getModule()->name));
    call->replace(retval);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void insertValueTemp(Expr* insertPoint, Expr* actual);
static bool isSameTypeOrInstantiation(Type* sub, Type* super, Expr* ctx);

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
    SymExpr*       base      = toSymExpr (call->get(1));
    const char*    fieldName = get_string(call->get(2));

    Type*          t         = canonicalDecoratedClassType(base->getValType());
    AggregateType* at        = toAggregateType(t);
    VarSymbol*     field     = toVarSymbol(at->getField(fieldName));

    if (field->isParameter() == true || field->isType() == true) {
      if (Symbol* value = at->getSubstitution(field->name)) {
        retval = new SymExpr(value);

        call->replace(retval);
      }

    } else if (base->symbol()->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      INT_ASSERT(field->isType() == false);

      retval = new SymExpr(field->type->symbol);

      call->replace(retval);
    }

  } else if (call->isPrimitive(PRIM_GET_MEMBER_VALUE) == true) {
    SymExpr*       base      = toSymExpr (call->get(1));
    const char*    fieldName = get_string(call->get(2));

    Type*          t         = base->getValType();
    AggregateType* at        = toAggregateType(t);
    if (DecoratedClassType* dt = toDecoratedClassType(t))
      at = dt->getCanonicalClass();

    VarSymbol*     field     = toVarSymbol(at->getField(fieldName));

    if (field->isParameter() == true || field->isType() == true) {
      if (field->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE) == false) {
        if (Symbol* value = at->getSubstitution(field->name)) {
          retval = new SymExpr(value);

          call->replace(retval);
        }
      }
    }

  } else if (call->isPrimitive(PRIM_IS_SUBTYPE) ||
             call->isPrimitive(PRIM_IS_INSTANTIATION_ALLOW_VALUES) ||
             call->isPrimitive(PRIM_IS_PROPER_SUBTYPE) ||
             call->isPrimitive(PRIM_IS_COERCIBLE)) {
    SymExpr* parentExpr = toSymExpr(call->get(1));
    SymExpr* subExpr    = toSymExpr(call->get(2));

    bool parentIsType = isTypeExpr(parentExpr);
    bool subIsType = isTypeExpr(subExpr);


    if (call->isPrimitive(PRIM_IS_INSTANTIATION_ALLOW_VALUES)) {
      if (parentIsType == false && subIsType == false)
        USR_FATAL_CONT(call, "Subtype query requires a type");
    } else {
      if (parentIsType == false || subIsType == false)
        USR_FATAL_CONT(call, "Subtype queries require two types");
    }

    Type* st = subExpr->getValType();
    Type* pt = parentExpr->getValType();

    if (st->symbol->hasFlag(FLAG_DISTRIBUTION) &&
        isDistClass(canonicalClassType(pt))) {
      AggregateType* ag = toAggregateType(st);

      st = canonicalDecoratedClassType(ag->getField("_instance")->type);
    } else {
      // Try to work around some resolution order issues
      st = resolveTypeAlias(subExpr);
      pt = resolveTypeAlias(parentExpr);
    }

    if (classesWithSameKind(st, pt)) {
      st = canonicalClassType(st);
      pt = canonicalClassType(pt);
    }

    if (st                                != dtUnknown &&
        pt                                != dtUnknown &&

        st                                != dtAny     &&
        pt                                != dtAny) {

      bool result = false;
      if (call->isPrimitive(PRIM_IS_COERCIBLE))
        result = isCoercibleOrInstantiation(st, pt, call);
      else if (call->isPrimitive(PRIM_IS_INSTANTIATION_ALLOW_VALUES))
        result = isSameTypeOrInstantiation(st, pt, call);
      else
        result = isSubtypeOrInstantiation(st, pt, call);

      if (call->isPrimitive(PRIM_IS_PROPER_SUBTYPE))
        result = result && (st != pt);

      if (result == true) {
        retval = new SymExpr(gTrue);

      } else {
        retval = new SymExpr(gFalse);
      }

      call->replace(retval);

    } else {
      USR_FATAL(call,
                "Unable to perform subtype query: %s <= %s",
                st->symbol->name,
                pt->symbol->name);
    }

  } else if (call->isPrimitive(PRIM_CAST) == true) {
    Type* t = call->get(1)->typeInfo();

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
      } else if (lhs->symbol()->type == dtBytes) {
        retval = new SymExpr(new_BytesSymbol(astr(lstr, rstr)));
      } else {
        retval = new SymExpr(new_CStringSymbol(astr(lstr, rstr)));
      }

      call->replace(retval);
    }

  } else if (call->isPrimitive("string_length_bytes") == true) {
    SymExpr* se = toSymExpr(call->get(1));

    INT_ASSERT(se);

    if (se->symbol()->isParameter() == true) {
      const char* str     = get_string(se);
      const size_t nbytes = unescapeString(str, se).length();

      retval = new SymExpr(new_IntSymbol(nbytes, INT_SIZE_DEFAULT));

      call->replace(retval);
    }

  } else if (call->isPrimitive("string_length_codepoints") == true) {
    SymExpr* se = toSymExpr(call->get(1));

    INT_ASSERT(se && se->symbol()->isParameter());

    const char* str         = get_string(se);
    const std::string unesc = unescapeString(str, se);
    const size_t nbytes     = unesc.length();

    // Don't bother looking at the first byte.
    // Count it as an initial UTF-8 byte.
    size_t ncodepoints  = (nbytes > 0);
    for (size_t i = 1; i < nbytes; ++i)
      if (isInitialUTF8Byte(unesc[i]))
        ++ncodepoints;

    retval = new SymExpr(new_IntSymbol(ncodepoints, INT_SIZE_DEFAULT));

    call->replace(retval);

  } else if (call->isPrimitive("ascii") == true) {
    // This primitive is used from two places in the module code.
    //
    // One place is in CString, which calls it with exactly one
    // argument.  That argument may or may not be a param.  If it is
    // not, the code here will do nothing with it, and it will become
    // a call to the runtime routine ascii().
    //
    // The above usage is deprecated, but the deprecation is handled
    // elsewhere.
    //
    // The other place is in String, which calls it with either one or
    // two arguments, which are always params.
    //
    // All tests for user errors involving out-of-bounds accesses are
    // done in the module code so that the line number of the error
    // message will be more useful.  Therefore, bounds checks are not
    // done here.
    //
    // After the deprecated cases are removed, this code should assert
    // that the first argument is a param instead of just testing.
    SymExpr* se = toSymExpr(call->get(1));

    INT_ASSERT(se);

    if (se->symbol()->isParameter() == true) {
      const char*       str       = get_string(se);
      const std::string unescaped = unescapeString(str, se);
      size_t            idx       = 0;

      if (call->numActuals() > 1) {
        SymExpr* ie = toSymExpr(call->get(2));
        int64_t val = 0;

        INT_ASSERT(ie && ie->symbol()->isParameter());
        bool found_int = get_int(ie, &val);
        INT_ASSERT(found_int);

        idx = static_cast<size_t>(val) - 1;
      }

      retval = new SymExpr(new_UIntSymbol((int)unescaped[idx], INT_SIZE_8));

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
      processStringInRequireStmt(str, false, call->astloc.filename);

    } else {
      USR_FATAL(call, "'require' statements require string arguments");
    }

    retval = new CallExpr(PRIM_NOOP);

    call->replace(retval);

  } else if (strncmp(call->primitive->name, "_fscan", 6)        == 0    ||
             strcmp (call->primitive->name, "_readToEndOfLine") == 0    ||
             strcmp (call->primitive->name, "_now_timer")       == 0)   {
    for_actuals(actual, call) {
      insertValueTemp(call->getStmtExpr(), actual);
    }
  }

  return retval;
}

// This function implements PRIM_IS_INSTANTIATION_ALLOW_VALUES
static bool isSameTypeOrInstantiation(Type* sub, Type* super, Expr* ctx) {

  if (sub == super)
    return true;

  // Consider instantiation
  if (super->symbol->hasFlag(FLAG_GENERIC)) {
    super = getInstantiationType(sub, NULL, super, NULL, ctx);
    if (sub == super)
      return true;
  }

  return false;
}

// This function implements PRIM_IS_SUBTYPE
bool isSubtypeOrInstantiation(Type* sub, Type* super, Expr* ctx) {

  // Consider instantiation
  if (super->symbol->hasFlag(FLAG_GENERIC))
    super = getInstantiationType(sub, NULL, super, NULL, ctx);

  bool promotes = false;
  bool dispatch = false;

  if (sub && super) {
    dispatch = sub == super ||
               canCoerceAsSubtype(sub, NULL, super, NULL, NULL, &promotes);
  }

  return dispatch && !promotes;
}

// This function implements PRIM_IS_COERCIBLE
bool isCoercibleOrInstantiation(Type* sub, Type* super, Expr* ctx) {

  // Consider instantiation
  if (super->symbol->hasFlag(FLAG_GENERIC))
    super = getInstantiationType(sub, NULL, super, NULL, ctx);

  bool promotes = false;
  bool dispatch = false;

  if (sub && super)
    dispatch = canDispatch(sub, NULL, super, NULL, NULL, &promotes);

  return dispatch && !promotes;
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

      if (fn != NULL && fn->name == astrSassign && fn->retType == dtVoid) {
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

      while (paramMap.get(rhsSym) != NULL) {
        rhsSym = paramMap.get(rhsSym);
      }
      if (rhsSym->isImmediate() == true ||
          isEnumSymbol(rhsSym)  == true ||
          rhsSym == gUninstantiated) {
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
            bool failedCoercion = false;
            if (SymExpr* rhsSe = toSymExpr(call->get(2)))
              if (SymExpr* se = rhsSe->symbol()->getSingleDef())
                if (CallExpr* p = toCallExpr(se->parentExpr))
                  if (p->isPrimitive(PRIM_MOVE))
                    if (CallExpr* rhsCall = toCallExpr(p->get(2)))
                      if (rhsCall->isPrimitive(PRIM_COERCE))
                        failedCoercion = true;

            if (failedCoercion)
              USR_FATAL_CONT(call,
                             "Could not coerce param into requested type");
            else
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

    } else if (rhs->isPrimitive(PRIM_DEREF)            == true) {
      isTypeVar = isTypeExpr(rhs->get(1));

    } else if (rhs->isPrimitive(PRIM_TYPEOF)           == true) {
      isTypeVar = true;

    } else if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE) == true) {
      isTypeVar = isTypeExpr(rhs);
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
      lhsSym->addFlag(FLAG_INSERT_AUTO_DESTROY);
    }

    if (isReferenceType(lhsSym->type)                          == true  ||
        lhsSym->type->symbol->hasFlag(FLAG_REF_ITERATOR_CLASS) == true  ||
        lhsSym->type->symbol->hasFlag(FLAG_ARRAY)              == true) {
      lhsSym->removeFlag(FLAG_EXPR_TEMP);
    }

  } else {
    INT_ASSERT(false);
  }
}

bool requiresImplicitDestroy(CallExpr* call) {
  bool retval = false;

  if (FnSymbol* fn = call->resolvedFunction()) {

    if (isRecord(fn->retType)                                 == true  &&
        fn->hasFlag(FLAG_NO_IMPLICIT_COPY)                    == false &&
        fn->isIterator()                                      == false &&
        fn->retType->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE) == false &&
        fn->hasFlag(FLAG_AUTO_II)                             == false &&
        fn->name != astrSassign                                        &&
        fn->name != astr_defaultOf) {
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
      if (call->isPrimitive(PRIM_MOVE) || call->isNamedAstr(astrSassign)) {
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

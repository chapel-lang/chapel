/*
 * Copyright 2004-2017 Cray Inc.
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

static void foldEnumOp(int         op,
                       EnumSymbol* e1,
                       EnumSymbol* e2,
                       Immediate*  imm);
static bool isSubTypeOrInstantiation(Type* sub, Type* super);
static void insertValueTemp(Expr* insertPoint, Expr* actual);

#define FOLD_CALL1(prim)                                                \
  if (SymExpr* sym = toSymExpr(call->get(1))) {                         \
    if (VarSymbol* lhs = toVarSymbol(sym->symbol())) {                  \
      if (lhs->immediate) {                                             \
        Immediate i3;                                                   \
                                                                        \
        fold_constant(prim, lhs->immediate, NULL, &i3);                 \
                                                                        \
        result = new SymExpr(new_ImmediateSymbol(&i3));                 \
                                                                        \
        call->replace(result);                                          \
      }                                                                 \
    }                                                                   \
  }

#define FOLD_CALL2(prim)                                                \
  if (SymExpr* sym = toSymExpr(call->get(1))) {                         \
    if (VarSymbol* lhs = toVarSymbol(sym->symbol())) {                  \
      if (lhs->immediate) {                                             \
        if (SymExpr* sym = toSymExpr(call->get(2))) {                   \
          if (VarSymbol* rhs = toVarSymbol(sym->symbol())) {            \
            if (rhs->immediate) {                                       \
              Immediate i3;                                             \
                                                                        \
              fold_constant(prim, lhs->immediate, rhs->immediate, &i3); \
                                                                        \
              result = new SymExpr(new_ImmediateSymbol(&i3));           \
                                                                        \
              call->replace(result);                                    \
            }                                                           \
          }                                                             \
        }                                                               \
      }                                                                 \
                                                                        \
    } else if (EnumSymbol* lhs = toEnumSymbol(sym->symbol())) {         \
      if (SymExpr* sym = toSymExpr(call->get(2))) {                     \
        if (EnumSymbol* rhs = toEnumSymbol(sym->symbol())) {            \
          Immediate imm;                                                \
                                                                        \
          foldEnumOp(prim, lhs, rhs, &imm);                             \
                                                                        \
          result = new SymExpr(new_ImmediateSymbol(&imm));              \
                                                                        \
          call->replace(result);                                        \
        }                                                               \
      }                                                                 \
    }                                                                   \
  }


Expr* postFold(Expr* expr) {
  Expr* result = expr;

  if (!expr->parentSymbol)
    return result;

  SET_LINENO(expr);

  if (CallExpr* call = toCallExpr(expr)) {
    if (FnSymbol* fn = call->resolvedFunction()) {
      if (fn->retTag == RET_PARAM || fn->hasFlag(FLAG_MAYBE_PARAM)) {
        VarSymbol* ret = toVarSymbol(fn->getReturnSymbol());
        if (ret && ret->immediate) {
          result = new SymExpr(ret);
          expr->replace(result);
        } else if (EnumSymbol* es = toEnumSymbol(fn->getReturnSymbol())) {
          result = new SymExpr(es);
          expr->replace(result);
        } else if (ret == gVoid) {
          result = new SymExpr(gVoid);
          expr->replace(result);
        }
      }
      if (fn->hasFlag(FLAG_MAYBE_TYPE) && fn->getReturnSymbol()->hasFlag(FLAG_TYPE_VARIABLE))
        fn->retTag = RET_TYPE;
      if (fn->retTag == RET_TYPE) {
        Symbol* ret = fn->getReturnSymbol();
        if (!ret->type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
          result = new SymExpr(ret->type->symbol);
          expr->replace(result);
        }
      }
      if (call->isNamed("=")) {
        if (SymExpr* lhs = toSymExpr(call->get(1))) {
          if (lhs->symbol()->hasFlag(FLAG_MAYBE_PARAM) || lhs->symbol()->isParameter()) {
            if (paramMap.get(lhs->symbol())) {
              USR_FATAL(call, "parameter set multiple times");
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIM_QUERY_TYPE_FIELD) ||
               call->isPrimitive(PRIM_QUERY_PARAM_FIELD)) {
      SymExpr* classWrap = toSymExpr(call->get(1));
      // Really should be a symExpr
      INT_ASSERT(classWrap);
      AggregateType* ct = toAggregateType(classWrap->symbol()->type);
      if (!ct) {
        USR_FATAL(call, "Attempted to obtain field of a type that was not a record or class");
      }
      const char* memberName = get_string(call->get(2));

      // Finds the field matching the specified name.
      Vec<Symbol *> keys;
      ct->substitutions.get_keys(keys);
      forv_Vec(Symbol, key, keys) {
        if (!strcmp(memberName, key->name)) {
          // If there is a substitution for it, replace this call with that
          // substitution
          if (Symbol* value = ct->substitutions.get(key)) {
              result = new SymExpr(value);
              expr->replace(result);
          }
        }
      }
    }
    // param initialization should not involve PRIM_ASSIGN or "=".
    else if (call->isPrimitive(PRIM_MOVE)) {
      bool set = false;
      if (SymExpr* lhs = toSymExpr(call->get(1))) {
        if (lhs->symbol()->hasFlag(FLAG_MAYBE_PARAM) || lhs->symbol()->isParameter()) {
          if (paramMap.get(lhs->symbol()))
            INT_FATAL(call, "parameter set multiple times");
          VarSymbol* lhsVar = toVarSymbol(lhs->symbol());
          // We are expecting the LHS to be a var (what else could it be? )
          if (lhsVar->immediate) {
            // The value of the LHS of this move has already been
            // established, most likely through a construct like
            // if (cond) return x;
            // return y;
            // In this case, the first 'true' conditional that hits a return
            // can fast-forward to the end of the routine, and some
            // resolution time can be saved.
            // Re-enable the fatal error to catch this case; the correct
            // solution is to ensure that the containing expression is never
            // resolved, using the abbreviated resolution suggested above.
            // INT_ASSERT(!lhsVar->immediate);
            set = true; // That is, set previously.
          } else {
            if (SymExpr* rhs = toSymExpr(call->get(2))) {
              if (VarSymbol* rhsVar = toVarSymbol(rhs->symbol())) {
                if (rhsVar->immediate) {
                  paramMap.put(lhs->symbol(), rhsVar);
                  lhs->symbol()->defPoint->remove();
                  makeNoop(call);
                  set = true;
                }
              }
              if (EnumSymbol* rhsv = toEnumSymbol(rhs->symbol())) {
                paramMap.put(lhs->symbol(), rhsv);
                lhs->symbol()->defPoint->remove();
                makeNoop(call);
                set = true;
              }
            }
          }
          if (Symbol* lhsSym = lhs->symbol()) {
            if (lhsSym->isParameter()) {
              if (!lhsSym->hasFlag(FLAG_TEMP)) {
                if (!isLegalParamType(lhsSym->type)) {
                  USR_FATAL_CONT(call, "'%s' is not of a supported param type", lhsSym->name);
                } else if (!set) {
                  USR_FATAL_CONT(call, "Initializing parameter '%s' to value not known at compile time", lhsSym->name);
                  lhs->symbol()->removeFlag(FLAG_PARAM);
                }
              } else /* this is a compiler temp */ {
                if (lhsSym->hasFlag(FLAG_RVV) && !set) {
                  USR_FATAL_CONT(call, "'param' functions cannot return non-'param' values");
                }
              }
            }
          }
        }
        if (!set) {
          if (lhs->symbol()->hasFlag(FLAG_MAYBE_TYPE)) {
            // Add FLAG_TYPE_VARIABLE when relevant
            if (SymExpr* rhs = toSymExpr(call->get(2))) {
              if (rhs->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
                lhs->symbol()->addFlag(FLAG_TYPE_VARIABLE);
            } else if (CallExpr* rhs = toCallExpr(call->get(2))) {
              if (FnSymbol* fn = rhs->resolvedFunction()) {
                if (fn->retTag == RET_TYPE)
                  lhs->symbol()->addFlag(FLAG_TYPE_VARIABLE);
              } else if (rhs->isPrimitive(PRIM_DEREF)) {
                if (isTypeExpr(rhs->get(1)))
                  lhs->symbol()->addFlag(FLAG_TYPE_VARIABLE);
              }
            }
          }
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (rhs->isPrimitive(PRIM_TYPEOF)) {
              lhs->symbol()->addFlag(FLAG_TYPE_VARIABLE);
            }
            if (FnSymbol* fn = rhs->resolvedFunction()) {
              if (!strcmp(fn->name, "=") && fn->retType == dtVoid) {
                call->replace(rhs->remove());
                result = rhs;
                set = true;
              }
            }
          }
        }

        if (!set) {
          if (lhs->symbol()->hasFlag(FLAG_EXPR_TEMP) &&
              !lhs->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
            if (CallExpr* rhsCall = toCallExpr(call->get(2))) {
              if (requiresImplicitDestroy(rhsCall)) {
                // this still semes to be necessary even if
                // isUserDefinedRecord(lhs->symbol()->type) == true
                // see call-expr-tmp.chpl for example
                lhs->symbol()->addFlag(FLAG_INSERT_AUTO_COPY);
                lhs->symbol()->addFlag(FLAG_INSERT_AUTO_DESTROY);
              }
            }
          }

          if (isReferenceType(lhs->symbol()->type) ||
              lhs->symbol()->type->symbol->hasFlag(FLAG_REF_ITERATOR_CLASS) ||
              lhs->symbol()->type->symbol->hasFlag(FLAG_ARRAY))
            // Should this conditional include domains, distributions, sync and/or single?
            // TODO -- remove this? Or explain its purpose?
            lhs->symbol()->removeFlag(FLAG_EXPR_TEMP);
        }
        if (!set) {
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (rhs->isPrimitive(PRIM_NO_INIT)) {
              // If the lhs is a primitive, then we can safely just remove this
              // value.  Otherwise the type needs to be resolved a little
              // further and so this statement can't be removed until
              // resolveRecordInitializers
              if (!isAggregateType(rhs->get(1)->getValType())) {
                makeNoop(call);
              }
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIM_GET_MEMBER)) {
      Type* baseType = call->get(1)->getValType();
      const char* memberName = get_string(call->get(2));
      Symbol* sym = baseType->getField(memberName);
      SymExpr* left = toSymExpr(call->get(1));
      VarSymbol* varSym = toVarSymbol(sym);
      if (left && left->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
        result = new SymExpr(sym->type->symbol);
        call->replace(result);
      } else if (sym->isParameter() || (varSym && varSym->isType())) {
        Vec<Symbol*> keys;
        baseType->substitutions.get_keys(keys);
        forv_Vec(Symbol, key, keys) {
          if (!strcmp(sym->name, key->name)) {
            if (Symbol* value = baseType->substitutions.get(key)) {
              result = new SymExpr(value);
              call->replace(result);
            }
          }
        }
      }
    } else if (call->isPrimitive(PRIM_IS_SUBTYPE)) {
      if (isTypeExpr(call->get(1)) || isTypeExpr(call->get(2))) {
        Type* lt = call->get(2)->getValType(); // a:t cast is cast(t,a)
        Type* rt = call->get(1)->getValType();
        if (lt != dtUnknown && rt != dtUnknown && lt != dtAny &&
            rt != dtAny && !lt->symbol->hasFlag(FLAG_GENERIC)) {
          bool is_true = isSubTypeOrInstantiation(lt, rt);
          result = (is_true) ? new SymExpr(gTrue) : new SymExpr(gFalse);
          call->replace(result);
        }
      }
    } else if (call->isPrimitive(PRIM_CAST)) {
      Type* t= call->get(1)->typeInfo();
      if (t == dtUnknown)
        INT_FATAL(call, "Unable to resolve type");
      call->get(1)->replace(new SymExpr(t->symbol));
    } else if (call->isPrimitive("string_compare")) {
      SymExpr* lhs = toSymExpr(call->get(1));
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(lhs && rhs);
      if (lhs->symbol()->isParameter() && rhs->symbol()->isParameter()) {
        const char* lstr = get_string(lhs);
        const char* rstr = get_string(rhs);
        int comparison = strcmp(lstr, rstr);
        result = new SymExpr(new_IntSymbol(comparison));
        call->replace(result);
      }
    } else if (call->isPrimitive("string_concat")) {
      SymExpr* lhs = toSymExpr(call->get(1));
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(lhs && rhs);
      if (lhs->symbol()->isParameter() && rhs->symbol()->isParameter()) {
        const char* lstr = get_string(lhs);
        const char* rstr = get_string(rhs);
        if (lhs->symbol()->type == dtString)
          result = new SymExpr(new_StringSymbol(astr(lstr, rstr)));
        else
          result = new SymExpr(new_CStringSymbol(astr(lstr, rstr)));
        call->replace(result);
      }
    } else if (call->isPrimitive("string_length")) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      if (se->symbol()->isParameter()) {
        const char* str = get_string(se);
        int length = unescapeString(str, se).length();
        result = new SymExpr(new_IntSymbol(length, INT_SIZE_DEFAULT));
        call->replace(result);
      }
    } else if (call->isPrimitive("ascii")) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      if (se->symbol()->isParameter()) {
        const char* str = get_string(se);
        const std::string unescaped = unescapeString(str, se);
        result = new SymExpr(new_UIntSymbol((int)unescaped[0], INT_SIZE_8));
        call->replace(result);
      }
    } else if (call->isPrimitive("string_contains")) {
      SymExpr* lhs = toSymExpr(call->get(1));
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(lhs && rhs);
      if (lhs->symbol()->isParameter() && rhs->symbol()->isParameter()) {
        const char* lstr = get_string(lhs);
        const char* rstr = get_string(rhs);
        result = new SymExpr(strstr(lstr, rstr) ? gTrue : gFalse);
        call->replace(result);
      }
    } else if (call->isPrimitive(PRIM_UNARY_MINUS)) {
      FOLD_CALL1(P_prim_minus);
    } else if (call->isPrimitive(PRIM_UNARY_PLUS)) {
      FOLD_CALL1(P_prim_plus);
    } else if (call->isPrimitive(PRIM_UNARY_NOT)) {
      FOLD_CALL1(P_prim_not);
    } else if (call->isPrimitive(PRIM_UNARY_LNOT)) {
      FOLD_CALL1(P_prim_lnot);
    } else if (call->isPrimitive(PRIM_ADD)) {
      FOLD_CALL2(P_prim_add);
    } else if (call->isPrimitive(PRIM_SUBTRACT)) {
      FOLD_CALL2(P_prim_subtract);
    } else if (call->isPrimitive(PRIM_MULT)) {
      FOLD_CALL2(P_prim_mult);
    } else if (call->isPrimitive(PRIM_DIV)) {
      FOLD_CALL2(P_prim_div);
    } else if (call->isPrimitive(PRIM_MOD)) {
      FOLD_CALL2(P_prim_mod);
    } else if (call->isPrimitive(PRIM_EQUAL)) {
      FOLD_CALL2(P_prim_equal);
    } else if (call->isPrimitive(PRIM_NOTEQUAL)) {
      FOLD_CALL2(P_prim_notequal);
    } else if (call->isPrimitive(PRIM_LESSOREQUAL)) {
      FOLD_CALL2(P_prim_lessorequal);
    } else if (call->isPrimitive(PRIM_GREATEROREQUAL)) {
      FOLD_CALL2(P_prim_greaterorequal);
    } else if (call->isPrimitive(PRIM_LESS)) {
      FOLD_CALL2(P_prim_less);
    } else if (call->isPrimitive(PRIM_GREATER)) {
      FOLD_CALL2(P_prim_greater);
    } else if (call->isPrimitive(PRIM_AND)) {
      FOLD_CALL2(P_prim_and);
    } else if (call->isPrimitive(PRIM_OR)) {
      FOLD_CALL2(P_prim_or);
    } else if (call->isPrimitive(PRIM_XOR)) {
      FOLD_CALL2(P_prim_xor);
    } else if (call->isPrimitive(PRIM_POW)) {
      FOLD_CALL2(P_prim_pow);
    } else if (call->isPrimitive(PRIM_LSH)) {
      FOLD_CALL2(P_prim_lsh);
    } else if (call->isPrimitive(PRIM_RSH)) {
      FOLD_CALL2(P_prim_rsh);
    } else if (call->isPrimitive(PRIM_REQUIRE)) {
      Expr* arg = call->argList.only();
      const char* str;
      if (get_string(arg, &str)) {
        processStringInRequireStmt(str, false);
      } else {
        USR_FATAL(call, "'require' statements require string arguments");
      }
      result = new CallExpr(PRIM_NOOP);
      call->replace(result);
    } else if (call->isPrimitive(PRIM_ARRAY_ALLOC) ||
               (call->primitive &&
                (!strncmp("_fscan", call->primitive->name, 6) ||
                 !strcmp("_readToEndOfLine", call->primitive->name) ||
                 !strcmp("_now_timer", call->primitive->name)))) {
      //
      // these primitives require temps to dereference actuals
      //   why not do this to all primitives?
      //
      for_actuals(actual, call) {
        insertValueTemp(call->getStmtExpr(), actual);
      }
    }
  } else if (SymExpr* sym = toSymExpr(expr)) {
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
        if (call->isPrimitive(PRIM_MOVE) || call->isNamed("=")) {
          if (sym->symbol()->hasFlag(FLAG_RVV)) {
            makeNoop(call);
          }
          return result;
        }
      }

      if (sym->symbol()->type != dtUnknown && sym->symbol()->type != val->type) {
        Symbol* toSym = sym->symbol();
        if (!toSym->hasFlag(FLAG_TYPE_VARIABLE)) {
          // This assertion is here to ensure compatibility with old code.
          // Todo: remove after 1.15 release.
          Symbol* newToSym = toSym->type->symbol;
          INT_ASSERT(toSym->type       // cast machinery used this previously
                  == newToSym->type);  // now it will use this
          toSym = newToSym;
        }
        INT_ASSERT(toSym->hasFlag(FLAG_TYPE_VARIABLE));
        CallExpr* cast = createCast(val, toSym);
        sym->replace(cast);

        // see whether preFold will fold this _cast call
        Expr* prevResult = result;
        result = preFold(cast);
        if (result == cast) {
          // if it doesn't, put things back as they were:
          cast->replace(sym);
          result = prevResult;
          // and then do what we would've done if we hadn't used the cast
          sym->setSymbol(val);
        }
      } else {
        sym->setSymbol(val);
      }
    }
  }

  if (CondStmt* cond = toCondStmt(result->parentExpr)) {
    if (cond->condExpr == result) {
      if (Expr* expr = cond->foldConstantCondition()) {
        result = expr;
      } else {
        //
        // push try block
        //
        if (SymExpr* se = toSymExpr(result))
          if (se->symbol() == gTryToken)
            tryStack.add(cond);
      }
    }
  }

  //
  // pop try block and delete else
  //
  if (tryStack.n) {
    if (BlockStmt* block = toBlockStmt(result)) {
      if (tryStack.tail()->thenStmt == block) {
        tryStack.tail()->replace(block->remove());
        tryStack.pop();
      }
    }
  }

  return result;
}

static void foldEnumOp(int         op,
                       EnumSymbol* e1,
                       EnumSymbol* e2,
                       Immediate*  imm) {
  int64_t val1 = -1, val2 = -1, count = 0;
  // ^^^ This is an assumption that "long" on the compiler host is at
  // least as big as "int" on the target.  This is not guaranteed to be true.

  EnumType *type1, *type2;

  type1 = toEnumType(e1->type);
  type2 = toEnumType(e2->type);
  INT_ASSERT(type1 && type2);

  // Loop over the enum values to find the int value of e1
  for_enums(constant, type1) {
    if (!get_int(constant->init, &count)) {
      count++;
    }
    if (constant->sym == e1) {
      val1 = count;
      break;
    }
  }
  // Loop over the enum values to find the int value of e2
  count = 0;
  for_enums(constant, type2) {
    if (!get_int(constant->init, &count)) {
      count++;
    }
    if (constant->sym == e2) {
      val2 = count;
      break;
    }
  }

  // All operators on enum types result in a bool
  imm->const_kind = NUM_KIND_BOOL;
  imm->num_index = BOOL_SIZE_SYS;
  switch (op) {
    default: INT_FATAL("fold constant op not supported"); break;
    case P_prim_equal:
      imm->v_bool = val1 == val2;
      break;
    case P_prim_notequal:
      imm->v_bool = val1 != val2;
      break;
    case P_prim_less:
      imm->v_bool = val1 < val2;
      break;
    case P_prim_lessorequal:
      imm->v_bool = val1 <= val2;
      break;
    case P_prim_greater:
      imm->v_bool = val1 > val2;
      break;
    case P_prim_greaterorequal:
      imm->v_bool = val1 >= val2;
      break;
  }
}

static bool isSubTypeOrInstantiation(Type* sub, Type* super) {
  if (sub == super)
    return true;

  forv_Vec(Type, parent, sub->dispatchParents) {
    if (isSubTypeOrInstantiation(parent, super))
      return true;
  }

  if (sub->instantiatedFrom &&
      isSubTypeOrInstantiation(sub->instantiatedFrom, super))
    return true;

  return false;
}


static void insertValueTemp(Expr* insertPoint, Expr* actual) {
  if (SymExpr* se = toSymExpr(actual)) {
    if (!se->symbol()->type->refType) {
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

/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

/************************************* | **************************************
*                                                                             *
* pruneResolvedTree() -- prunes and cleans the AST                            *
* after function calls and types have been resolved                           *
*                                                                             *
************************************** | *************************************/

#include "DecoratedClassType.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "iterator.h"
#include "passes.h"
#include "resolution.h"
#include "wellknown.h"

static void clearDefaultInitFns(FnSymbol* unusedFn) {
  AggregateType* at = toAggregateType(unusedFn->retType);
  if (at) {
    // Also remove unused fns from iterator infos.
    // Ditto for iterator fn in iterator info.
    if (at->iteratorInfo) {
      IteratorInfo* ii = at->iteratorInfo;
      INT_ASSERT(at->symbol->hasEitherFlag(FLAG_ITERATOR_RECORD,
                                           FLAG_ITERATOR_CLASS));
      if (ii) {
        if (ii->iterator == unusedFn)
          ii->iterator = nullptr;
        if (ii->getIterator == unusedFn)
          ii->getIterator = nullptr;
      }
    }
  }
}


static void removeUnusedFunction(FnSymbol* unusedFn) {
  // remove the function
  unusedFn->defPoint->remove();
}


static void removeUnusedFunctions() {
  std::set<FnSymbol*> concreteWellKnownFunctionsSet;

  clearGenericWellKnownFunctions();

  std::vector<FnSymbol*> fns = getWellKnownFunctions();

  for_vector(FnSymbol, fn, fns) {
    INT_ASSERT(! fn->isGeneric());

    concreteWellKnownFunctionsSet.insert(fn);
  }

  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
    if (concreteWellKnownFunctionsSet.count(fn) == 0) {
      if (fn->defPoint->parentSymbol != stringLiteralModule) {
        if (! fn->isResolved() || fn->retTag == RET_PARAM)
        {
          std::vector<DefExpr*> defExprs;

          collectDefExprs(fn, defExprs);

          forv_Vec(DefExpr, def, defExprs) {
            if (TypeSymbol* typeSym = toTypeSymbol(def->sym)) {
              Type* refType = typeSym->type->refType;

              removeCopyFns(typeSym->type);

              if (AggregateType* at = toAggregateType(refType)) {
                removeCopyFns(at);

                at->symbol->defPoint->remove();
              }
            }
          }

          clearDefaultInitFns(fn);
          removeUnusedFunction(fn);

        } else if (fn->isResolved() && fn->retTag == RET_TYPE) {
          // BHARSH TODO: This is a way to work around the cleanup logic that
          // removes generic types from the tree. If the function was left
          // alive and returned a generic type the compiler would encounter
          // memory corruption issues. Ideally type functions could remain
          // in the AST and prevent the types they use from being removed.
          //
          // Skip if fatal errors were encountered because in such cases
          // postFold will leave type-returning function calls in the AST.
          Type* type = fn->retType;
          if (type->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE) == false &&
              type->symbol->hasFlag(FLAG_EXTERN) == false &&
              fatalErrorsEncountered() == false) {
            removeUnusedFunction(fn);
          }
        } else if (fn->isConstrainedGeneric()) {
          INT_ASSERT(fn->firstSymExpr() == nullptr);  // these better be unused
          removeUnusedFunction(fn);
        }
      }
    }
  }
}

static CallExpr* replaceRuntimeTypeGetField(CallExpr* call) {
  SymExpr* rt = toSymExpr(call->get(1));

  INT_ASSERT(rt->typeInfo()->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE));

  Symbol* field = getPrimGetRuntimeTypeField_Field(call);
  INT_ASSERT(field);
  SET_LINENO(call);
  CallExpr* ret = new CallExpr(PRIM_GET_MEMBER_VALUE, rt->remove(), field);
  call->replace(ret);
  return ret;
}

static void removeRandomPrimitive(CallExpr* call) {

  if (call->isPrimitive(PRIM_GET_RUNTIME_TYPE_FIELD))
    call = replaceRuntimeTypeGetField(call);

  switch (call->primitive->tag)
  {
    default: /* do nothing */ break;

    case PRIM_NOOP:
      call->remove();
      break;

    case PRIM_TYPEOF:
    {
      // Remove move(x, PRIM_TYPEOF(y)) calls -- useless after this
      CallExpr* parentCall = toCallExpr(call->parentExpr);
      if (parentCall && parentCall->isPrimitive(PRIM_MOVE) &&
          parentCall->get(2) == call) {
        parentCall->remove();
      } else {
        // Replace PRIM_TYPEOF with argument
        call->replace(call->get(1)->remove());
      }
    }
    break;

    case PRIM_CAST:
      // Remove trivial casts.
      if (call->get(1)->typeInfo() == call->get(2)->typeInfo())
        call->replace(call->get(2)->remove());
       break;

    case PRIM_SET_MEMBER:
    case PRIM_GET_MEMBER:
    case PRIM_GET_MEMBER_VALUE:
    {
      // Remove member accesses of types
      // Replace string literals with field symbols in member primitives
      Type* baseType = call->get(1)->typeInfo();

      if (!call->parentSymbol->hasFlag(FLAG_REF) &&
          baseType->symbol->hasFlag(FLAG_REF))
        baseType = baseType->getValType();

      baseType = canonicalDecoratedClassType(baseType);

      SymExpr* memberSE = toSymExpr(call->get(2));
      const char* memberName = nullptr;
      Symbol* sym = nullptr;  // the member symbol

      if (get_string(memberSE, &memberName)) {
        sym = baseType->getField(memberName);
        SET_LINENO(memberSE);
        memberSE->replace(new SymExpr(sym));
      } else {
        // Confirm that this is already a correct field Symbol.
        sym = memberSE->symbol();
        // This used to check for type equality, is this wrong?
        INT_ASSERT(isSubtypeOrInstantiation(baseType,
                                            sym->defPoint->parentSymbol->type,
                                            call));

      }

      if (sym->hasFlag(FLAG_TYPE_VARIABLE) ||
          sym->isParameter())
        call->getStmtExpr()->remove();
    }
    break;

    case PRIM_MOVE:
    {
      // Remove types to enable --baseline
      SymExpr* se = toSymExpr(call->get(2));
      if (se && se->symbol()) {
        Symbol* sym = se->symbol();
        if (isTypeSymbol(sym) || sym->hasFlag(FLAG_TYPE_VARIABLE))
          call->remove();
      }
    }
    break;

    case PRIM_WARNING:
    case PRIM_ERROR:
    {
      // Warnings have now been issued, no need to keep the function around.
      // Remove calls to compilerWarning and let dead code elimination handle
      // the rest.
      call->remove();
    }
    break;

  }
}

static void removeRandomPrimitives() {
  for_alive_in_Vec(CallExpr, call, gCallExprs)
    if (call->isPrimitive())
      removeRandomPrimitive(call);
}


// remove ASTs that supported CG (constrained generics / interfaces)
// see also cleanupGenericStandins()
static void cleanupConstrainedGenerics() {
  // This should be done before removing InterfaceSymbols
  // so we can get at and remove refTypes.
  for_alive_in_Vec(ConstrainedType, ct, gConstrainedTypes) {
    ct->symbol->defPoint->remove();
    if (Type* ctRef = ct->refType)
    {
      //INT_FATAL("CG case"); // used for testing
      if (ctRef->symbol->defPoint->inTree())
        ctRef->symbol->defPoint->remove();
    }
  }

  for_alive_in_Vec(InterfaceSymbol, isym, gInterfaceSymbols)
    isym->defPoint->remove();

  for_alive_in_Vec(ImplementsStmt, istm, gImplementsStmts) {
    FnSymbol* wrapFn = wrapperFnForImplementsStmt(istm);
    INT_ASSERT(wrapFn->hasFlag(FLAG_IMPLEMENTS_WRAPPER));
    Expr* wrapFnDef = wrapFn->defPoint;
    // We will remove istm and wrapFn. Preserve the implementation fns.
    for_alist(impl, istm->implBody->body)
      wrapFnDef->insertBefore(impl->remove());
    wrapFnDef->remove();
  }
}


static void replaceTypeArgsWithFormalTypeTemps() {
  compute_call_sites();

  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
    if (! fn->isResolved())
      // Don't bother with unresolved functions.
      // They will be removed from the tree.
      continue;

    // We do not remove type args from extern functions so that e.g.:
    //  extern proc sizeof(type t);
    //  sizeof(int)
    // will function correctly.
    // However, in such a case, we'd like to pass the type symbol
    // to the call site rather than a type variable call_tmp
    if (fn->hasFlag(FLAG_EXTERN)) {
      // Replace the corresponding actual with a SymExpr TypeSymbol
      // for all the call sites.
      forv_Vec(CallExpr, call, *fn->calledBy)
      {
        for_formals_actuals(formal, actual, call)
        {
          if (! formal->hasFlag(FLAG_TYPE_VARIABLE))
            continue;

          if (SymExpr* se = toSymExpr(actual)) {
            if (isTypeSymbol(se->symbol()))
              continue;
            if (se->symbol()->hasFlag(FLAG_EXTERN) &&
                se->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
              continue;
          }

          SET_LINENO(actual);
          TypeSymbol* ts = formal->type->symbol;
          actual->replace(new SymExpr(ts));
        }
      }
      continue;
    }

    for_formals(formal, fn)
    {
      // We are only interested in type formals
      if (! formal->hasFlag(FLAG_TYPE_VARIABLE))
        continue;

      // Replace the formal with a _formal_type_tmp_.
      SET_LINENO(formal);
      VarSymbol* tmp = newTemp("_formal_type_tmp_", formal->type);
      fn->insertAtHead(new DefExpr(tmp));
      subSymbol(fn, formal, tmp);

      // Remove the corresponding actual from all call sites.
      forv_Vec(CallExpr, call, *fn->calledBy)
      {
        for_formals_actuals(cf, ca, call)
        {
          if (cf == formal)
          {
            ca->remove();
            break;
          }
        }
      }
      formal->defPoint->remove();
      //
      // If we're removing the formal representing 'this' (if it's a
      // type, say), we need to nullify the 'this' pointer in the
      // function as well to avoid assumptions that it's legal later.
      //
      if (formal == fn->_this) {
        fn->_this = nullptr;
      }
    }
  }
}


// Remove the method token, parameter and type arguments
// from function signatures and corresponding calls.
static void removeParamArgs() {
  compute_call_sites();

  for_alive_in_Vec(FnSymbol, fn, gFnSymbols)
  {
    if (! fn->isResolved())
      // Don't bother with unresolved functions.
      // They will be removed from the tree.
      continue;

    for_formals(formal, fn)
    {
      if (formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
          formal->type == dtMethodToken)
      {
        // Remove the argument from the call site.
        for_alive_in_Vec(CallExpr, call, *fn->calledBy)
        {
          // Performance note: AList::get(int) also performs a linear search.
          for_formals_actuals(cf, ca, call)
          {
            if (cf == formal)
            {
              ca->remove();
              break;
            }
          }
        }
        formal->defPoint->remove();
      }
    }
  }
}


static void removeAggTypeFieldInfo() {
  for_alive_in_Vec(AggregateType, at, gAggregateTypes) {
      // Defined an initializer (so we left its init
      // and exprType information in the tree)
      for_fields(field, at) {
        if (field->defPoint->exprType) {
          field->defPoint->exprType->remove();
        }

        if (field->defPoint->init) {
          field->defPoint->init->remove();
        }
      }
  }
}


// Remove module level variables if they are not defined or used
// With the exception of variables that are defined in the rootModule
static void removeUnusedModuleVariables() {
  forv_Vec(DefExpr, def, gDefExprs) {
    if (VarSymbol* var = toVarSymbol(def->sym)) {
      if (ModuleSymbol* module = toModuleSymbol(def->parentSymbol)) {
        if (var->isDefined() == false && var->isUsed() == false) {
          if (module != rootModule) {
            def->remove();
          }
        }
      }
    }
  }
}


static bool do_isUnusedClass(Type* t, const std::set<Type*>& wellknown) {
  bool retval = true;

  AggregateType* at = toAggregateType(t);

  // Special case for global types.
  if (t->symbol->hasFlag(FLAG_GLOBAL_TYPE_SYMBOL)) {
    retval = false;

  // Runtime types are assumed to be always used.
  } else if (t->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
    retval = false;

  // Uses of iterator records get inserted in lowerIterators
  } else if (t->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    retval = false;

  // FALSE if iterator class's getIterator is used
  // (this case may not be necessary)
  } else if (t->symbol->hasFlag(FLAG_ITERATOR_CLASS) &&
             at && at->iteratorInfo->getIterator->isResolved()) {
    retval = false;

  } else if (at && at->resolveStatus == RESOLVED) {
    retval = false;

  // FALSE if the type uses an initializer and that initializer was
  // resolved
  } else if (at && at->initializerResolved) {
    retval = false;

  } else if (wellknown.count(at) != 0) {
    retval = false;

  } else if (at) {
    forv_Vec(AggregateType, childClass, at->dispatchChildren) {
      if (childClass && isUnusedClass(childClass, wellknown) == false) {
        retval = false;
        break;
      }
    }
  }

  return retval;
}

std::set<Type*> getWellKnownTypesSet() {
  std::set<Type*> concreteWellKnownTypesSet;
  std::vector<Type*> wellKnownTypes= getWellKnownTypes();

  for_vector(Type, type, wellKnownTypes) {
    AggregateType* at = toAggregateType(type);
    if (at == nullptr || at->isGeneric() == false)
      concreteWellKnownTypesSet.insert(type);
  }
  return concreteWellKnownTypesSet;
}

bool isUnusedClass(Type* t, const std::set<Type*>& wellknown) {
  bool retval = true;

  retval = do_isUnusedClass(t, wellknown);

  // check other variant
  //  borrow/class types can have unmanaged class type used
  //  unmanaged class types can have borrow/canonical class type used
  if (AggregateType* at = toAggregateType(t)) {
    if (isClass(at)) {
      for (int i = 0; i < NUM_DECORATED_CLASS_TYPES; i++) {
        ClassTypeDecorator decorator = (ClassTypeDecorator)i;
        if (Type* dt = at->getDecoratedClass(decorator))
          retval &= do_isUnusedClass(dt, wellknown);
      }
    }
  } else if (DecoratedClassType* dt = toDecoratedClassType(t)) {
    retval &= do_isUnusedClass(dt->getCanonicalClass(), wellknown);
  }

  return retval;
}

static void removeUnusedTypes() {

  clearGenericWellKnownTypes();
  std::set<Type*> wellknown = getWellKnownTypesSet();

  // Remove unused aggregate types.
  for_alive_in_Vec(TypeSymbol, type, gTypeSymbols) {
    if (! type->hasFlag(FLAG_REF)                &&
        ! type->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
      if (AggregateType* at = toAggregateType(type->type)) {
        if (isUnusedClass(at, wellknown)) {
          at->symbol->defPoint->remove();
        }
      } else if(DecoratedClassType* dt = toDecoratedClassType(type->type)) {
        if (isUnusedClass(dt->getCanonicalClass(), wellknown)) {
          dt->symbol->defPoint->remove();
        }
      }
    }
  }

  // Remove unused ref types.
  for_alive_in_Vec(TypeSymbol, type, gTypeSymbols) {
    if (type->hasFlag(FLAG_REF)) {
      // Get the value type of the ref type.
      if (AggregateType* at1 = toAggregateType(type->getValType())) {
        if (isUnusedClass(at1, wellknown)) {
          // If the value type is unused, its ref type can also be removed.
          type->defPoint->remove();
        }
      } else if(DecoratedClassType* dt =
                toDecoratedClassType(type->getValType())) {
        if (isUnusedClass(dt->getCanonicalClass(), wellknown)) {
          type->defPoint->remove();
        }
      }
    }
  }
}


static void removeActualNames() {
  for_alive_in_Vec(NamedExpr, named, gNamedExprs)
  {
    Expr* actual = named->actual;
    actual->remove();
    named->replace(actual);
  }
}


static void removeFormalTypeAndInitBlocks() {
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
      for_formals(formal, fn) {
        // Remove formal default values
        if (formal->defaultExpr)
          formal->defaultExpr->remove();
        // Remove formal type expressions
        if (formal->typeExpr)
          formal->typeExpr->remove();
      }
  }
}


//
// Remove the ref types and the autocopy/autodestroy functions
// for the types that are about to be removed when removing 'block'.
//
// Todo: we probably want to do this for all types that have been
// deleted, as a cleanup function at the end of resolve().
//
static void removeRefAndAutoCopyForDeadTypes(BlockStmt* block) {
  for_alist(stmt, block->body) {
    if (DefExpr* def = toDefExpr(stmt)) {
      Symbol* sym = def->sym;
      // Check whether this is the definition of a type.
      if (sym->hasFlag(FLAG_TYPE_VARIABLE) && sym->type->symbol == sym)
      {
        if (Type* ref = sym->type->getRefType())
          ref->symbol->defPoint->remove();

        removeCopyFns(sym->type);
      }
    }
  }
}

static void removeTypeBlocks() {
  for_alive_in_Vec(BlockStmt, block, gBlockStmts) {
    // Remove type blocks--code that exists only to determine types
    if (block->blockTag & BLOCK_TYPE_ONLY)
    {
      removeRefAndAutoCopyForDeadTypes(block);
      block->remove();
    }
  }
}

// Remove moot parts of AST for type a=b
static void removeTypedefParts() {
  for_alive_in_Vec(DefExpr, def, gDefExprs)
  {
    if (def->init &&
        (def->sym->hasFlag(FLAG_TYPE_VARIABLE) ||
         def->sym->type->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE))) {
      def->init->remove();
      def->init = nullptr;
    }

    // Also remove DefExprs for generic type variables
    if (!isPrimitiveType(def->sym->type) &&
        def->sym->hasFlag(FLAG_TYPE_VARIABLE) &&
        def->sym->type->symbol->hasFlag(FLAG_GENERIC)) {
      bool removeIt = true;
      if (TypeSymbol* ts = toTypeSymbol(def->sym)) {
        if (DecoratedClassType* dt = toDecoratedClassType(ts->type)) {
          ClassTypeDecorator d = dt->getDecorator();
          if ((isDecoratorUnknownNilability(d) ||
              isDecoratorUnknownManagement(d)) &&
              dt->getCanonicalClass()->inTree()) {
            // After resolution, can't consider it generic anymore...
            // The generic-ness will be moot though because later
            // it will all be replaced with the AggregateType.
            ts->removeFlag(FLAG_GENERIC);
            removeIt = false;
          }
        }
      }
      if (removeIt)
        def->remove();
    }
  }
}

static void removeWhereClausesAndReturnTypeBlocks() {
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->where)
      fn->where->remove();

    if (fn->retExprType) {
      // First, move any defs in the return type block out
      // (e.g. an array return type creates forall-expr fns)
      for_alist(expr, fn->retExprType->body) {
        if (DefExpr* def = toDefExpr(expr)) {
          if (isFnSymbol(def->sym)) {
            def->remove();
            fn->defPoint->insertBefore(def);
          }
        }
      }
      // Now remove the type block
      fn->retExprType->remove();
    }
  }
}

static void removeMootFields() {
  // Remove type fields and parameter fields
  for_alive_in_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (AggregateType* at = toAggregateType(ts->type)) {
      for_fields(field, at) {
        if (field->hasFlag(FLAG_TYPE_VARIABLE) ||
            field->isParameter())
          field->defPoint->remove();
      }
    }
  }
}

//
// If a symbol's type is not live, we can't use it.
// Remove it if VarSymbol; remove containing fn if ArgSymbol.
// Hopefully they are not used.
//
static void removeSymbolsWithRemovedTypes() {
  std::vector<Symbol*> removedSyms; // for verification

  for_alive_in_Vec(ArgSymbol, arg, gArgSymbols) {
    if (! arg->type->inTree()) {
      if (FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol)) {
        removedSyms.push_back(fn);
        fn->defPoint->remove();
      }
    }
  }

  for_alive_in_Vec(VarSymbol, var, gVarSymbols) {
    if (! var->type->inTree()) {
      removedSyms.push_back(var);
      var->defPoint->remove();
    }
  }

  // Running the asserts after the above guards against potential false alarms
  // when an earlier-removed Fn/VarSymbol refers to a later-removed one.
  for_vector(Symbol, sym, removedSyms)
    INT_ASSERT(sym->firstSymExpr() == nullptr);  // these better be unused
}

//
// A few internal pointers may point to nodes not in tree.
// Zero out such pointers whether or not their targets are live,
// to ensure they are not looked at again.
static void cleanupAfterRemoves() {

  // Note: some pointers are already zero'd in saveGenericSubstitutions

  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    // Zero the initFn pointer if the function is now dead. Ditto deinitFn.
    if (mod->initFn && !isAlive(mod->initFn))
      mod->initFn = nullptr;
    if (mod->deinitFn && !isAlive(mod->deinitFn))
      mod->deinitFn = nullptr;
  }

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (arg->instantiatedFrom != nullptr)
      arg->addFlag(FLAG_INSTANTIATED_GENERIC);
    arg->instantiatedFrom = nullptr;
  }

  cleanupAfterTypeRemoval();
}

static bool isNothingType(Type* type) {
  if (type == nullptr) {
    return false;
  }
  if (type == dtNothing) {
    return true;
  }
  if (type->symbol->hasFlag(FLAG_REF)) {
    if (type->getField("_val", false)) {
      return isNothingType(type->getValType());
    } else {
      // The _val field has already been removed because it is
      // void or tuple of void
      return true;
    }
  }
  if (type->symbol->hasFlag(FLAG_STAR_TUPLE)) {
    Symbol* field = type->getField("x0", false);
    if (field == nullptr || field->type == dtNothing) {
      return true;
    }
  }
  return false;
}

static void cleanupNothingVarsAndFields() {
  // Remove most uses of nothing variables and fields
  for_alive_in_Vec(CallExpr, call, gCallExprs) {
     if (call->isPrimitive())
      switch (call->primitive->tag) {
      case PRIM_MOVE:
        if (isNothingType(call->get(2)->typeInfo()) ||
            call->get(2)->typeInfo() == dtNothing->refType) {
          INT_ASSERT(call->get(1)->typeInfo() == call->get(2)->typeInfo());
          // Remove moves where the rhs has type nothing. If the rhs is a
          // call to something other than a few primitives, still make
          // that call, just don't move the result into anything.
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (rhs->isPrimitive(PRIM_DEREF) ||
                rhs->isPrimitive(PRIM_GET_MEMBER) ||
                rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
              call->remove();
            } else {
              Expr* rmRhs = rhs->remove();
              call->insertBefore(rmRhs);
              call->remove();
            }
          } else if (isSymExpr(call->get(2))) {
            call->remove();
          }
        }
        break;
      case PRIM_SET_MEMBER:
        if (isNothingType(call->get(3)->typeInfo())) {
          INT_ASSERT(call->get(2)->typeInfo() == call->get(3)->typeInfo());
          // Remove set_member(a, nothing, nothing) calls
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            Expr* rmRhs = rhs->remove();
            call->insertBefore(rmRhs);
            call->remove();
          } else if (isSymExpr(call->get(2))) {
            call->remove();
          }
        }
        break;
      case PRIM_YIELD:
      case PRIM_RETURN:
        if (isNothingType(call->get(1)->typeInfo()) ||
            call->get(1)->typeInfo() == dtNothing->refType) {
          // Change functions/iterators that return/yield nothing to use the
          // global nothing value instead of a local nothing.
          if (SymExpr* ret = toSymExpr(call->get(1))) {
            if (ret->symbol() != gNone) {
              SET_LINENO(call);
              call->replace(new CallExpr(call->primitive->tag, gNone));
            }
          }
        }
        break;
      case PRIM_CALL_DESTRUCTOR:
        // Remove calls to destructors for homogeneous tuples of nothing
        if (isNothingType(call->get(1)->typeInfo())) {
          call->remove();
        }
        break;
      default:
        break;
      } // switch (call->primitive->tag)
     else
      if (FnSymbol* fn = call->resolvedFunction()) {
        bool seenNothing = false;
        // Remove actual arguments that are nothing from function calls
        for_actuals(actual, call) {
          if (isNothingType(actual->typeInfo())) {
            actual->remove();
            seenNothing = true;
          }
        }
        if (seenNothing && fn->hasFlag(FLAG_AUTO_DESTROY_FN)) {
          INT_ASSERT(call->numActuals() == 0);
          // A 0-arg call to autoDestroy would upset later passes.
          call->remove();
        }
      }
  }

  // Remove nothing formal arguments from functions.
  // Change functions that return ref(nothing) to just return nothing.
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
      for_formals(formal, fn) {
        if (isNothingType(formal->type)) {
          if (formal == fn->_this) {
            fn->_this = nullptr;
          }
          formal->defPoint->remove();
        }
      }
      if (fn->retType == dtNothing->refType ||
          isNothingType(fn->retType)) {
        fn->retType = dtNothing;
      }
      if (fn->_this) {
        if (isNothingType(fn->_this->type)) {
          fn->_this = nullptr;
        }
      }
  }

  // Set for loop index variables that are nothing to the global nothing value
  for_alive_in_Vec(BlockStmt, block, gBlockStmts) {
    if (ForLoop* loop = toForLoop(block)) {
      if (loop->indexGet() && loop->indexGet()->typeInfo() == dtNothing) {
        loop->indexGet()->setSymbol(gNone);
      }
    }
  }

  // Now that uses of nothing have been cleaned up, remove the
  // DefExprs for nothing variables.
  for_alive_in_Vec(DefExpr, def, gDefExprs) {
      if (isNothingType(def->sym->type) ||
          def->sym->type == dtNothing->refType) {
        if (VarSymbol* var = toVarSymbol(def->sym)) {
          // Avoid removing the "_val" field from refs
          // and forall statements' induction/shadow variables.
          if (! def->parentSymbol->hasFlag(FLAG_REF) &&
              ! isForallIterVarDef(def)              &&
              ! preserveShadowVar(var)               ) {
            if (var != gNone) {
              def->remove();
            }
          }
        }
      } else if (def->sym->type == dtUninstantiated &&
                 isVarSymbol(def->sym) &&
                 !def->parentSymbol->hasFlag(FLAG_REF)) {
        def->remove();
      }
  }

  adjustNothingShadowVariables();

  // Problem case introduced by postFoldNormal where a statement-level call
  // returning void can be replaced by a 'none' SymExpr. Such SymExprs will
  // be left in the tree if optimizations are disabled, and can cause codegen
  // failures later on (at least under LLVM).
  //
  // Solution: Remove SymExprs to none if the expr is at the
  // statement level.
  for_SymbolSymExprs(se, gNone) {
    if (se == se->getStmtExpr()) {
      se->remove();
    }
  }
}

void saveGenericSubstitutions() {
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->substitutions.n > 0) {

      // Generate substitutionsPostResolve which should not be generated yet
      INT_ASSERT(fn->substitutionsPostResolve.size() == 0);

      if (FnSymbol* genericFn = fn->instantiatedFrom) {
        // Construct substitutionsPostResolve
        for_formals(genericFormal, genericFn) {
          if (genericFormal->type != dtMethodToken) {
            Symbol* value = fn->substitutions.get(genericFormal);
            NameAndSymbol ns;
            ns.name = genericFormal->name;
            ns.value = value;
            ns.isParam = genericFormal->isParameter();
            ns.isType = (genericFormal->originalIntent == INTENT_TYPE);
            fn->substitutionsPostResolve.push_back(ns);
          }
        }

      } else {
        // This case is a workaround for patterns that
        // come up with compiler-generated tuple functions
        INT_ASSERT(fn->hasFlag(FLAG_INIT_TUPLE));
        form_Map(SymbolMapElem, e, fn->substitutions) {
          NameAndSymbol ns;
          ns.name = e->key->name;
          ns.value = e->value;
          ns.isParam = false;
          ns.isType = false;
          fn->substitutionsPostResolve.push_back(ns);
        }
      }

      // Clear substitutions since keys might refer to deleted AST nodes
      fn->substitutions.clear();
    }

    // Clear instantiatedFrom since it would refer to a deleted AST node
    if (fn->instantiatedFrom != nullptr) {
      fn->addFlag(FLAG_INSTANTIATED_GENERIC);

      // Clear instantiatedFrom since it would refer to a deleted AST node
      fn->instantiatedFrom = nullptr;
    }

    fn->setInstantiationPoint(nullptr);
  }

  for_alive_in_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (AggregateType* at = toAggregateType(ts->type)) {
      if (at->substitutions.n > 0) {
        // Generate substitutionsPostResolve which should not be generated yet
        INT_ASSERT(at->substitutionsPostResolve.size() == 0);
        for_fields(field, at) {
          if (Symbol* value = at->getSubstitutionWithName(field->name)) {
            NameAndSymbol ns;
            ns.name = field->name;
            ns.value = value;
            ns.isParam = field->isParameter();
            ns.isType = field->hasFlag(FLAG_TYPE_VARIABLE);
            at->substitutionsPostResolve.push_back(ns);
          }
        }
        // Clear substitutions since keys might refer to deleted AST nodes
        at->substitutions.clear();
      }

      if (at->instantiatedFrom != nullptr) {
        // Clear instantiatedFrom since it would refer to a deleted AST node
        at->instantiatedFrom = nullptr;

        ts->addFlag(FLAG_INSTANTIATED_GENERIC);
      }
    }
  }
}

void pruneResolvedTree() {
  cleanupConstrainedGenerics();

  removeTiMarks();

  removeUnusedFunctions();

  if (fRemoveUnreachableBlocks) {
    deadBlockElimination();
  }

  removeRandomPrimitives();

  replaceTypeArgsWithFormalTypeTemps();

  removeParamArgs();

  removeAggTypeFieldInfo();

  removeUnusedModuleVariables();

  removeUnusedTypes();

  removeActualNames();

  removeFormalTypeAndInitBlocks();

  removeTypeBlocks();

  removeTypedefParts();

  removeWhereClausesAndReturnTypeBlocks();

  removeMootFields();

  removeSymbolsWithRemovedTypes();

  expandInitFieldPrims();

  cleanupNothingVarsAndFields();

  cleanupAfterRemoves();
}

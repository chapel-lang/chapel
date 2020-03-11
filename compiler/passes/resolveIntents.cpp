/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "resolveIntents.h"

#include "passes.h"
#include "resolution.h"

bool intentsResolved = false;

static IntentTag constIntentForType(Type* t) {
  if (is_bool_type(t) ||
      is_int_type(t) ||
      is_uint_type(t) ||
      is_real_type(t) ||
      is_imag_type(t) ||
      is_complex_type(t) ||
      is_enum_type(t) ||
      isClass(t) ||
      isDecoratedClassType(t) ||
      t == dtOpaque ||
      t == dtTaskID ||
      t == dtFile ||
      t == dtNil ||
      t == dtStringC ||
      t == dtCVoidPtr ||
      t == dtCFnPtr ||
      t == dtNothing ||
      t == dtVoid ||
      t == dtUninstantiated ||
      t->symbol->hasFlag(FLAG_RANGE) ||
      // MPF: This rule seems odd to me
      (t->symbol->hasFlag(FLAG_EXTERN) && !isRecord(t))) {
    return INTENT_CONST_IN;

  } else if (isSyncType(t)          ||
             isSingleType(t)        ||
             isRecordWrappedType(t) ||  // domain, array, or distribution
             isManagedPtrType(t) ||
             isAtomicType(t) ||
             isUnion(t) ||
             isRecord(t)) { // may eventually want to decide based on size
    return INTENT_CONST_REF;

  }

  INT_FATAL(t, "Unhandled type in constIntentForType()");
  return INTENT_CONST;
}

// Detect tuples containing e.g. arrays by reference
// These should be const / not const element-by-element.
static
bool isTupleContainingRefMaybeConst(Type* t)
{
  AggregateType* at = toAggregateType(t);
  if (t->symbol->hasFlag(FLAG_TUPLE)) {
    for_fields(field, at) {
      Type* fieldType = field->getValType();
      if (field->isRef()) {
        if (fieldType->symbol->hasFlag(FLAG_DEFAULT_INTENT_IS_REF_MAYBE_CONST))
          return true;
      }
      if (isTupleContainingRefMaybeConst(fieldType))
        return true;
    }
  }
  return false;
}


IntentTag blankIntentForType(Type* t) {
  IntentTag retval = INTENT_BLANK;

  if (isAtomicType(t)                                ||
      t->symbol->hasFlag(FLAG_DEFAULT_INTENT_IS_REF)) {
    retval = INTENT_REF;

  } else if (t->symbol->hasFlag(FLAG_DEFAULT_INTENT_IS_REF_MAYBE_CONST)
            || isTupleContainingRefMaybeConst(t)) {
    retval = INTENT_REF_MAYBE_CONST;

  } else if (isManagedPtrType(t)) {
    // TODO: INTENT_REF_MAYBE_CONST could
    // allow blank intent owned to be transferred out of
    retval = INTENT_CONST_REF;

  } else if (is_bool_type(t)                         ||
             is_int_type(t)                          ||
             is_uint_type(t)                         ||
             is_real_type(t)                         ||
             is_imag_type(t)                         ||
             is_complex_type(t)                      ||
             is_enum_type(t)                         ||
             t == dtStringC                          ||
             t == dtCVoidPtr                         ||
             t == dtCFnPtr                           ||
             isClass(t)                              ||
             isDecoratedClassType(t)                 ||
             isRecord(t)                             ||
             // Note: isRecord(t) includes range (FLAG_RANGE)
             isUnion(t)                              ||
             t == dtTaskID                           ||
             t == dtFile                             ||
             t == dtNil                              ||
             t == dtVoid                             ||
             t == dtOpaque                           ||
             t == dtNothing                          ||
             t == dtUninstantiated                   ||
             t->symbol->hasFlag(FLAG_DOMAIN)         ||
             t->symbol->hasFlag(FLAG_DISTRIBUTION)   ||
             t->symbol->hasFlag(FLAG_EXTERN)) {
    retval = constIntentForType(t);

  } else {
    INT_FATAL(t, "Unhandled type in blankIntentForType()");
  }

  return retval;
}

IntentTag concreteIntent(IntentTag existingIntent, Type* t) {
  if (existingIntent == INTENT_BLANK || existingIntent == INTENT_CONST) {
    if (t->symbol->hasFlag(FLAG_REF)) {
      // Handle REF type
      // A formal with a ref type should always have some ref-intent.
      // This is will hopefully be a short-lived fix
      // while converting entirely over to QualifiedType.
      //
      // TODO: Are there cases where we should handle const-ref intent? Should
      // a QualifiedType be used instead of a Type* ?

      IntentTag baseIntent = INTENT_TYPE;
      Type* valType = t->getValType();
      if (existingIntent == INTENT_BLANK)
        baseIntent = blankIntentForType(valType);
      else if (existingIntent == INTENT_CONST)
        baseIntent = constIntentForType(valType);
      if ( (baseIntent & INTENT_FLAG_REF) )
        return baseIntent; // it's already REF/CONST_REF/REF_MAYBE_CONST
      else if (baseIntent == INTENT_CONST_IN)
        return INTENT_CONST_REF;
      else
        INT_ASSERT(0); // unhandled case
    }

    if (existingIntent == INTENT_BLANK) {
      return blankIntentForType(t);
    } else if (existingIntent == INTENT_CONST) {
      return constIntentForType(t);
    }
  }

  return existingIntent;
}

static IntentTag constIntentForThisArg(Type* t) {
  if (t->symbol->hasFlag(FLAG_RANGE))
    return INTENT_CONST_IN;
  else if (isRecord(t) || isUnion(t) || t->symbol->hasFlag(FLAG_REF))
    return INTENT_CONST_REF;
  else
    return INTENT_CONST_IN;
}

static IntentTag blankIntentForThisArg(Type* t) {
  // todo: be honest when 't' is an array or domain

  Type* valType = t->getValType();

  // Range default this intent is const-in
  if (valType->symbol->hasFlag(FLAG_RANGE))
    return INTENT_CONST_IN;

  // For user records or types with FLAG_DEFAULT_INTENT_IS_REF_MAYBE_CONST,
  // the intent for this is INTENT_REF_MAYBE_CONST
  //
  // This applies to both arguments of type _ref(t) and t
  if (isRecord(valType) || isUnion(valType) ||
      valType->symbol->hasFlag(FLAG_DEFAULT_INTENT_IS_REF_MAYBE_CONST))
    return INTENT_REF_MAYBE_CONST;

  if (isRecordWrappedType(t))
    // domain / distribution
    return INTENT_CONST_REF;
  else if (t->symbol->hasFlag(FLAG_REF))
    // reference
    return INTENT_REF;
  else
    return INTENT_CONST_IN;
}

static
IntentTag blankIntentForExternFnArg(Type* type) {
  if (llvmCodegen && type->getValType()->symbol->hasFlag(FLAG_C_ARRAY))
    // Pass c_array by ref by default for --llvm
    // (for C, an argument like int arg[2] is actually just int* arg).
    // This needs to be here because otherwise the following rule overrides it.
    return INTENT_REF_MAYBE_CONST;
  else
    return INTENT_CONST_IN;
}

IntentTag concreteIntentForArg(ArgSymbol* arg) {

  FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);

  if (arg->hasFlag(FLAG_ARG_THIS) && arg->intent == INTENT_BLANK)
    return blankIntentForThisArg(arg->type);
  else if (arg->hasFlag(FLAG_ARG_THIS) && arg->intent == INTENT_CONST)
    return constIntentForThisArg(arg->type);
  else if (fn->hasFlag(FLAG_EXTERN) && arg->intent == INTENT_BLANK)
    return blankIntentForExternFnArg(arg->type);
  else if (fn->hasFlag(FLAG_ALLOW_REF) && arg->type->symbol->hasFlag(FLAG_REF))

    // This is a workaround for an issue with RVF erroneously forwarding a
    // reduce variable. The workaround adjusts the build_tuple_always_allow_ref
    // call to take all _ref arguments by `ref` intent regardless of
    // the type. It would be better to rely on task/forall intents
    // to correctly mark const / not const / maybe const.
    return INTENT_REF;

  else
    return concreteIntent(arg->intent, arg->type);

}

void resolveArgIntent(ArgSymbol* arg) {
  if (!resolved) {
    if (arg->type == dtMethodToken ||
        arg->type == dtTypeDefaultToken ||
        arg->type == dtNothing ||
        arg->type == dtUnknown ||
        arg->hasFlag(FLAG_TYPE_VARIABLE) ||
        arg->hasFlag(FLAG_PARAM)) {
      return; // Leave these alone during resolution.
    }
  }

  IntentTag intent = concreteIntentForArg(arg);

  if (resolved) {
    // After resolution, change out/inout/in to ref
    // to be more accurate to the generated code.

    if (intent == INTENT_OUT ||
        intent == INTENT_INOUT) {
      // Resolution already handled out/inout copying
      intent = INTENT_REF;
    } else if (intent == INTENT_IN) {
      // MPF note: check types/range/ferguson/range-begin.chpl
      // if you try to add INTENT_CONST_IN here.
      //
      // BHARSH: Update insertWideReferences.fixTupleFormal and the following
      // test if INTENT_CONST_IN is added here:
      //   multilocale/bharshbarg/constInTuple.chpl

      // Resolution already handled copying for INTENT_IN for
      // records/unions.
      bool addedTmp = (isRecord(arg->type) || isUnion(arg->type));
      FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);
      if (fn->hasFlag(FLAG_EXTERN)) 
        // Q - should this check arg->type->symbol->hasFlag(FLAG_EXTERN)?
        addedTmp = false;
      // Pass wrappers used in libraries/interop by value.
      if (arg->type->symbol->hasFlag(FLAG_EXPORT_WRAPPER))
        addedTmp = false;
      if (addedTmp) {
        if (arg->type->symbol->hasFlag(FLAG_COPY_MUTATES) ||
            (formalRequiresTemp(arg, fn) &&
             shouldAddInFormalTempAtCallSite(arg, fn)))
          intent = INTENT_REF;
      }
      // Otherwise, leave the intent INTENT_IN so that the formal can
      // be modified in the body of the function.
    }
  }

  if (arg->intent != intent) {
    arg->originalIntent = arg->intent;
  }
  arg->intent = intent;
}

static void resolveVarIntent(VarSymbol* sym) {
  QualifiedType q = sym->qualType();
  if (q.getQual() == QUAL_UNKNOWN) {
    if (sym->isRef()) {
      sym->qual = QUAL_REF;
    } else {
      sym->qual = QUAL_VAL;
    }
    // TODO also check sym->isConstant() and set one of CONST qualifiers
  }
}

void resolveIntents() {
  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    resolveArgIntent(arg);
  }

  // BHARSH TODO: This shouldn't be necessary, but will be until we fully
  // switch over to qualified types.
  forv_Vec(VarSymbol, sym, gVarSymbols) {
    resolveVarIntent(sym);
  }
  forv_Vec(ShadowVarSymbol, sym, gShadowVarSymbols) {
    resolveVarIntent(sym);
  }

  intentsResolved = true;
}

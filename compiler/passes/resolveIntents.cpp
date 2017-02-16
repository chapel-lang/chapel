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

#include "resolveIntents.h"

#include "passes.h"

bool intentsResolved = false;

static IntentTag constIntentForType(Type* t) {
  if (isSyncType(t)          ||
      isSingleType(t)        ||
      isRecordWrappedType(t) ||  // domain, array, or distribution
      isRecord(t)) { // may eventually want to decide based on size
    return INTENT_CONST_REF;
  } else if (is_bool_type(t) ||
             is_int_type(t) ||
             is_uint_type(t) ||
             is_real_type(t) ||
             is_imag_type(t) ||
             is_complex_type(t) ||
             is_enum_type(t) ||
             isClass(t) ||
             isUnion(t) ||
             isAtomicType(t) ||
             t == dtOpaque ||
             t == dtTaskID ||
             t == dtFile ||
             t == dtNil ||
             t == dtStringC ||
             t == dtStringCopy ||
             t == dtCVoidPtr ||
             t == dtCFnPtr ||
             // TODO: t->symbol->hasFlag(FLAG_RANGE) ||
             t->symbol->hasFlag(FLAG_EXTERN)) {
    return INTENT_CONST_IN;
  }
  INT_FATAL(t, "Unhandled type in constIntentForType()");
  return INTENT_CONST;
}

IntentTag blankIntentForType(Type* t) {
  IntentTag retval = INTENT_BLANK;

  if (isSyncType(t)                                  ||
      isSingleType(t)                                ||
      isAtomicType(t)                                ||
      t->symbol->hasFlag(FLAG_DEFAULT_INTENT_IS_REF) ||
      t->symbol->hasFlag(FLAG_ARRAY)) {
    retval = INTENT_REF;

  } else if (is_bool_type(t)                         ||
             is_int_type(t)                          ||
             is_uint_type(t)                         ||
             is_real_type(t)                         ||
             is_imag_type(t)                         ||
             is_complex_type(t)                      ||
             is_enum_type(t)                         ||
             t == dtStringC                          ||
             t == dtStringCopy                       ||
             t == dtCVoidPtr                         ||
             t == dtCFnPtr                           ||
             isClass(t)                              ||
             isRecord(t)                             ||
             isUnion(t)                              ||
             t == dtTaskID                           ||
             t == dtFile                             ||
             t == dtNil                              ||
             t == dtOpaque                           ||
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
  if (existingIntent == INTENT_BLANK) {
    if (t->symbol->hasFlag(FLAG_REF)) {
      // A formal with a ref type should always have the ref-intent. No other
      // intent makes sense. This is will hopefully be a short-lived fix
      // while converting entirely over to QualifiedType.
      //
      // TODO: Are there cases where we should handle const-ref intent? Should
      // a QualifiedType be used instead of a Type* ?
      return INTENT_REF;
    } else {
      return blankIntentForType(t);
    }
  } else if (existingIntent == INTENT_CONST) {
    return constIntentForType(t);
  } else {
    return existingIntent;
  }
}

static IntentTag blankIntentForThisArg(Type* t) {
  // todo: be honest when 't' is an array or domain

  if (isRecord(t) || isUnion(t) || t->symbol->hasFlag(FLAG_REF))
    return INTENT_REF;
  else
    return INTENT_CONST_IN;
}

IntentTag concreteIntentForArg(ArgSymbol* arg) {

  if (arg->hasFlag(FLAG_ARG_THIS) && arg->intent == INTENT_BLANK)
    return blankIntentForThisArg(arg->type);
  else if (toFnSymbol(arg->defPoint->parentSymbol)->hasFlag(FLAG_EXTERN) &&
           arg->intent == INTENT_BLANK) {
    return INTENT_CONST_IN;
  }
  else
    return concreteIntent(arg->intent, arg->type);

}

void resolveArgIntent(ArgSymbol* arg) {
  if (!resolved) {
    if (arg->type == dtMethodToken ||
        arg->type == dtTypeDefaultToken ||
        arg->type == dtVoid ||
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
      // Resolution already handled in copying
      intent = constIntentForType(arg->type);
    }
  }

  arg->intent = intent;
}

void resolveIntents() {
  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    resolveArgIntent(arg);
  }

  // BHARSH TODO: This shouldn't be necessary, but will be until we fully
  // switch over to qualified types.
  forv_Vec(VarSymbol, sym, gVarSymbols) {
    QualifiedType q = sym->qualType();
    if (q.getQual() == QUAL_UNKNOWN) {
      if (sym->isRef()) {
        sym->qual = QUAL_REF;
      } else {
        sym->qual = QUAL_VAL;
      }
    }
  }

  intentsResolved = true;
}

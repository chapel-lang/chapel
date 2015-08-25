/*
 * Copyright 2004-2015 Cray Inc.
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

#include "passes.h"
#include "resolveIntents.h"

bool intentsResolved = false;

static IntentTag constIntentForType(Type* t) {
  if (isSyncType(t) ||
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
             t == dtTaskList ||
             t == dtNil ||
             t == dtStringC ||
             t == dtStringCopy ||
             t->symbol->hasFlag(FLAG_EXTERN)) {
    return INTENT_CONST_IN;
  }
  INT_FATAL(t, "Unhandled type in constIntentForType()");
  return INTENT_CONST;
}

IntentTag blankIntentForType(Type* t) {
  if (isSyncType(t) ||
      isAtomicType(t) ||
      t->symbol->hasFlag(FLAG_ARRAY)) {
    return INTENT_REF;
  } else if (is_bool_type(t) ||
             is_int_type(t) ||
             is_uint_type(t) ||
             is_real_type(t) ||
             is_imag_type(t) ||
             is_complex_type(t) ||
             is_enum_type(t) ||
             t == dtStringC ||
             t == dtStringCopy ||
             isClass(t) ||
             isRecord(t) ||
             isUnion(t) ||
             t == dtTaskID ||
             t == dtFile ||
             t == dtTaskList ||
             t == dtNil ||
             t == dtOpaque ||
             t->symbol->hasFlag(FLAG_DOMAIN) ||
             t->symbol->hasFlag(FLAG_DISTRIBUTION) ||
             t->symbol->hasFlag(FLAG_EXTERN)) {
    return constIntentForType(t);
  }
  INT_FATAL(t, "Unhandled type in blankIntentForType()");
  return INTENT_BLANK;
}

IntentTag concreteIntent(IntentTag existingIntent, Type* t) {
  if (existingIntent == INTENT_BLANK) {
    return blankIntentForType(t);
  } else if (existingIntent == INTENT_CONST) {
    return constIntentForType(t);
  } else {
    return existingIntent;
  }
}

static IntentTag blankIntentForThisArg(Type* t) {
  // todo: be honest when 't' is an array or domain
  return INTENT_CONST_IN;
}

void resolveArgIntent(ArgSymbol* arg) {
  arg->intent = 
    arg->hasFlag(FLAG_ARG_THIS) && arg->intent == INTENT_BLANK ?
    blankIntentForThisArg(arg->type) :
    concreteIntent(arg->intent, arg->type);
}

// In Chapel, there is only one reference level: there are no references to
// references (ignoring the wide case and class variables for simplicity).
// This routine adjusts the type of the argument to match the REF flag of its
// concrete intent, which in turn controls how the argument is passed in the
// generated code.
//
// Application of this function to various arguments will temporarily violate
// the invariant that the types of formal and actual arguments match.  The calls
// to insertReferenceTemps() and insertDerefTemps() will correct that.
static void adjustRefLevel(ArgSymbol* arg)
{
  if (arg->intent & INTENT_FLAG_REF)
  {
    // This conditional is only present for debugging purposes.
    // It may be removed when this change-of-type seems obvious.
    Type* t = arg->type;

    // There is only one level of referencing in Chapel, so of this arg is
    // already a ref type, we are done.
    if (t->symbol->hasFlag(FLAG_REF))
      return;

    // Type arguments are always passed by "value".
    if (arg->hasFlag(FLAG_TYPE_VARIABLE))
      return;

    // Do not do this for record-wrapped types, because somewhere else we
    // believe that RWTs should always be passed around by value(!?)
    if (isRecordWrappedType(t))
        return;

    // This argument wants to be passed by ref.
    // But is not.  Let's fix that.
    INT_ASSERT(arg->type->refType);
    arg->type = arg->type->refType;
  }
}

void resolveIntents() {
  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    resolveArgIntent(arg);
    adjustRefLevel(arg);
  }
  replaceValArgsWithRefArgs();
  insertReferenceTemps();
  insertDerefTemps();

  intentsResolved = true;
}

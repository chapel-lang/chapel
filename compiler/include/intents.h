/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#ifndef INTENTS_H
#define INTENTS_H

// TODO: Consider moving other things (like printers) to this header
// when/if we want to clean things up further.

const int INTENT_FLAG_IN          = 0x01;
const int INTENT_FLAG_OUT         = 0x02;
const int INTENT_FLAG_CONST       = 0x04;
const int INTENT_FLAG_REF         = 0x08;
const int INTENT_FLAG_PARAM       = 0x10;
const int INTENT_FLAG_TYPE        = 0x20;
const int INTENT_FLAG_BLANK       = 0x40;
const int INTENT_FLAG_MAYBE_CONST = 0x80;

// If this enum is modified, ArgSymbol::intentDescrString()
// and intentDescrString(IntentTag) should also be updated to match
enum IntentTag {
  INTENT_IN              = INTENT_FLAG_IN,
  INTENT_OUT             = INTENT_FLAG_OUT,
  INTENT_INOUT           = INTENT_FLAG_IN          | INTENT_FLAG_OUT,
  INTENT_CONST           = INTENT_FLAG_CONST,
  INTENT_CONST_IN        = INTENT_FLAG_CONST       | INTENT_FLAG_IN,
  INTENT_REF             = INTENT_FLAG_REF,
  INTENT_CONST_REF       = INTENT_FLAG_CONST       | INTENT_FLAG_REF,
  INTENT_REF_MAYBE_CONST = INTENT_FLAG_MAYBE_CONST | INTENT_FLAG_REF,
  INTENT_PARAM           = INTENT_FLAG_PARAM,
  INTENT_TYPE            = INTENT_FLAG_TYPE,
  INTENT_BLANK           = INTENT_FLAG_BLANK
};

/*
enum ForallIntentTag : task- or forall-intent tags

TFI_IN_PARENT
  The compiler adds this shadow var during resolution for each TFI_IN
  and TFI_CONST_IN. A TFI_IN_PARENT represents the task function's formal
  that the corresponding TFI_IN or TFI_CONST_IN is to be initialized from.

TFI_REDUCE
  This shadow var replaces the uses of the outer variable in the loop body
  in case of a 'reduce' intent. This is done in parsing and scopeResolve.
  It is analogous to the TFI_IN shadow var for an 'in' intent.
  A TFI_REDUCE var represents the current task's accumulation state.

TFI_REDUCE_*
  The compiler adds one each of these shadow vars during resolution
  for each TFI_REDUCE. They represent:

  TFI_REDUCE_OP        - the current task's reduction OP
  TFI_REDUCE_PARENT_AS - the parent task's Accumulation State
  TFI_REDUCE_PARENT_OP - the parent task's reduction OP

  The *PARENT* vars, like TFI_IN_PARENT, are the task function's formals.

The remaining tags should be self-explanatory.
*/
enum ForallIntentTag {
  // user-specified intents
  TFI_DEFAULT,  // aka TFI_BLANK
  TFI_CONST,                       // ShadowVarSymbol nicknames:
  TFI_IN,                          //   SI
  TFI_CONST_IN,                    //   "
  TFI_REF,                         //   SR
  TFI_CONST_REF,                   //   "
  TFI_REDUCE,                      //   AS    (for Accumulation State)
  TFI_TASK_PRIVATE,                //   TPV
  // compiler-added helpers; note isCompilerAdded()
  TFI_IN_PARENT,                   //   INP
  TFI_REDUCE_OP,                   //   RP    (for Reduce oP)
  TFI_REDUCE_PARENT_AS,            //   PAS
  TFI_REDUCE_PARENT_OP,            //   PRP
};

const char* forallIntentTagDescription(ForallIntentTag tfiTag);

enum RetTag {
  RET_VALUE,
  RET_REF,
  RET_CONST_REF,
  RET_PARAM,
  RET_TYPE
};

#endif

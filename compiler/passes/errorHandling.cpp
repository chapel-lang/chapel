/*
 * Copyright 2004-2016 Cray Inc.
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

#include "symbol.h"
#include "view.h"
#include <cstdio>

  // change 'throws' to 'out'
    // add an Error formal to a function marked 'throws'
    // for all calls to such functions
      // pass a variable to receive the Error arg
      // check the Error arg after the call

  // TODO: how do we find Error temps from a call to a throwing function?

void lowerErrorHandling(void) {
  printf("hello world\n");

  // give 'throws' an 'out'
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->throwsError()) {
      SET_LINENO(fn);

      // TODO: dtObject should be dtError, but we don't have that right now
      ArgSymbol* outFormal = new ArgSymbol(INTENT_REF, "error", dtObject);
      fn->insertFormalAtTail(outFormal);
    }
  }
}

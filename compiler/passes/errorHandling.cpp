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

#include "errorHandling.h"
#include "stmt.h"
#include "symbol.h"
#include "view.h"

// for each function that 'throws':
  // add an error_out formal
  // for each call to one of these functions:
    // create and pass a variable to receive error_out
    // check error after the call
      // halt() if needed
  // for each 'throw' in the function:
    // replace it with setting error_out

// TODO: dtObject should be dtError, but we don't have that right now
// TODO: how do we find Error temps from a call to a throwing function?

void lowerErrorHandling(void) {

}

/*
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

#include <cstring>

#include "library.h"

#include "FnSymbol.h"
#include "beautify.h"
#include "codegen.h"
#include "driver.h"
#include "expr.h"
#include "stlUtil.h"
#include "stringutil.h"

//
// TODO: Multi-Locale Interop prototype code!
//
// I decided to make a separate source file just to handle generation of the
// MLI wrapper code (this includes RPC and marshalling).
//
// --
// --
//

//
// Generate wrappers for exported functions one module at a time. This way we
// minimize the point of contact for the rest of code generation.
//
void codegenMLIWrappers(ModuleSymbol* md) {
  USR_WARN("MLI wrapper codegen hit!");

  return;
}

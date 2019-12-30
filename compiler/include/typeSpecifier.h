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

#ifndef _TYPE_SPECIFIER_H_
#define _TYPE_SPECIFIER_H_

class Expr;
class Type;

// This function attempts to determine the type for
//   1) Primitive types (parameterized and default)
//   2) Non-parameterized, i.e. non-generic, classes and records
//
// It is defined to return NULL for more general type specifiers.
//
// This function was initially defined for a simple use in preFold().
// In that use case it generated FATAL errors for invalid primitives.
//
// It was exposed to allow it to be used in earlier passes
// e.g. in limited situations within normalize.  In these
// situations param-folding will not have occurred and it
// is necessary to suppress any error messages and simply
// return NULL.
//

Type* typeForTypeSpecifier(Expr* expr, bool fatalOK);

#endif

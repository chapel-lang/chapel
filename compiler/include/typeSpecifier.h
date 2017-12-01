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

#ifndef _TYPE_SPECIFIER_H_
#define _TYPE_SPECIFIER_H_

class Expr;
class Type;

//
// Convert a type specifier expression, 'expr' into a Type* if
// possible; return NULL otherwise.
//
// fatalOK: This argument indicates whether it is reasonable for the
// routine to issue a fatal error when the type expression is found
// to be illegal (e.g., 'int(3)').  Calls early in the compiler
// working on code that may later be folded out or found to be
// unreachable should set this to 'false' to avoid issuing errors
// prematurely.
//
Type* typeForTypeSpecifier(Expr* expr, bool fatalOK = true);

#endif

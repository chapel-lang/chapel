/*
 * Copyright 2004-2018 Cray Inc.
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

/*

   Functions for finding out about fields, functions, and methods.

   .. note ::

     There are several ways in which this module could be improved upon:
     
       * the methods here might be better as type methods,
         so you could use `R.numFields()` instead of `numFields(R)`.
       * :proc:`getField` does not yet return a mutable value.
*/
module Reflection {

/* Return the number of fields in a class or record as a param.
   The count of fields includes types and param fields.
 */
proc numFields(type t) param : int
  return __primitive("num fields", t);

/* Get the name of the ith field in a class or record.
   Causes a compilation error if `i` is not in 1..numFields(t).

   :arg t: a class or record type
   :arg i: which field to get the name of
   :returns: the name of the field, as a param string
 */
proc getFieldName(type t, param i:int) param : string
  return __primitive("field num to name", t, i);

// Note, since this version has a where clause, it is preferred
// over the const ref one.
/* Get the ith field in a class or record. When the ith field is
   a `param` this overload will be chosen to return a `param`.
   Causes a compilation error if `i` is not in 1..numFields(t).

   :arg x: a class or record
   :arg i: which field to get
   :returns: the `param` that field represents
*/
proc getField(const ref x:?t, param i: int) param
  where i > 0 && i <= numFields(t) &&
        isParam(__primitive("field by num", x, i)) {

  return __primitive("field by num", x, i);
}

// Note, since this version has a where clause, it is preferred
// over the const ref one.
/* Get the ith field in a class or record. When the ith field is
   a `type` variable this overload will be chosen to return a type.
   Causes a compilation error if `i` is not in 1..numFields(t).

   :arg x: a class or record
   :arg i: which field to get
   :returns: the type that field represents
*/
proc getField(const ref x:?t, param i: int) type
  where i > 0 && i <= numFields(t) &&
        isType(__primitive("field by num", x, i)) {

  return __primitive("field by num", x, i);
}

/* Get the ith field in a class or record.
   Causes a compilation error if `i` is not in 1..numFields(t).

   :arg x: a class or record
   :arg i: which field to get
   :returns: an rvalue referring to that field.
 */
pragma "unsafe"
inline
proc getField(const ref x:?t, param i:int) const ref
  return __primitive("field by num", x, i);

/* Get a field in a class or record by name. When the named
   field is a `param` this overload will be chosen to return a
   `param`. Will generate a compilation error if a field with
   that name is not found.

   :arg x: a class or record
   :arg s: the name of a field
   :returns: the `param` that field represents
 */
proc getField(const ref x:?t, param s: string) param
  where getFieldIndex(t, s) != 0 && isParam(getField(x, getFieldIndex(t, s))) {

  return getField(x, getFieldIndex(t, s));
}

/* Get a field in a class or record by name. When the named
   field is a `type` variable this overload will be chosen to
   return a type. Will generate a compilation error if a field
   with that name is not found.

   :arg x: a class or record
   :arg s: the name of a field
   :returns: the type that field represents
 */
proc getField(const ref x:?t, param s: string) type
  where getFieldIndex(t, s) != 0 && isType(getField(x, getFieldIndex(t, s))) {

  return getField(x, getFieldIndex(t, s));
}

/* Get a field in a class or record by name.
   Will generate a compilation error if a field with that name
   is not found.

   :arg x: a class or record
   :arg s: the name of a field
   :returns: an rvalue referring to that field.
 */
proc getField(const ref x:?t, param s:string) const ref {
  param i = __primitive("field name to num", t, s);
  if i == 0 then
    compilerError("field ", s, " not found in ", t:string);
  return __primitive("field by num", x, i);
}

/* Get a mutable ref to the ith field in a class or record.
   Causes a compilation error if `i` is not in 1..numFields(t)
   or if the argument is not mutable.

   :arg x: a class or record
   :arg i: which field to get
   :returns: an rvalue referring to that field.
 */
pragma "unsafe"
inline
proc getFieldRef(ref x:?t, param i:int) ref
  return __primitive("field by num", x, i);

/* Get a mutable ref to a field in a class or record by name.
   Will generate a compilation error if a field with that name
   is not found or if the class or record is not mutable.

   :arg x: a class or record
   :arg s: the name of a field
   :returns: an rvalue referring to that field.
 */
proc getFieldRef(ref x:?t, param s:string) ref {
  param i = __primitive("field name to num", t, s);
  if i == 0 then
    compilerError("field ", s, " not found in ", t:string);
  return __primitive("field by num", x, i);
}

/* Get a field index in a class or record, or 0 if
   the field is not found.

   :arg t: a class or record type
   :arg s: the name of a field
   :returns: an index `i` usable in getField, or 0 if the field was not found.
 */
proc getFieldIndex(type t, param s:string) param : int
  return __primitive("field name to num", t, s);

/* Returns `true` if a class or record has a field named `s`,
   or `false` otherwise.

   :arg t: a class or record type
   :arg s: the name of a field
   :returns: `true` if the field is present.
 */

proc hasField(type t, param s:string) param : bool
  return getFieldIndex(t, s) > 0;

/* Returns true if a function named `fname` taking no arguments
   could be called in the current scope.
   */
proc canResolve(param fname : string) param : bool
  return __primitive("call resolves", fname);

/* Returns true if a function named `fname` taking the arguments in `args`
   could be called in the current scope.
   */
proc canResolve(param fname : string, args ...) param : bool
  return __primitive("call resolves", fname, (...args));

// TODO -- how can this work with by-name argument passing?

/* Returns true if a method named `fname` taking no arguments
   could be called on `obj` in the current scope.
   */
proc canResolveMethod(obj, param fname : string) param : bool
  return __primitive("method call resolves", obj, fname);

/* Returns true if a method named `fname` taking the arguments in `args`
   could be called on `obj` in the current scope.
   */
proc canResolveMethod(obj, param fname : string, args ...) param : bool
  return __primitive("method call resolves", obj, fname, (...args));

/* Returns true if a type method named `fname` taking no
   arguments could be called on type `t` in the current scope.
   */
proc canResolveTypeMethod(type t, param fname : string) param : bool
  return __primitive("method call resolves", t, fname);
 
/* Returns true if a type method named `fname` taking the
   arguments in `args` could be called on type `t` in the current scope.
   */
proc canResolveTypeMethod(type t, param fname : string, args ...) param : bool
  return __primitive("method call resolves", t, fname, (...args));

// TODO -- do we need a different version of can resolve with ref this?

} // module Reflection

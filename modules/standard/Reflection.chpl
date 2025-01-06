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

/* Support for reflecting about properties of a Chapel program's code.

   Functions for reflecting about language elements, such as fields,
   functions, and methods.

   For reflecting about aspects of the compilation process, see
   :mod:`ChplConfig`.
*/
module Reflection {

// Used to test "--warn-unstable-standard", ignore.
@unstable
var chpl_unstableStandardSymbolForTesting: int;
chpl_unstableStandardSymbolForTesting;

/* Ensure that a query about fields is applied to a class/record/union type.
   Return that type. If it is a class type, strip any decorators/mem managers.
*/
pragma "suppress generic actual warning"
private proc checkQueryT(type t) type {
  if isClassType(t) then
    return t: borrowed class;
  else if isRecordType(t) || isUnionType(t) then
    return t;
  else
    compilerError(t:string, " is not a class, record, or union type", 2);
}
private proc checkValidQueryT(type t) param {
  if !isClassType(t) && !isRecordType(t) && !isUnionType(t) then
    compilerError(t:string, " is not a class, record, or union type", 2);
}

/* Return the number of fields in a class or record as a param.
   The count of fields includes types and param fields.
 */
pragma "suppress generic actual warning"
proc getNumFields(type t) param : int do
  return __primitive("num fields", checkQueryT(t));

/* Return the number of fields in a class or record as a param.
   The count of fields includes types and param fields.
 */
@deprecated(notes="'numFields' is deprecated - please use 'getNumFields' instead")
proc numFields(type t) param : int do return getNumFields(t);

/* Get the name of the field at `idx` in a class or record.
   Causes a compilation error if `idx` is not in 0..<getNumFields(t).

   :arg t: a class or record type
   :arg idx: which field to get the name of
   :returns: the name of the field, as a param string
 */
pragma "suppress generic actual warning"
proc getFieldName(type t, param idx:int) param : string do
  return __primitive("field num to name", checkQueryT(t), idx+1);

// Note, since this version has a where clause, it is preferred
// over the const ref one.
/* Get the field at `idx` in a class or record. When the field at `idx`
   is a `param`, this overload will be chosen to return a `param`.
   Causes a compilation error if `idx` is not in 0..<getNumFields(t).

   :arg obj: a class or record
   :arg idx: which field to get
   :returns: the `param` that field represents
*/
proc getField(const ref obj:?t, param idx: int) param
  where idx >= 0 && idx < getNumFields(t) &&
        isParam(__primitive("field by num", obj, idx+1)) {

  return __primitive("field by num", obj, idx+1);
}

// Note, since this version has a where clause, it is preferred
// over the const ref one.
/* Get the field at `idx` in a class or record. When the field at `idx`
   is a `type` variable, this overload will be chosen to return a type.
   Causes a compilation error if `idx` is not in 0..<getNumFields(t).

   :arg obj: a class or record
   :arg idx: which field to get
   :returns: the type that field represents
*/
proc getField(const ref obj:?t, param idx: int) type
  where idx >= 0 && idx < getNumFields(t) &&
        isType(__primitive("field by num", obj, idx+1)) {
  return __primitive("field by num", obj, idx+1);
}

/* Get the field at `idx` in a class or record.
   Causes a compilation error if `idx` is not in 0..<getNumFields(t).

   :arg obj: a class or record
   :arg idx: which field to get
   :returns: a const reference to that field.
 */
pragma "unsafe"
inline proc getField(const ref obj:?t, param idx:int) const ref do
  return __primitive("field by num", obj, idx+1);

/* Get a field in a class or record by name. When the named
   field is a `param`, this overload will be chosen to return a
   `param`. Will generate a compilation error if a field named
   `name` is not found.

   :arg obj: a class or record
   :arg name: the name of a field
   :returns: the `param` that field represents
 */
proc getField(const ref obj:?t, param name: string) param
where getFieldIndex(t, name) != -1 &&
      isParam(getField(obj, getFieldIndex(t, name))) {
  return getField(obj, getFieldIndex(t, name));
}

/* Get a field in a class or record by name. When the named
   field is a `type` variable, this overload will be chosen to
   return a type. Will generate a compilation error if a field
   named `name` is not found.

   :arg obj: a class or record
   :arg name: the name of a field
   :returns: the type that field represents
 */
proc getField(const ref obj:?t, param name: string) type
  where getFieldIndex(t, name) != -1 && isType(getField(obj, getFieldIndex(t, name))) {

  return getField(obj, getFieldIndex(t, name));
}

/* Get a field in a class or record by name.
   Will generate a compilation error if a field named `name`
   is not found.

   :arg obj: a class or record
   :arg name: the name of a field
   :returns: a const reference to that field.
 */
pragma "unsafe"
inline proc getField(const ref obj:?t, param name:string) const ref {
  param i = __primitive("field name to num", t, name);
  if i == 0 then
    compilerError("field ", name, " not found in ", t:string);
  return __primitive("field by num", obj, i);
}

/* numImplementationFields() and getImplementationField()
   allows querying non-record types that are implemented using records.
   Restricting their applicability with isImplementedWithRecords()
   prevents an internal error that currently would happen
   when they are invoked on `int`, for example. Additional
   types can be added to isImplementedWithRecords() as needed.
*/

pragma "suppress generic actual warning"
@chpldoc.nodoc
proc isImplementedWithRecords(type t) param do
  return isRangeType(t) || isStringType(t);

pragma "suppress generic actual warning"
@chpldoc.nodoc
proc numImplementationFields(type t) param : int
  where isImplementedWithRecords(t) do
  return __primitive("num fields", t);

@chpldoc.nodoc
proc getImplementationField(const ref x:?t, param i: int) type
  where isImplementedWithRecords(t) &&
        isType(__primitive("field by num", x, i)) do
  return __primitive("field by num", x, i);

@chpldoc.nodoc
proc getImplementationField(const ref x:?t, param i: int) param
  where isImplementedWithRecords(t) &&
        isParam(__primitive("field by num", x, i)) do
  return __primitive("field by num", x, i);

pragma "unsafe"
@chpldoc.nodoc
proc getImplementationField(const ref x:?t, param i:int) const ref {
  if !isImplementedWithRecords(t) then
    compilerError("an argument of the type ", t:string,
                  " is not valid for getImplementationField()");
  return __primitive("field by num", x, i);
}

/* Get a mutable ref to the ith field in a class or record.
   Causes a compilation error if `i` is not in 0..<getNumFields(t)
   or if the argument is not mutable.

   :arg x: a class or record
   :arg i: which field to get
   :returns: a mutable reference to that field.
 */
pragma "unsafe"
@unstable(reason="'getFieldRef' is unstable")
inline proc getFieldRef(ref x:?t, param i:int) ref {
  checkValidQueryT(t);
  if isType(__primitive("field by num", x, i+1)) then
    compilerError("cannot return a reference to 'type' field '",
                  getFieldName(t, i), "'");
  if isParam(__primitive("field by num", x, i+1)) then
    compilerError("cannot return a reference to 'param' field '",
                  getFieldName(t, i), "'");
  return __primitive("field by num", x, i+1);
}

pragma "unsafe"
@chpldoc.nodoc
@unstable(reason="'getFieldRef' is unstable")
inline proc getFieldRef(x: borrowed, param i:int) ref {
  checkValidQueryT(x.type);
  if isType(__primitive("field by num", x, i+1)) then
    compilerError("cannot return a reference to 'type' field '",
                  getFieldName(x.type, i), "'");
  if isParam(__primitive("field by num", x, i+1)) then
    compilerError("cannot return a reference to 'param' field '",
                  getFieldName(x.type, i), "'");
  return __primitive("field by num", x, i+1);
}

/* Get a mutable ref to a field in a class or record by name.
   Will generate a compilation error if a field with that name
   is not found or if the class or record is not mutable.

   :arg x: a class or record
   :arg s: the name of a field
   :returns: a mutable reference to that field.
 */
pragma "unsafe"
@unstable(reason="'getFieldRef' is unstable")
proc getFieldRef(ref x:?t, param s:string) ref {
  checkValidQueryT(t);
  param i = __primitive("field name to num", t, s);
  if i == 0 then
    compilerError("field ", s, " not found in ", t:string);
  if isType(__primitive("field by num", x, i)) then
    compilerError("cannot return a reference to 'type' field '", s, "'");
  if isParam(__primitive("field by num", x, i)) then
    compilerError("cannot return a reference to 'param' field '", s, "'");
  return __primitive("field by num", x, i);
}

/* Get the index of a field named `name` in a class or record type `t`,
   or ``-1`` if the field `name` is not found.

   :arg t: a class or record type
   :arg name: the name of a field
   :returns: an index usable in :proc:`getField`, or ``-1`` if the field
             was not found.
 */
pragma "suppress generic actual warning"
proc getFieldIndex(type t, param name:string) param : int do
  return __primitive("field name to num", checkQueryT(t), name)-1;

/* Returns ``true`` if a class or record has a field named `name`,
   or ``false`` otherwise.

   :arg t: a class or record type
   :arg name: the name of a field
   :returns: ``true`` if the field is present.
 */
pragma "suppress generic actual warning"
proc hasField(type t, param name:string) param : bool do
  return getFieldIndex(t, name) >= 0;

/* Returns ``true`` if the field at `idx` has been instantiated in a given
   class or record type `t`.

   :arg t: a class or record type
   :arg idx: which field to query
   :returns: ``true`` if the field is instantiated
*/
pragma "suppress generic actual warning"
@unstable(reason="'isFieldBound' is unstable - consider using 'T.fieldName != ?' syntax instead")
proc isFieldBound(type t, param idx: int) param : bool {
  return __primitive("is bound", checkQueryT(t),
                     getFieldName(checkQueryT(t), idx));
}

/* Returns ``true`` if the field named `name` has been instantiated in a
   given class or record type `t`.

   :arg t: a class or record type
   :arg name: the name of a field
   :returns: ``true`` if the field is instantiated
*/
pragma "suppress generic actual warning"
@unstable(reason="'isFieldBound' is unstable - consider using 'T.fieldName != ?' syntax instead")
proc isFieldBound(type t, param name : string) param : bool {
  return __primitive("is bound", checkQueryT(t), name);
}

/* Returns ``true`` if a function named `fname` taking no arguments
   could be called in the current scope.
   */
@unstable(reason="The 'canResolve...' family of procedures are unstable")
proc canResolve(param fname : string) param : bool do
  return __primitive("call and fn resolves", fname);

/* Returns ``true`` if a function named `fname` taking the arguments in
   `args` could be called in the current scope.
   */
@unstable(reason="The 'canResolve...' family of procedures are unstable")
proc canResolve(param fname : string, args ...) param : bool do
  return __primitive("call and fn resolves", fname, (...args));

// TODO -- how can this work with by-name argument passing?

/* Returns ``true`` if a method named `fname` taking no arguments
   could be called on `obj` in the current scope.
   */
@unstable(reason="The 'canResolve...' family of procedures are unstable")
proc canResolveMethod(obj, param fname : string) param : bool do
  return __primitive("method call and fn resolves", obj, fname);

/* Returns ``true`` if a method named `fname` taking the arguments in
   `args` could be called on `obj` in the current scope.
   */
@unstable(reason="The 'canResolve...' family of procedures are unstable")
proc canResolveMethod(obj, param fname : string, args ...) param : bool do
  return __primitive("method call and fn resolves", obj, fname, (...args));

/* Returns ``true`` if a type method named `fname` taking no
   arguments could be called on type `t` in the current scope.
   */
@unstable(reason="The 'canResolve...' family of procedures are unstable")
proc canResolveTypeMethod(type t, param fname : string) param : bool do
  return __primitive("method call and fn resolves", t, fname);

/* Returns ``true`` if a type method named `fname` taking the
   arguments in `args` could be called on type `t` in the current scope.
   */
@unstable(reason="The 'canResolve...' family of procedures are unstable")
proc canResolveTypeMethod(type t, param fname : string, args ...) param : bool do
  return __primitive("method call and fn resolves", t, fname, (...args));

// TODO -- do we need a different version of can resolve with ref this?

/* Returns the line number of the call to this function. */
pragma "get line number"
proc getLineNumber() param : int { }

/* Returns the file name this function was called from. */
pragma "get file name"
proc getFileName() param : string { }

/* Returns the name of the function this function was called from. */
pragma "get function name"
proc getRoutineName() param : string { }

/* Returns the name of the module this function was called from. */
pragma "get module name"
proc getModuleName() param : string { }

} // module Reflection

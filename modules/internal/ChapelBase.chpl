/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

// ChapelBase.chpl
//

module ChapelBase {

  // We deprecate in the module code so that we don't have to modify dyno
  // to teach it about the 'c_string' type. We perform the deprecation in
  // ChapelBase so that you don't have to 'import CTypes' to see the type
  // 'c_string' (which could break a lot of programs). This is OK because
  // after the deprecation period we can just remove 'c_string' entirely.
  pragma "last resort"
  @deprecated(notes="the type 'c_string' is deprecated; please 'use CTypes' and replace 'c_string' with 'c_ptrConst(c_char)'")
  type c_string = chpl_c_string;

  // c_fn_ptr stuff

  // although it can just be a compiler-inserted primitive,
  // we declare it so we can mark it unstable
  @chpldoc.nodoc
  @unstable("'c_fn_ptr' is unstable, and may be replaced by first-class procedure functionality")
  type c_fn_ptr = chpl_c_fn_ptr;

  @chpldoc.nodoc
  @unstable
  inline operator c_fn_ptr.=(ref a:c_fn_ptr, b:c_fn_ptr) {
    __primitive("=", a, b);
  }
  pragma "do not resolve unless called"
  @chpldoc.nodoc
  @unstable
  proc c_fn_ptr.this() {
    compilerError("Can't call a C function pointer within Chapel");
  }
  pragma "do not resolve unless called"
  @chpldoc.nodoc
  @unstable
  proc c_fn_ptr.this(args...) {
    compilerError("Can't call a C function pointer within Chapel");
  }

  pragma "locale private"
  @chpldoc.nodoc
  var rootLocaleInitialized: bool = false;

  public use ChapelStandard;
  use CTypes;
  use ChplConfig;

  config param enablePostfixBangChecks = false;

  // These two are called by compiler-generated code.
  extern proc chpl_config_has_value(name:c_ptrConst(c_char), module_name:c_ptrConst(c_char)): bool;
  extern proc chpl_config_get_value(name:c_ptrConst(c_char), module_name:c_ptrConst(c_char)): c_ptrConst(c_char);

  // the default low bound to use for arrays, tuples, etc.
  config param defaultLowBound = 0;

  // minimum buffer size allocated for string/bytes
  config param chpl_stringMinAllocSize = 0;

  config param warnMaximalRange = false;    // Warns if integer rollover will cause
                                            // the iterator to yield zero times.

  // Used to test "--warn-unstable-internal", ignore.
  @chpldoc.nodoc
  @unstable
  var chpl_unstableInternalSymbolForTesting: int;
  chpl_unstableInternalSymbolForTesting;


  // We have the following two config params here instead of in
  // ChapelDomain so to make it easy for us to access these in the
  // compiler, since we already always have a reference to baseModule
  // in the production compiler.

  /* Compile with ``-sassocParSafeDefault=true`` to use ``parSafe=true``
     by default for associative domains and arrays.
     Compiling with an explicit ``-sassocParSafeDefault[=false]`` will
     turn off the par safe warning, just like ``-snoParSafeWarning``*/
  @chpldoc.nodoc
  config param assocParSafeDefault = false;

  /* Compile with ``-snoParSafeWarning`` to suppress the warning
     about a missing explicit ``parSafe`` parameter and
     about the default parSafe mode for associative domains
     and arrays changing from ``true`` to ``false``. */
  @chpldoc.nodoc
  config param noParSafeWarning = false;

  pragma "object class"
  pragma "global type symbol"
  pragma "no object"
  class _object { }

  @deprecated(notes="the 'object' abstract root class has been deprecated; please use 'RootClass' instead")
  class object { }

  enum iterKind {leader, follower, standalone};

  // This flag toggles on the new pointer-based implementation.
  // It is unstable and experimental.
  config param fcfsUsePointerImplementation = false;

  //
  // assignment on primitive types
  //
  inline operator =(ref a: bool, b: bool) { __primitive("=", a, b); }

  inline operator =(ref a: int(8), b: int(8)) do __primitive("=", a, b);
  inline operator =(ref a: int(16), b: int(16)) do __primitive("=", a, b);
  inline operator =(ref a: int(32), b: int(32)) do __primitive("=", a, b);
  inline operator =(ref a: int(64), b: int(64)) do __primitive("=", a, b);

  inline operator =(ref a: uint(8), b: uint(8)) do __primitive("=", a, b);
  inline operator =(ref a: uint(16), b: uint(16)) do __primitive("=", a, b);
  inline operator =(ref a: uint(32), b: uint(32)) do __primitive("=", a, b);
  inline operator =(ref a: uint(64), b: uint(64)) do __primitive("=", a, b);

  inline operator =(ref a: real(32), b: real(32)) do __primitive("=", a, b);
  inline operator =(ref a: real(64), b: real(64)) do __primitive("=", a, b);

  inline operator =(ref a: imag(32), b: imag(32)) do __primitive("=", a, b);
  inline operator =(ref a: imag(64), b: imag(64)) do __primitive("=", a, b);

  inline operator =(ref a: complex(64), b: complex(64)) do __primitive("=", a, b);
  inline operator =(ref a: complex(128), b: complex(128)) do __primitive("=", a, b);
  inline operator =(ref a:opaque, b:opaque) {__primitive("=", a, b); }
  inline operator =(ref a:enum, b:enum) where (a.type == b.type) {__primitive("=", a, b); }

  // Need pragma "last resort" to allow assignments to sync/single vars.
  // a.type in a formal's type is computed before instantiation vs.
  // a.type in the where clause is computed after instantiation.
  pragma "last resort"
  inline operator =(ref a: borrowed class,   b: a.type) where isSubtype(b.type, a.type) { __primitive("=", a, b); }
  pragma "last resort"
  inline operator =(ref a: borrowed class?,  b: a.type) where isSubtype(b.type, a.type) { __primitive("=", a, b); }
  pragma "last resort"
  inline operator =(ref a: unmanaged class,  b: a.type) where isSubtype(b.type, a.type) { __primitive("=", a, b); }
  pragma "last resort"
  inline operator =(ref a: unmanaged class?, b: a.type) where isSubtype(b.type, a.type) { __primitive("=", a, b); }

  inline operator =(ref a: nothing, b: ?t) where t != nothing {
    compilerError("a nothing variable cannot be assigned");
  }

  inline operator =(ref a: ?t, b: nothing) where t != nothing {
    compilerError("cannot assign none to a variable of non-nothing type");
  }

  // This needs to be param so calls to it are removed after they are resolved
  inline operator =(ref a: nothing, b: nothing) param { }

  //
  // equality comparison on primitive types
  //
  inline operator ==(a: _nilType, b: _nilType) param do return true;
  inline operator ==(a: bool, b: bool) do return __primitive("==", a, b);

  inline operator ==(a: int(8), b: int(8)) do return __primitive("==", a, b);
  inline operator ==(a: int(16), b: int(16)) do return __primitive("==", a, b);
  inline operator ==(a: int(32), b: int(32)) do return __primitive("==", a, b);
  inline operator ==(a: int(64), b: int(64)) do return __primitive("==", a, b);

  inline operator ==(a: uint(8), b: uint(8)) do return __primitive("==", a, b);
  inline operator ==(a: uint(16), b: uint(16)) do return __primitive("==", a, b);
  inline operator ==(a: uint(32), b: uint(32)) do return __primitive("==", a, b);
  inline operator ==(a: uint(64), b: uint(64)) do return __primitive("==", a, b);

  inline operator ==(a: real(32), b: real(32)) do return __primitive("==", a, b);
  inline operator ==(a: real(64), b: real(64)) do return __primitive("==", a, b);

  inline operator ==(a: imag(32), b: imag(32)) do return __primitive("==", a, b);
  inline operator ==(a: imag(64), b: imag(64)) do return __primitive("==", a, b);

  inline operator ==(a: complex(64), b: complex(64)) do return a.re == b.re && a.im == b.im;
  inline operator ==(a: complex(128), b: complex(128)) do return a.re == b.re && a.im == b.im;
  inline operator ==(a: borrowed RootClass?, b: borrowed RootClass?) do return __primitive("ptr_eq", a, b);
  inline operator ==(a: borrowed RootClass?, b: _nilType) do return __primitive("==", a, nil);
  inline operator ==(a: _nilType, b: borrowed RootClass?) do return __primitive("==", b, nil);
  inline operator ==(a: enum, b: enum) where (a.type == b.type) {
    return __primitive("==", a, b);
  }
  pragma "last resort"
  inline operator ==(a: enum, b: enum) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return false;
  }

  inline operator !=(a: _nilType, b: _nilType) param do return false;
  inline operator !=(a: bool, b: bool) do return __primitive("!=", a, b);

  inline operator !=(a: int(8), b: int(8)) do return __primitive("!=", a, b);
  inline operator !=(a: int(16), b: int(16)) do return __primitive("!=", a, b);
  inline operator !=(a: int(32), b: int(32)) do return __primitive("!=", a, b);
  inline operator !=(a: int(64), b: int(64)) do return __primitive("!=", a, b);

  inline operator !=(a: uint(8), b: uint(8)) do return __primitive("!=", a, b);
  inline operator !=(a: uint(16), b: uint(16)) do return __primitive("!=", a, b);
  inline operator !=(a: uint(32), b: uint(32)) do return __primitive("!=", a, b);
  inline operator !=(a: uint(64), b: uint(64)) do return __primitive("!=", a, b);

  inline operator !=(a: real(32), b: real(32)) do return __primitive("!=", a, b);
  inline operator !=(a: real(64), b: real(64)) do return __primitive("!=", a, b);

  inline operator !=(a: imag(32), b: imag(32)) do return __primitive("!=", a, b);
  inline operator !=(a: imag(64), b: imag(64)) do return __primitive("!=", a, b);

  inline operator !=(a: complex(64), b: complex(64)) do return a.re != b.re || a.im != b.im;
  inline operator !=(a: complex(128), b: complex(128)) do return a.re != b.re || a.im != b.im;
  inline operator !=(a: borrowed RootClass?, b: borrowed RootClass?) do return __primitive("ptr_neq", a, b);
  inline operator !=(a: borrowed RootClass?, b: _nilType) do return __primitive("!=", a, nil);
  inline operator !=(a: _nilType, b: borrowed RootClass?) do return __primitive("!=", b, nil);
  inline operator !=(a: enum, b: enum) where (a.type == b.type) {
    return __primitive("!=", a, b);
  }
  pragma "last resort"
  inline operator !=(a: enum, b: enum) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return true;
  }

  inline operator ==(param a: bool, param b: bool) param do return __primitive("==", a, b);

  inline operator ==(param a: int(8), param b: int(8)) param do return __primitive("==", a, b);
  inline operator ==(param a: int(16), param b: int(16)) param do return __primitive("==", a, b);
  inline operator ==(param a: int(32), param b: int(32)) param do return __primitive("==", a, b);
  inline operator ==(param a: int(64), param b: int(64)) param do return __primitive("==", a, b);

  inline operator ==(param a: uint(8), param b: uint(8)) param do return __primitive("==", a, b);
  inline operator ==(param a: uint(16), param b: uint(16)) param do return __primitive("==", a, b);
  inline operator ==(param a: uint(32), param b: uint(32)) param do return __primitive("==", a, b);
  inline operator ==(param a: uint(64), param b: uint(64)) param do return __primitive("==", a, b);
  //
  inline operator ==(param a: enum, param b: enum) param where (a.type == b.type) do return __primitive("==", a, b);
  //
  // NOTE: For param enums, Only '==' is implemented in the compiler
  // as a primitive. It assumes that the two param enums are of the
  // same type, as guaranteed by the where clause above.  All other
  // param enum routines are defined as module code to avoid having to
  // teach the compiler how to implement all enum comparisons.

  inline operator ==(param a: real(32), param b: real(32)) param do return __primitive("==", a, b);
  inline operator ==(param a: real(64), param b: real(64)) param do return __primitive("==", a, b);

  inline operator ==(param a: imag(32), param b: imag(32)) param do return __primitive("==", a, b);
  inline operator ==(param a: imag(64), param b: imag(64)) param do return __primitive("==", a, b);

  inline operator ==(param a: complex(64), param b: complex(64)) param do return __primitive("==", a, b);
  inline operator ==(param a: complex(128), param b: complex(128)) param do return __primitive("==", a, b);
  inline operator ==(a: nothing, b: nothing) param do return true;

  inline operator !=(param a: bool, param b: bool) param do return __primitive("!=", a, b);

  inline operator !=(param a: int(8), param b: int(8)) param do return __primitive("!=", a, b);
  inline operator !=(param a: int(16), param b: int(16)) param do return __primitive("!=", a, b);
  inline operator !=(param a: int(32), param b: int(32)) param do return __primitive("!=", a, b);
  inline operator !=(param a: int(64), param b: int(64)) param do return __primitive("!=", a, b);

  inline operator !=(param a: uint(8), param b: uint(8)) param do return __primitive("!=", a, b);
  inline operator !=(param a: uint(16), param b: uint(16)) param do return __primitive("!=", a, b);
  inline operator !=(param a: uint(32), param b: uint(32)) param do return __primitive("!=", a, b);
  inline operator !=(param a: uint(64), param b: uint(64)) param do return __primitive("!=", a, b);

  inline operator !=(param a: enum, param b: enum) param where (a.type == b.type) do return __primitive("!=", chpl__enumToOrder(a), chpl__enumToOrder(b));

  inline operator !=(param a: real(32), param b: real(32)) param do return __primitive("!=", a, b);
  inline operator !=(param a: real(64), param b: real(64)) param do return __primitive("!=", a, b);

  inline operator !=(param a: imag(32), param b: imag(32)) param do return __primitive("!=", a, b);
  inline operator !=(param a: imag(64), param b: imag(64)) param do return __primitive("!=", a, b);

  inline operator !=(param a: complex(64), param b: complex(64)) param do return __primitive("!=", a, b);
  inline operator !=(param a: complex(128), param b: complex(128)) param do return __primitive("!=", a, b);
  inline operator !=(a: nothing, b: nothing) param do return false;

  //
  // ordered comparison on primitive types
  //
  inline operator <=(a: int(8), b: int(8)) do return __primitive("<=", a, b);
  inline operator <=(a: int(16), b: int(16)) do return __primitive("<=", a, b);
  inline operator <=(a: int(32), b: int(32)) do return __primitive("<=", a, b);
  inline operator <=(a: int(64), b: int(64)) do return __primitive("<=", a, b);

  inline operator <=(a: uint(8), b: uint(8)) do return __primitive("<=", a, b);
  inline operator <=(a: uint(16), b: uint(16)) do return __primitive("<=", a, b);
  inline operator <=(a: uint(32), b: uint(32)) do return __primitive("<=", a, b);
  inline operator <=(a: uint(64), b: uint(64)) do return __primitive("<=", a, b);

  inline operator <=(a: real(32), b: real(32)) do return __primitive("<=", a, b);
  inline operator <=(a: real(64), b: real(64)) do return __primitive("<=", a, b);
  operator <=(a: enum, b: enum) where (a.type == b.type) {
    return __primitive("<=", chpl__enumToOrder(a), chpl__enumToOrder(b));
  }
  pragma "last resort"
  inline operator <=(a: enum, b: enum) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return false;
  }

  inline operator >=(a: int(8), b: int(8)) do return __primitive(">=", a, b);
  inline operator >=(a: int(16), b: int(16)) do return __primitive(">=", a, b);
  inline operator >=(a: int(32), b: int(32)) do return __primitive(">=", a, b);
  inline operator >=(a: int(64), b: int(64)) do return __primitive(">=", a, b);

  inline operator >=(a: uint(8), b: uint(8)) do return __primitive(">=", a, b);
  inline operator >=(a: uint(16), b: uint(16)) do return __primitive(">=", a, b);
  inline operator >=(a: uint(32), b: uint(32)) do return __primitive(">=", a, b);
  inline operator >=(a: uint(64), b: uint(64)) do return __primitive(">=", a, b);

  inline operator >=(a: real(32), b: real(32)) do return __primitive(">=", a, b);
  inline operator >=(a: real(64), b: real(64)) do return __primitive(">=", a, b);
  operator >=(a: enum, b: enum) where (a.type == b.type) {
    return __primitive(">=", chpl__enumToOrder(a), chpl__enumToOrder(b));
  }
  pragma "last resort"
  inline operator >=(a: enum, b: enum) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return false;
  }

  // operator <(integral, integral)

  // cast an int(?w) to uint(w)
  private inline proc asUint(a: integral) do return a: uint(numBits(a.type));

  inline operator <(a: integral, b: integral) do return
    // prim("<") works correctly for same signed-ness even with diff. sizes
    if isInt(a) then
      if isInt(b) then __primitive("<", a, b)
      else __primitive("<", a, 0) || __primitive("<", asUint(a), b)
    else
      if isUint(b) then __primitive("<", a, b)
      else ! __primitive("<", b, 0) && __primitive("<", a, asUint(b));

  inline operator <(a: integral, param b: integral) param
    where isUint(a) && __primitive("<=", b, 0)
    do return false;

  inline operator <(param a: integral, b: integral) param
    where isUint(b) && __primitive("<", a, 0)
    do return true;

  inline operator <(param a: integral, param b: integral) param do return
    // prim("<") may be wrong with mixed int*uint ex. prim("<", 1, (-5):uint)
    if a >= 0 then
      if b >= 0 then __primitive("<", a, b) else false
    else
      if b >= 0 then true else __primitive("<", a, b);

  // operator < involving bool
  inline operator <(a: bool, b: bool)     do return a:int < b:int;
  inline operator <(a: bool, b: integral) do return a:int < b;
  inline operator <(a: integral, b: bool) do return a     < b:int;
  inline operator <(param a: bool, param b: bool) param
    do return __primitive("<", a, b);

  inline operator <(a: real(32), b: real(32)) do return __primitive("<", a, b);
  inline operator <(a: real(64), b: real(64)) do return __primitive("<", a, b);

  operator <(a: enum, b: enum) where (a.type == b.type) {
    return __primitive("<", chpl__enumToOrder(a), chpl__enumToOrder(b));
  }
  pragma "last resort"
  inline operator <(a: enum, b: enum) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return false;
  }

  inline operator >(a: int(8), b: int(8)) do return __primitive(">", a, b);
  inline operator >(a: int(16), b: int(16)) do return __primitive(">", a, b);
  inline operator >(a: int(32), b: int(32)) do return __primitive(">", a, b);
  inline operator >(a: int(64), b: int(64)) do return __primitive(">", a, b);

  inline operator >(a: uint(8), b: uint(8)) do return __primitive(">", a, b);
  inline operator >(a: uint(16), b: uint(16)) do return __primitive(">", a, b);
  inline operator >(a: uint(32), b: uint(32)) do return __primitive(">", a, b);
  inline operator >(a: uint(64), b: uint(64)) do return __primitive(">", a, b);

  inline operator >(a: real(32), b: real(32)) do return __primitive(">", a, b);
  inline operator >(a: real(64), b: real(64)) do return __primitive(">", a, b);
  operator >(a: enum, b: enum) where (a.type == b.type) {
    return __primitive(">", chpl__enumToOrder(a), chpl__enumToOrder(b));
  }
  pragma "last resort"
  inline operator >(a: enum, b: enum) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return false;
  }

  inline operator <=(param a: int(8), param b: int(8)) param do return __primitive("<=", a, b);
  inline operator <=(param a: int(16), param b: int(16)) param do return __primitive("<=", a, b);
  inline operator <=(param a: int(32), param b: int(32)) param do return __primitive("<=", a, b);
  inline operator <=(param a: int(64), param b: int(64)) param do return __primitive("<=", a, b);

  inline operator <=(param a: uint(8), param b: uint(8)) param do return __primitive("<=", a, b);
  inline operator <=(param a: uint(16), param b: uint(16)) param do return __primitive("<=", a, b);
  inline operator <=(param a: uint(32), param b: uint(32)) param do return __primitive("<=", a, b);
  inline operator <=(param a: uint(64), param b: uint(64)) param do return __primitive("<=", a, b);
  inline operator <=(param a: enum, param b: enum) param where (a.type == b.type) do return __primitive("<=", chpl__enumToOrder(a), chpl__enumToOrder(b));

  inline operator <=(param a: real(32), param b: real(32)) param do return __primitive("<=", a, b);
  inline operator <=(param a: real(64), param b: real(64)) param do return __primitive("<=", a, b);

  inline operator >=(param a: int(8), param b: int(8)) param do return __primitive(">=", a, b);
  inline operator >=(param a: int(16), param b: int(16)) param do return __primitive(">=", a, b);
  inline operator >=(param a: int(32), param b: int(32)) param do return __primitive(">=", a, b);
  inline operator >=(param a: int(64), param b: int(64)) param do return __primitive(">=", a, b);

  inline operator >=(param a: uint(8), param b: uint(8)) param do return __primitive(">=", a, b);
  inline operator >=(param a: uint(16), param b: uint(16)) param do return __primitive(">=", a, b);
  inline operator >=(param a: uint(32), param b: uint(32)) param do return __primitive(">=", a, b);
  inline operator >=(param a: uint(64), param b: uint(64)) param do return __primitive(">=", a, b);
  inline operator >=(param a: enum, param b: enum) param where (a.type == b.type) do return __primitive(">=", chpl__enumToOrder(a), chpl__enumToOrder(b));

  inline operator >=(param a: real(32), param b: real(32)) param do return __primitive(">=", a, b);
  inline operator >=(param a: real(64), param b: real(64)) param do return __primitive(">=", a, b);

  inline operator <(param a: enum, param b: enum) param where (a.type == b.type) do return __primitive("<", chpl__enumToOrder(a), chpl__enumToOrder(b));

  inline operator <(param a: real(32), param b: real(32)) param do return __primitive("<", a, b);
  inline operator <(param a: real(64), param b: real(64)) param do return __primitive("<", a, b);

  inline operator >(param a: int(8), param b: int(8)) param do return __primitive(">", a, b);
  inline operator >(param a: int(16), param b: int(16)) param do return __primitive(">", a, b);
  inline operator >(param a: int(32), param b: int(32)) param do return __primitive(">", a, b);
  inline operator >(param a: int(64), param b: int(64)) param do return __primitive(">", a, b);

  inline operator >(param a: uint(8), param b: uint(8)) param do return __primitive(">", a, b);
  inline operator >(param a: uint(16), param b: uint(16)) param do return __primitive(">", a, b);
  inline operator >(param a: uint(32), param b: uint(32)) param do return __primitive(">", a, b);
  inline operator >(param a: uint(64), param b: uint(64)) param do return __primitive(">", a, b);
  inline operator >(param a: enum, param b: enum) param where (a.type == b.type) do return __primitive(">", chpl__enumToOrder(a), chpl__enumToOrder(b));

  inline operator >(param a: real(32), param b: real(32)) param do return __primitive(">", a, b);
  inline operator >(param a: real(64), param b: real(64)) param do return __primitive(">", a, b);

  //
  // unary + and - on primitive types
  //
  inline operator +(a: int(?w)) do return a;
  inline operator +(a: uint(?w)) do return a;
  inline operator +(a: real(?w)) do return a;
  inline operator +(a: imag(?w)) do return a;
  inline operator +(a: complex(?w)) do return a;

  inline operator -(a: int(?w)) do return __primitive("u-", a);
  inline operator -(a: uint(?w)) { compilerError("illegal use of '-' on operand of type ", a.type:string); }

  inline operator -(a: real(?w)) do return __primitive("u-", a);
  inline operator -(a: imag(?w)) do return __primitive("u-", a);
  inline operator -(a: complex(?w)) do return __primitive("u-", a);

  inline operator +(param a: int(?w)) param do return a;
  inline operator +(param a: uint(?w)) param do return a;
  inline operator +(param a: real(?w)) param do return a;
  inline operator +(param a: imag(?w)) param do return a;
  inline operator +(param a: complex(?w)) param do return a;

  inline operator -(param a: int(?w)) param do return __primitive("u-", a);
  inline operator -(param a: uint(?w)) param {
    compilerError("illegal use of '-' on operand of type ", a.type:string);
  }

  inline operator -(param a: real(?w)) param do return __primitive("u-", a);
  inline operator -(param a: imag(?w)) param do return __primitive("u-", a);
  inline operator -(param a: complex(?w)) param do return __primitive("u-", a);

  //
  // binary + and - on primitive types for runtime values
  //
  inline operator +(a: int(8), b: int(8)) do return __primitive("+", a, b);
  inline operator +(a: int(16), b: int(16)) do return __primitive("+", a, b);
  inline operator +(a: int(32), b: int(32)) do return __primitive("+", a, b);
  inline operator +(a: int(64), b: int(64)) do return __primitive("+", a, b);

  inline operator +(a: uint(8), b: uint(8)) do return __primitive("+", a, b);
  inline operator +(a: uint(16), b: uint(16)) do return __primitive("+", a, b);
  inline operator +(a: uint(32), b: uint(32)) do return __primitive("+", a, b);
  inline operator +(a: uint(64), b: uint(64)) do return __primitive("+", a, b);

  inline operator +(a: real(32), b: real(32)) do return __primitive("+", a, b);
  inline operator +(a: real(64), b: real(64)) do return __primitive("+", a, b);

  inline operator +(a: imag(32), b: imag(32)) do return __primitive("+", a, b);
  inline operator +(a: imag(64), b: imag(64)) do return __primitive("+", a, b);

  inline operator +(a: complex(64), b: complex(64)) do return __primitive("+", a, b);
  inline operator +(a: complex(128), b: complex(128)) do return __primitive("+", a, b);


  inline operator +(a: real(32), b: imag(32)) do return (a, _i2r(b)) : complex(64);
  inline operator +(a: real(64), b: imag(64)) do return (a, _i2r(b)) : complex(128);

  inline operator +(a: imag(32), b: real(32)) do return (b, _i2r(a)) : complex(64);
  inline operator +(a: imag(64), b: real(64)) do return (b, _i2r(a)) : complex(128);

  inline operator +(a: real(32), b: complex(64)) do return (a+b.re, b.im) : complex(64);
  inline operator +(a: real(64), b: complex(128)) do return (a+b.re, b.im) : complex(128);

  inline operator +(a: complex(64), b: real(32)) do return (a.re+b, a.im) : complex(64);
  inline operator +(a: complex(128), b: real(64)) do return (a.re+b, a.im) : complex(128);

  inline operator +(a: imag(32), b: complex(64)) do return (b.re, _i2r(a)+b.im) : complex(64);
  inline operator +(a: imag(64), b: complex(128)) do return (b.re, _i2r(a)+b.im) : complex(128);

  inline operator +(a: complex(64), b: imag(32)) do return (a.re, a.im+_i2r(b)) : complex(64);
  inline operator +(a: complex(128), b: imag(64)) do return (a.re, a.im+_i2r(b)) : complex(128);


  inline operator -(a: int(8), b: int(8)) do return __primitive("-", a, b);
  inline operator -(a: int(16), b: int(16)) do return __primitive("-", a, b);
  inline operator -(a: int(32), b: int(32)) do return __primitive("-", a, b);
  inline operator -(a: int(64), b: int(64)) do return __primitive("-", a, b);

  inline operator -(a: uint(8), b: uint(8)) do return __primitive("-", a, b);
  inline operator -(a: uint(16), b: uint(16)) do return __primitive("-", a, b);
  inline operator -(a: uint(32), b: uint(32)) do return __primitive("-", a, b);
  inline operator -(a: uint(64), b: uint(64)) do return __primitive("-", a, b);

  inline operator -(a: real(32), b: real(32)) do return __primitive("-", a, b);
  inline operator -(a: real(64), b: real(64)) do return __primitive("-", a, b);

  inline operator -(a: imag(32), b: imag(32)) do return __primitive("-", a, b);
  inline operator -(a: imag(64), b: imag(64)) do return __primitive("-", a, b);

  inline operator -(a: complex(64), b: complex(64)) do return __primitive("-", a, b);
  inline operator -(a: complex(128), b: complex(128)) do return __primitive("-", a, b);

  inline operator -(a: real(32), b: imag(32)) do return (a, -_i2r(b)) : complex(64);
  inline operator -(a: real(64), b: imag(64)) do return (a, -_i2r(b)) : complex(128);

  inline operator -(a: imag(32), b: real(32)) do return (-b, _i2r(a)) : complex(64);
  inline operator -(a: imag(64), b: real(64)) do return (-b, _i2r(a)) : complex(128);

  inline operator -(a: real(32), b: complex(64)) do return (a-b.re, -b.im) : complex(64);
  inline operator -(a: real(64), b: complex(128)) do return (a-b.re, -b.im) : complex(128);

  inline operator -(a: complex(64), b: real(32)) do return (a.re-b, a.im) : complex(64);
  inline operator -(a: complex(128), b: real(64)) do return (a.re-b, a.im) : complex(128);

  inline operator -(a: imag(32), b: complex(64)) do return (-b.re, _i2r(a)-b.im) : complex(64);
  inline operator -(a: imag(64), b: complex(128)) do return (-b.re, _i2r(a)-b.im) : complex(128);

  inline operator -(a: complex(64), b: imag(32)) do return (a.re, a.im-_i2r(b)) : complex(64);
  inline operator -(a: complex(128), b: imag(64)) do return (a.re, a.im-_i2r(b)) : complex(128);

  //
  // binary + and - on param values
  //
  inline operator +(param a: int(8), param b: int(8)) param do return __primitive("+", a, b);
  inline operator +(param a: int(16), param b: int(16)) param do return __primitive("+", a, b);
  inline operator +(param a: int(32), param b: int(32)) param do return __primitive("+", a, b);
  inline operator +(param a: int(64), param b: int(64)) param do return __primitive("+", a, b);

  inline operator +(param a: uint(8), param b: uint(8)) param do return __primitive("+", a, b);
  inline operator +(param a: uint(16), param b: uint(16)) param do return __primitive("+", a, b);
  inline operator +(param a: uint(32), param b: uint(32)) param do return __primitive("+", a, b);
  inline operator +(param a: uint(64), param b: uint(64)) param do return __primitive("+", a, b);

  inline operator +(param a: real(32), param b: real(32)) param do return __primitive("+", a, b);
  inline operator +(param a: real(64), param b: real(64)) param do return __primitive("+", a, b);

  inline operator +(param a: imag(32), param b: imag(32)) param do return __primitive("+", a, b);
  inline operator +(param a: imag(64), param b: imag(64)) param do return __primitive("+", a, b);

  inline operator +(param a: complex(64), param b: complex(64)) param do return __primitive("+", a, b);
  inline operator +(param a: complex(128), param b: complex(128)) param do return __primitive("+", a, b);

  inline operator +(param a: real(32), param b: imag(32)) param do return __primitive("+", a, b);
  inline operator +(param a: real(64), param b: imag(64)) param do return __primitive("+", a, b);

  inline operator +(param a: imag(32), param b: real(32)) param do return __primitive("+", a, b);
  inline operator +(param a: imag(64), param b: real(64)) param do return __primitive("+", a, b);
  /*inline operator +(param a: real(?w), param b: complex(w*2)) param do return
  __primitive("+", a, b);*/

  inline operator -(param a: int(8), param b: int(8)) param do return __primitive("-", a, b);
  inline operator -(param a: int(16), param b: int(16)) param do return __primitive("-", a, b);
  inline operator -(param a: int(32), param b: int(32)) param do return __primitive("-", a, b);
  inline operator -(param a: int(64), param b: int(64)) param do return __primitive("-", a, b);

  inline operator -(param a: uint(8), param b: uint(8)) param do return __primitive("-", a, b);
  inline operator -(param a: uint(16), param b: uint(16)) param do return __primitive("-", a, b);
  inline operator -(param a: uint(32), param b: uint(32)) param do return __primitive("-", a, b);
  inline operator -(param a: uint(64), param b: uint(64)) param do return __primitive("-", a, b);

  inline operator -(param a: real(32), param b: real(32)) param do return __primitive("-", a, b);
  inline operator -(param a: real(64), param b: real(64)) param do return __primitive("-", a, b);

  inline operator -(param a: imag(32), param b: imag(32)) param do return __primitive("-", a, b);
  inline operator -(param a: imag(64), param b: imag(64)) param do return __primitive("-", a, b);

  inline operator -(param a: complex(64), param b: complex(64)) param do return __primitive("-", a, b);
  inline operator -(param a: complex(128), param b: complex(128)) param do return __primitive("-", a, b);

  inline operator -(param a: real(32), param b: imag(32)) param do return __primitive("-", a, b);
  inline operator -(param a: real(64), param b: imag(64)) param do return __primitive("-", a, b);

  inline operator -(param a: imag(32), param b: real(32)) param do return __primitive("-", a, b);
  inline operator -(param a: imag(64), param b: real(64)) param do return __primitive("-", a, b);
  /*inline operator -(param a: real(?w), param b: complex(w*2)) param do return
  __primitive("-", a, b);*/

  //
  // * and / on primitive types
  //
  inline operator *(a: int(8), b: int(8)) do return __primitive("*", a, b);
  inline operator *(a: int(16), b: int(16)) do return __primitive("*", a, b);
  inline operator *(a: int(32), b: int(32)) do return __primitive("*", a, b);
  inline operator *(a: int(64), b: int(64)) do return __primitive("*", a, b);

  inline operator *(a: uint(8), b: uint(8)) do return __primitive("*", a, b);
  inline operator *(a: uint(16), b: uint(16)) do return __primitive("*", a, b);
  inline operator *(a: uint(32), b: uint(32)) do return __primitive("*", a, b);
  inline operator *(a: uint(64), b: uint(64)) do return __primitive("*", a, b);

  inline operator *(a: real(32), b: real(32)) do return __primitive("*", a, b);
  inline operator *(a: real(64), b: real(64)) do return __primitive("*", a, b);

  inline operator *(a: imag(32), b: imag(32)) do return _i2r(__primitive("*", -a, b));
  inline operator *(a: imag(64), b: imag(64)) do return _i2r(__primitive("*", -a, b));

  inline operator *(a: complex(64), b: complex(64)) do return __primitive("*", a, b);
  inline operator *(a: complex(128), b: complex(128)) do return __primitive("*", a, b);

  inline operator *(a: real(32), b: imag(32)) do return _r2i(a*_i2r(b));
  inline operator *(a: real(64), b: imag(64)) do return _r2i(a*_i2r(b));

  inline operator *(a: imag(32), b: real(32)) do return _r2i(_i2r(a)*b);
  inline operator *(a: imag(64), b: real(64)) do return _r2i(_i2r(a)*b);

  inline operator *(a: real(32), b: complex(64)) do return (a*b.re, a*b.im) : complex(64);
  inline operator *(a: real(64), b: complex(128)) do return (a*b.re, a*b.im) : complex(128);

  inline operator *(a: complex(64), b: real(32)) do return (a.re*b, a.im*b) : complex(64);
  inline operator *(a: complex(128), b: real(64)) do return (a.re*b, a.im*b) : complex(128);

  inline operator *(a: imag(32), b: complex(64)) do return (-_i2r(a)*b.im, _i2r(a)*b.re) : complex(64);
  inline operator *(a: imag(64), b: complex(128)) do return (-_i2r(a)*b.im, _i2r(a)*b.re) : complex(128);

  inline operator *(a: complex(64), b: imag(32)) do return (-a.im*_i2r(b), a.re*_i2r(b)) : complex(64);
  inline operator *(a: complex(128), b: imag(64)) do return (-a.im*_i2r(b), a.re*_i2r(b)) : complex(128);

  inline operator /(a: int(8), b: int(8)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(a: int(16), b: int(16)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(a: int(32), b: int(32)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(a: int(64), b: int(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }

  inline operator /(a: uint(8), b: uint(8)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(a: uint(16), b: uint(16)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(a: uint(32), b: uint(32)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(a: uint(64), b: uint(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }

  inline operator /(a: real(32), b: real(32)) do return __primitive("/", a, b);
  inline operator /(a: real(64), b: real(64)) do return __primitive("/", a, b);

  inline operator /(a: imag(32), b: imag(32)) do return _i2r(__primitive("/", a, b));
  inline operator /(a: imag(64), b: imag(64)) do return _i2r(__primitive("/", a, b));

  inline operator /(a: complex(64), b: complex(64)) do return __primitive("/", a, b);
  inline operator /(a: complex(128), b: complex(128)) do return __primitive("/", a, b);

  inline operator /(a: real(32), b: imag(32)) do return _r2i(-a/_i2r(b));
  inline operator /(a: real(64), b: imag(64)) do return _r2i(-a/_i2r(b));

  inline operator /(a: imag(32), b: real(32)) do return _r2i(_i2r(a)/b);
  inline operator /(a: imag(64), b: real(64)) do return _r2i(_i2r(a)/b);

  inline operator /(a: real(32), b: complex(64)) {
    const d = abs(b);
    return ((a/d)*(b.re/d), (-a/d)*(b.im/d)):complex(64);
  }
  inline operator /(a: real(64), b: complex(128)) {
    const d = abs(b);
    return ((a/d)*(b.re/d), (-a/d)*(b.im/d)):complex(128);
  }

  inline operator /(a: complex(64), b: real(32)) do return (a.re/b, a.im/b) : complex(64);
  inline operator /(a: complex(128), b: real(64)) do return (a.re/b, a.im/b) : complex(128);

  inline operator /(a: imag(32), b: complex(64)) {
    const d = abs(b);
    return ((_i2r(a)/d)*(b.im/d), (_i2r(a)/d)*(b.re/d)):complex(64);
  }
  inline operator /(a: imag(64), b: complex(128)) {
    const d = abs(b);
    return ((_i2r(a)/d)*(b.im/d), (_i2r(a)/d)*(b.re/d)):complex(128);
  }

  inline operator /(a: complex(64), b: imag(32)) do return (a.im/_i2r(b), -a.re/_i2r(b)) : complex(64);
  inline operator /(a: complex(128), b: imag(64)) do return (a.im/_i2r(b), -a.re/_i2r(b)) : complex(128);

  inline operator *(param a: int(8), param b: int(8)) param do return __primitive("*", a, b);
  inline operator *(param a: int(16), param b: int(16)) param do return __primitive("*", a, b);
  inline operator *(param a: int(32), param b: int(32)) param do return __primitive("*", a, b);
  inline operator *(param a: int(64), param b: int(64)) param do return __primitive("*", a, b);

  inline operator *(param a: uint(8), param b: uint(8)) param do return __primitive("*", a, b);
  inline operator *(param a: uint(16), param b: uint(16)) param do return __primitive("*", a, b);
  inline operator *(param a: uint(32), param b: uint(32)) param do return __primitive("*", a, b);
  inline operator *(param a: uint(64), param b: uint(64)) param do return __primitive("*", a, b);

  inline operator *(param a: real(32), param b: real(32)) param do return __primitive("*", a, b);
  inline operator *(param a: real(64), param b: real(64)) param do return __primitive("*", a, b);

  inline operator *(param a: imag(32), param b: imag(32)) param do return __primitive("*", -a, b) : real(32);
  inline operator *(param a: imag(64), param b: imag(64)) param do return __primitive("*", -a, b) : real(64);

  inline operator *(param a: real(32), param b: imag(32)) param do return __primitive("*", a, b : real(32)) : imag(32);
  inline operator *(param a: real(64), param b: imag(64)) param do return __primitive("*", a, b : real(64)) : imag(64);

  inline operator *(param a: imag(32), param b: real(32)) param do return __primitive("*", a : real(32), b) : imag(32);
  inline operator *(param a: imag(64), param b: real(64)) param do return __primitive("*", a : real(64), b) : imag(64);

  inline operator /(param a: int(8), param b: int(8)) param {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(param a: int(16), param b: int(16)) param {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(param a: int(32), param b: int(32)) param {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(param a: int(64), param b: int(64)) param {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }

  inline operator /(param a: uint(8), param b: uint(8)) param {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(param a: uint(16), param b: uint(16)) param {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(param a: uint(32), param b: uint(32)) param {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(param a: uint(64), param b: uint(64)) param {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }

  inline operator /(param a: real(32), param b: real(32)) param do return __primitive("/", a, b);
  inline operator /(param a: real(64), param b: real(64)) param do return __primitive("/", a, b);

  inline operator /(param a: imag(32), param b: imag(32)) param do return __primitive("/", a, b) : real(32);
  inline operator /(param a: imag(64), param b: imag(64)) param do return __primitive("/", a, b) : real(64);

  inline operator /(param a: real(32), param b: imag(32)) param do return __primitive("/", -a, b : real(32)) : imag(32);
  inline operator /(param a: real(64), param b: imag(64)) param do return __primitive("/", -a, b : real(64)) : imag(64);

  inline operator /(param a: imag(32), param b: real(32)) param do return __primitive("/", a : real(32), b) : imag(32);
  inline operator /(param a: imag(64), param b: real(64)) param do return __primitive("/", a : real(64), b) : imag(64);


  //
  // % on primitive types
  //

  inline operator %(a: int(8), b: int(8)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(a: int(16), b: int(16)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(a: int(32), b: int(32)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(a: int(64), b: int(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }

  inline operator %(a: uint(8), b: uint(8)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(a: uint(16), b: uint(16)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(a: uint(32), b: uint(32)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(a: uint(64), b: uint(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }

  inline operator %(param a: int(8), param b: int(8)) param {
    if b == 0 then
      compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(param a: int(16), param b: int(16)) param {
    if b == 0 then
      compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(param a: int(32), param b: int(32)) param {
    if b == 0 then
      compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(param a: int(64), param b: int(64)) param {
    if b == 0 then
      compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }

  inline operator %(param a: uint(8), param b: uint(8)) param {
    if b == 0 then
      compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(param a: uint(16), param b: uint(16)) param {
    if b == 0 then
      compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(param a: uint(32), param b: uint(32)) param {
    if b == 0 then
      compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(param a: uint(64), param b: uint(64)) param {
    if b == 0 then
      compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }

  //
  // ** on primitive types
  //

  inline proc _intExpHelp(a: integral, b) where a.type == b.type {
    if isIntType(b.type) && b < 0 then
      if a == 0 then
        halt("cannot compute ", a, " ** ", b);
      else if a == 1 then
        return 1;
      else if a == -1 then
        return if b % 2 == 0 then 1 else -1;
      else
        return 0;
    var i = b, y:a.type = 1, z = a;
    while i != 0 {
      if i % 2 == 1 then
        y *= z;
      z *= z;
      i /= 2;
    }
    return y;
  }

  inline operator **(a: int(8), b: int(8)) do return _intExpHelp(a, b);
  inline operator **(a: int(16), b: int(16)) do return _intExpHelp(a, b);
  inline operator **(a: int(32), b: int(32)) do return _intExpHelp(a, b);
  inline operator **(a: int(64), b: int(64)) do return _intExpHelp(a, b);

  inline operator **(a: uint(8), b: uint(8)) do return _intExpHelp(a, b);
  inline operator **(a: uint(16), b: uint(16)) do return _intExpHelp(a, b);
  inline operator **(a: uint(32), b: uint(32)) do return _intExpHelp(a, b);
  inline operator **(a: uint(64), b: uint(64)) do return _intExpHelp(a, b);

  inline operator **(a: real(32), b: real(32)) do return __primitive("**", a, b);
  inline operator **(a: real(64), b: real(64)) do return __primitive("**", a, b);

  inline operator **(a: complex(64), b: complex(64)) {
    pragma "fn synchronization free"
    extern proc cpowf(x: complex(64), y: complex(64)): complex(64);
    return cpowf(a, b);
  }
  inline operator **(a: complex(128), b: complex(128)) {
    pragma "fn synchronization free"
    extern proc cpow(x: complex(128), y: complex(128)): complex(128);
    return cpow(a, b);
  }

  operator **(param a: int(8), param b: int(8)) param {
    if a == 0 && b < 0 then
      compilerError("0 cannot be raised to a negative power");
    return __primitive("**", a, b);
  }
  operator **(param a: int(16), param b: int(16)) param {
    if a == 0 && b < 0 then
      compilerError("0 cannot be raised to a negative power");
    return __primitive("**", a, b);
  }
  operator **(param a: int(32), param b: int(32)) param {
    if a == 0 && b < 0 then
      compilerError("0 cannot be raised to a negative power");
    return __primitive("**", a, b);
  }
  operator **(param a: int(64), param b: int(64)) param {
    if a == 0 && b < 0 then
      compilerError("0 cannot be raised to a negative power");
    return __primitive("**", a, b);
  }

  operator **(param a: uint(8), param b: uint(8)) param do return __primitive("**", a, b);
  operator **(param a: uint(16), param b: uint(16)) param do return __primitive("**", a, b);
  operator **(param a: uint(32), param b: uint(32)) param do return __primitive("**", a, b);
  operator **(param a: uint(64), param b: uint(64)) param do return __primitive("**", a, b);

  inline proc _expHelp(a, param b: integral) {
    if b == 0 {
      return 1:a.type;
    } else if b == 1 {
      return a;
    } else if b == 2 {
      return a*a;
    } else if b == 3 {
      return a*a*a;
    } else if b == 4 {
      const t = a*a;
      return t*t;
    } else if b == 5 {
      const t = a*a;
      return t*t*a;
    } else if b == 6 {
      const t = a*a;
      return t*t*t;
    } else if b == 8 {
      const t = a*a, u = t*t;
      return u*u;
    }
    else
      compilerError("unexpected case in exponentiation optimization");
  }

  inline proc _expBaseHelp(param a: int, b) where _basePowerTwo(a) {
    if b == 0 then
      return 1:a.type;
    if b < 0 then
      if a == 1 then // "where _basePowerTwo(a)" means 'a' cannot be <= 0
        return 1;
      else
        return 0;
    var c = 0;
    var x: int = a;
    while (x > 1) // shift right to count the power
    {
      c += 1;
      x = x >> 1;
    }
    var exp = c * (b-1);
    return a << exp;
  }

  proc _canOptimizeExp(param b: integral) param do return b >= 0 && b <= 8 && b != 7;

  // complement and compare is an efficient way to test for a power of 2
  proc _basePowerTwo(param a: integral) param do return (a > 0 && ((a & (~a + 1)) == a));

  inline operator **(a: int(?w), param b: integral) where _canOptimizeExp(b) do return _expHelp(a, b);
  inline operator **(a: uint(?w), param b: integral) where _canOptimizeExp(b) do return _expHelp(a, b);
  inline operator **(a: real(?w), param b: integral) where _canOptimizeExp(b) do return _expHelp(a, b);
  inline operator **(param a: integral, b: int) where _basePowerTwo(a) do return _expBaseHelp(a, b);

  //
  // logical operations on primitive types
  //
  inline operator !(a: bool) do return __primitive("u!", a);
  inline operator !(a: int(?w)) do return (a == 0);
  inline operator !(a: uint(?w)) do return (a == 0);

  inline proc isTrue(a: bool) do return a;
  inline proc isTrue(param a: bool) param do return a;

  proc isTrue(a: integral) { compilerError("short-circuiting logical operators not supported on integers"); }

  inline operator !(param a: bool) param do return __primitive("u!", a);
  inline operator !(param a: int(?w)) param do return (a == 0);
  inline operator !(param a: uint(?w)) param do return (a == 0);

  //
  // bitwise operations on primitive types
  //
  inline operator ~(a: int(?w)) do return __primitive("u~", a);
  inline operator ~(a: uint(?w)) do return __primitive("u~", a);
  inline operator ~(a: bool) { compilerError("~ is not supported on operands of boolean type"); }

  inline operator &(a: bool, b: bool) do return __primitive("&", a, b);

  inline operator &(a: int(8), b: int(8)) do return __primitive("&", a, b);
  inline operator &(a: int(16), b: int(16)) do return __primitive("&", a, b);
  inline operator &(a: int(32), b: int(32)) do return __primitive("&", a, b);
  inline operator &(a: int(64), b: int(64)) do return __primitive("&", a, b);

  inline operator &(a: uint(8), b: uint(8)) do return __primitive("&", a, b);
  inline operator &(a: uint(16), b: uint(16)) do return __primitive("&", a, b);
  inline operator &(a: uint(32), b: uint(32)) do return __primitive("&", a, b);
  inline operator &(a: uint(64), b: uint(64)) do return __primitive("&", a, b);

  inline operator &(a: uint(8), b: int(8)) do return __primitive("&", a, b : uint(8));
  inline operator &(a: uint(16), b: int(16)) do return __primitive("&", a, b : uint(16));
  inline operator &(a: uint(32), b: int(32)) do return __primitive("&", a, b : uint(32));
  inline operator &(a: uint(64), b: int(64)) do return __primitive("&", a, b : uint(64));

  inline operator &(a: int(8), b: uint(8)) do return __primitive("&", a : uint(8), b);
  inline operator &(a: int(16), b: uint(16)) do return __primitive("&", a : uint(16), b);
  inline operator &(a: int(32), b: uint(32)) do return __primitive("&", a : uint(32), b);
  inline operator &(a: int(64), b: uint(64)) do return __primitive("&", a : uint(64), b);

  inline operator |(a: bool, b: bool) do return __primitive("|", a, b);

  inline operator |(a: int(8), b: int(8)) do return __primitive("|", a, b);
  inline operator |(a: int(16), b: int(16)) do return __primitive("|", a, b);
  inline operator |(a: int(32), b: int(32)) do return __primitive("|", a, b);
  inline operator |(a: int(64), b: int(64)) do return __primitive("|", a, b);

  inline operator |(a: uint(8), b: uint(8)) do return __primitive("|", a, b);
  inline operator |(a: uint(16), b: uint(16)) do return __primitive("|", a, b);
  inline operator |(a: uint(32), b: uint(32)) do return __primitive("|", a, b);
  inline operator |(a: uint(64), b: uint(64)) do return __primitive("|", a, b);

  inline operator |(a: uint(8), b: int(8)) do return __primitive("|", a, b : uint(8));
  inline operator |(a: uint(16), b: int(16)) do return __primitive("|", a, b : uint(16));
  inline operator |(a: uint(32), b: int(32)) do return __primitive("|", a, b : uint(32));
  inline operator |(a: uint(64), b: int(64)) do return __primitive("|", a, b : uint(64));

  inline operator |(a: int(8), b: uint(8)) do return __primitive("|", a : uint(8), b);
  inline operator |(a: int(16), b: uint(16)) do return __primitive("|", a : uint(16), b);
  inline operator |(a: int(32), b: uint(32)) do return __primitive("|", a : uint(32), b);
  inline operator |(a: int(64), b: uint(64)) do return __primitive("|", a : uint(64), b);

  inline operator ^(a: bool, b: bool) do return __primitive("^", a, b);

  inline operator ^(a: int(8), b: int(8)) do return __primitive("^", a, b);
  inline operator ^(a: int(16), b: int(16)) do return __primitive("^", a, b);
  inline operator ^(a: int(32), b: int(32)) do return __primitive("^", a, b);
  inline operator ^(a: int(64), b: int(64)) do return __primitive("^", a, b);

  inline operator ^(a: uint(8), b: uint(8)) do return __primitive("^", a, b);
  inline operator ^(a: uint(16), b: uint(16)) do return __primitive("^", a, b);
  inline operator ^(a: uint(32), b: uint(32)) do return __primitive("^", a, b);
  inline operator ^(a: uint(64), b: uint(64)) do return __primitive("^", a, b);

  inline operator ^(a: uint(8), b: int(8)) do return __primitive("^", a, b : uint(8));
  inline operator ^(a: uint(16), b: int(16)) do return __primitive("^", a, b : uint(16));
  inline operator ^(a: uint(32), b: int(32)) do return __primitive("^", a, b : uint(32));
  inline operator ^(a: uint(64), b: int(64)) do return __primitive("^", a, b : uint(64));

  inline operator ^(a: int(8), b: uint(8)) do return __primitive("^", a : uint(8), b);
  inline operator ^(a: int(16), b: uint(16)) do return __primitive("^", a : uint(16), b);
  inline operator ^(a: int(32), b: uint(32)) do return __primitive("^", a : uint(32), b);
  inline operator ^(a: int(64), b: uint(64)) do return __primitive("^", a : uint(64), b);

  inline operator ~(param a: bool) { compilerError("~ is not supported on operands of boolean type"); }
  inline operator ~(param a: int(?w)) param do return __primitive("u~", a);
  inline operator ~(param a: uint(?w)) param do return __primitive("u~", a);

  inline operator &(param a: bool, param b: bool) param do return __primitive("&", a, b);

  inline operator &(param a: int(8), param b: int(8)) param do return __primitive("&", a, b);
  inline operator &(param a: int(16), param b: int(16)) param do return __primitive("&", a, b);
  inline operator &(param a: int(32), param b: int(32)) param do return __primitive("&", a, b);
  inline operator &(param a: int(64), param b: int(64)) param do return __primitive("&", a, b);

  inline operator &(param a: uint(8), param b: uint(8)) param do return __primitive("&", a, b);
  inline operator &(param a: uint(16), param b: uint(16)) param do return __primitive("&", a, b);
  inline operator &(param a: uint(32), param b: uint(32)) param do return __primitive("&", a, b);
  inline operator &(param a: uint(64), param b: uint(64)) param do return __primitive("&", a, b);

  inline operator &(param a: uint(8), param b: int(8)) param do return __primitive("&", a, b : uint(8));
  inline operator &(param a: uint(16), param b: int(16)) param do return __primitive("&", a, b : uint(16));
  inline operator &(param a: uint(32), param b: int(32)) param do return __primitive("&", a, b : uint(32));
  inline operator &(param a: uint(64), param b: int(64)) param do return __primitive("&", a, b : uint(64));

  inline operator &(param a: int(8), param b: uint(8)) param do return __primitive("&", a : uint(8), b);
  inline operator &(param a: int(16), param b: uint(16)) param do return __primitive("&", a : uint(16), b);
  inline operator &(param a: int(32), param b: uint(32)) param do return __primitive("&", a : uint(32), b);
  inline operator &(param a: int(64), param b: uint(64)) param do return __primitive("&", a : uint(64), b);

  inline operator |(param a: bool, param b: bool) param do return __primitive("|", a, b);

  inline operator |(param a: int(8), param b: int(8)) param do return __primitive("|", a, b);
  inline operator |(param a: int(16), param b: int(16)) param do return __primitive("|", a, b);
  inline operator |(param a: int(32), param b: int(32)) param do return __primitive("|", a, b);
  inline operator |(param a: int(64), param b: int(64)) param do return __primitive("|", a, b);

  inline operator |(param a: uint(8), param b: uint(8)) param do return __primitive("|", a, b);
  inline operator |(param a: uint(16), param b: uint(16)) param do return __primitive("|", a, b);
  inline operator |(param a: uint(32), param b: uint(32)) param do return __primitive("|", a, b);
  inline operator |(param a: uint(64), param b: uint(64)) param do return __primitive("|", a, b);

  inline operator |(param a: uint(8), param b: int(8)) param do return __primitive("|", a, b : uint(8));
  inline operator |(param a: uint(16), param b: int(16)) param do return __primitive("|", a, b : uint(16));
  inline operator |(param a: uint(32), param b: int(32)) param do return __primitive("|", a, b : uint(32));
  inline operator |(param a: uint(64), param b: int(64)) param do return __primitive("|", a, b : uint(64));

  inline operator |(param a: int(8), param b: uint(8)) param do return __primitive("|", a : uint(8), b);
  inline operator |(param a: int(16), param b: uint(16)) param do return __primitive("|", a : uint(16), b);
  inline operator |(param a: int(32), param b: uint(32)) param do return __primitive("|", a : uint(32), b);
  inline operator |(param a: int(64), param b: uint(64)) param do return __primitive("|", a : uint(64), b);

  inline operator ^(param a: bool, param b: bool) param do return __primitive("^", a, b);

  inline operator ^(param a: int(8), param b: int(8)) param do return __primitive("^", a, b);
  inline operator ^(param a: int(16), param b: int(16)) param do return __primitive("^", a, b);
  inline operator ^(param a: int(32), param b: int(32)) param do return __primitive("^", a, b);
  inline operator ^(param a: int(64), param b: int(64)) param do return __primitive("^", a, b);

  inline operator ^(param a: uint(8), param b: uint(8)) param do return __primitive("^", a, b);
  inline operator ^(param a: uint(16), param b: uint(16)) param do return __primitive("^", a, b);
  inline operator ^(param a: uint(32), param b: uint(32)) param do return __primitive("^", a, b);
  inline operator ^(param a: uint(64), param b: uint(64)) param do return __primitive("^", a, b);

  inline operator ^(param a: uint(8), param b: int(8)) param do return __primitive("^", a, b : uint(8));
  inline operator ^(param a: uint(16), param b: int(16)) param do return __primitive("^", a, b : uint(16));
  inline operator ^(param a: uint(32), param b: int(32)) param do return __primitive("^", a, b : uint(32));
  inline operator ^(param a: uint(64), param b: int(64)) param do return __primitive("^", a, b : uint(64));

  inline operator ^(param a: int(8), param b: uint(8)) param do return __primitive("^", a : uint(8), b);
  inline operator ^(param a: int(16), param b: uint(16)) param do return __primitive("^", a : uint(16), b);
  inline operator ^(param a: int(32), param b: uint(32)) param do return __primitive("^", a : uint(32), b);
  inline operator ^(param a: int(64), param b: uint(64)) param do return __primitive("^", a : uint(64), b);

  //
  // left and right shift on primitive types
  //

  inline proc bitshiftChecks(a, b: integral) {
    use HaltWrappers;

    if b < 0 {
      var msg = "Cannot bitshift " + a:string + " by " + b:string +
                " because " + b:string + " is less than 0";
      HaltWrappers.boundsCheckHalt(msg);
    } else if b >= numBits(a.type) {
      var msg = "Cannot bitshift " + a:string + " by " + b:string +
                " because " + b:string + " is >= the bitwidth of " +
                a.type:string;
      HaltWrappers.boundsCheckHalt(msg);
    }
  }

  inline proc bitshiftChecks(param a, param b: integral) {
    if b < 0 {
      param msg = "Cannot bitshift " + a:string + " by " + b:string +
                  " because " + b:string + " is less than 0";
      compilerError(msg);
    } else if b >= numBits(a.type) {
      param msg = "Cannot bitshift " + a:string + " by " + b:string +
                  " because " + b:string + " is >= the bitwidth of " +
                  a.type:string;
      compilerError(msg);
    }
  }

  inline operator <<(a: int(?w), b: integral) {
    if boundsChecking then bitshiftChecks(a, b);
    // Intentionally cast `a` to `uint(w)` for an unsigned left shift.
    return __primitive("<<", a:uint(w), b):int(w);
  }

  inline operator <<(a: uint(?w), b: integral) {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive("<<", a, b);
  }

  inline operator >>(a: int(?w), b: integral) {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive(">>", a, b);
  }

  inline operator >>(a: uint(?w), b: integral) {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive(">>", a, b);
  }

  inline operator <<(param a: int(?w), param b: integral) param {
    if boundsChecking then bitshiftChecks(a, b);
    // Intentionally cast `a` to `uint(w)` for an unsigned left shift.
    return __primitive("<<", a:uint(w), b):int(w);
  }

  inline operator <<(param a: uint(?w), param b: integral) param {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive("<<", a, b);
  }

  inline operator >>(param a: int(?w), param b: integral) param {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive(">>", a, b);
  }

  inline operator >>(param a: uint(?w), param b: integral) param {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive(">>", a, b);
  }

  pragma "always propagate line file info"
  private inline proc checkNotNil(x:borrowed class?) {
    import HaltWrappers;
    // Check only if --nil-checks is enabled or user requested
    if chpl_checkNilDereferences || enablePostfixBangChecks {
      // Add check for nilable types only.
      if x == nil {
        HaltWrappers.nilCheckHalt("argument to ! is nil");
      }
    }
  }

  inline proc postfix!(x:unmanaged class) {
    return _to_nonnil(x);
  }
  inline proc postfix!(x:borrowed class) {
    return _to_nonnil(x);
  }

  pragma "always propagate line file info"
  inline proc postfix!(x:unmanaged class?) {
    checkNotNil(_to_borrowed(x));
    return _to_nonnil(x);
  }
  pragma "always propagate line file info"
  inline proc postfix!(x:borrowed class?) {
    checkNotNil(x);
    return _to_nonnil(x);
  }

  pragma "last resort"
  proc postfix!(x) {
    compilerError("postfix ! can only apply to classes");
  }

  //
  // These functions are used to implement the semantics of
  // reading a sync/single var when the variable is not actually
  // assigned to anything.  For example, a statement that simply uses
  // a sync to read it or a sync returned from a function but not
  // explicitly captured.
  //
  inline proc chpl_statementLevelSymbol(a) { }
  inline proc chpl_statementLevelSymbol(a: sync)  {
    compilerWarning("implicitly reading from a sync is deprecated; apply a '.read??()' method");
    a.readFE();
  }
  // param and type args are handled in the compiler

  //
  // If an iterator is called without capturing the result, iterate over it
  // to ensure any side effects it has will happen.
  //
  inline proc chpl_statementLevelSymbol(ir: _iteratorRecord) {
    iter _ir_copy_recursive(ir) {
      for e in ir do
        yield chpl__initCopy(e, definedConst=false);
    }

    pragma "no copy" var irc = _ir_copy_recursive(ir);
    for e in irc { }
  }

  //
  // _cond_test function supports statement bool conversions and sync
  //   variables in conditional statements; and checks for errors
  // _cond_invalid function checks a conditional expression for
  //   incorrectness; it is used to give better error messages for
  //   promotion of && and ||
  //
  // These are written with two generic functions
  // to avoid problems with preference between implicitly
  // converting and using a generic fall-back.

  inline proc _cond_test(param x: ?t) param : bool {
    if isCoercible(t, bool) {
      return x;
    } else if (isCoercible(t, int) || isCoercible(t, uint)) {
      return x != 0:x.type;
    } else {
      compilerError("invalid type ", t:string, " used in if or while condition");
    }
  }

  inline proc _cond_test(x: ?t): bool {
    if isSubtype(t, sync(?)) {
      compilerWarning("direct reads of sync variables are deprecated; please apply a 'read??' method");
      return _cond_test(x.readFE());
    } else if isCoercible(t, borrowed RootClass?) {
      return x != nil;
    } else if isCoercible(t, bool) {
      return x;
    } else if isCoercible(t, int) || isCoercible(t, uint) {
      return x != 0;
    } else if isSubtype(t, c_ptr) {
      return x != nil;
    } else {
      use Reflection;
      if canResolveMethod(x, "chpl_cond_test_method") {
        return x.chpl_cond_test_method();
      } else {
        if isSubtype(t, _iteratorRecord) {
          compilerError("iterator or promoted expression iterator used in if or while condition");
        } else {
          compilerError("type '", x.type:string, "' used in if or while condition");
        }
      }
    }
  }

  proc _cond_invalid(x: borrowed RootClass?) param do return false;
  proc _cond_invalid(x: bool) param do return false;
  proc _cond_invalid(x: int) param do return false;
  proc _cond_invalid(x: uint) param do return false;
  pragma "last resort"
  proc _cond_invalid(x) param do return true;

  //
  // isNonnegative(i) == (i>=0), but is a param value if i is unsigned.
  //
  inline proc isNonnegative(i: int(?)) do return i >= 0;
  inline proc isNonnegative(i: uint(?)) param do return true;
  inline proc isNonnegative(param i) param do return i >= 0;


  //
  // complex component methods re and im
  //
  inline proc ref chpl_anycomplex.re ref {
    return __primitive("complex_get_real", this);
  }
  proc param chpl_anycomplex.re param {
    return __primitive("complex_get_real", this);
  }
  inline proc chpl_anycomplex.re {
    if this.type == complex(128) {
      pragma "fn synchronization free"
      pragma "codegen for CPU and GPU"
      extern proc chpl_creal(x:complex(128)): real(64);
      return chpl_creal(this);
    } else {
      pragma "fn synchronization free"
      pragma "codegen for CPU and GPU"
      extern proc chpl_crealf(x:complex(64)): real(32);
      return chpl_crealf(this);
    }
  }
  inline proc ref chpl_anycomplex.im ref {
    return __primitive("complex_get_imag", this);
  }
  proc param chpl_anycomplex.im param {
    return __primitive("complex_get_imag", this);
  }
  inline proc chpl_anycomplex.im {
    if this.type == complex(128) {
      pragma "fn synchronization free"
      pragma "codegen for CPU and GPU"
      extern proc chpl_cimag(x:complex(128)): real(64);
      return chpl_cimag(this);
    } else {
      pragma "fn synchronization free"
      pragma "codegen for CPU and GPU"
      extern proc chpl_cimagf(x:complex(64)): real(32);
      return chpl_cimagf(this);
    }
  }

  //
  // helper functions
  //
  inline proc _i2r(a: imag(?w)) do return __primitive("cast", real(w), a);
  inline proc _r2i(a: real(?w)) do return __primitive("cast", imag(w), a);

  //
  // More primitive funs
  //
  enum ArrayInit {heuristicInit, noInit, serialInit, parallelInit, gpuInit};
  config param chpl_defaultArrayInitMethod = ArrayInit.heuristicInit;
  config param chpl_defaultGpuArrayInitMethod =
    if CHPL_GPU_MEM_STRATEGY == "array_on_device" then
      ArrayInit.gpuInit else chpl_defaultArrayInitMethod;

  config param chpl_arrayInitMethodRuntimeSelectable = false;
  private var chpl_arrayInitMethod = chpl_defaultArrayInitMethod;

  inline proc chpl_setArrayInitMethod(initMethod: ArrayInit) {
    if chpl_arrayInitMethodRuntimeSelectable == false {
      compilerWarning("must set 'chpl_arrayInitMethodRuntimeSelectable' for ",
                      "'chpl_setArrayInitMethod' to have any effect");
    }
    const oldInit = chpl_arrayInitMethod;
    chpl_arrayInitMethod = initMethod;
    return oldInit;
  }

  inline proc chpl_getArrayInitMethod() {
    if chpl_arrayInitMethodRuntimeSelectable == false {
      return chpl_defaultArrayInitMethod;
    } else {
      return chpl_arrayInitMethod;
    }
  }

  proc chpl_shouldDoGpuInit(): bool {
    pragma "codegen for CPU and GPU"
    extern proc chpl_task_getRequestedSubloc(): int(32);
    return
      CHPL_LOCALE_MODEL=="gpu" &&
      chpl_defaultGpuArrayInitMethod == ArrayInit.gpuInit &&
      chpl_task_getRequestedSubloc() >= 0;
  }

  // s is the number of elements, t is the element type
  proc init_elts_method(s, type t) {
    var initMethod = chpl_getArrayInitMethod();

    if s == 0 {
      // Skip init for empty arrays. Needed for uints so that `s-1` in init_elts
      // code doesn't overflow.
      initMethod = ArrayInit.noInit;
    } else if chpl_shouldDoGpuInit() {
        initMethod = ArrayInit.gpuInit;
    } else if  !rootLocaleInitialized {
      // The parallel range iter uses 'here`/rootLocale, so fallback to serial
      // initialization if the root locale hasn't been setup. Only used early
      // in module initialization
      initMethod = ArrayInit.serialInit;
    } else if initMethod == ArrayInit.heuristicInit {
      // Heuristically determine if we should do parallel initialization. The
      // current heuristic really just checks that we have an array that's at
      // least 2MB. This value was chosen experimentally: Any smaller and the
      // cost of a forall (mostly the task creation) outweighs the benefit of
      // using multiple tasks. This was tested on a 2 core laptop, 8 core
      // workstation, and 24 core XC40.
      const elemsizeInBytes = if isNumericType(t) then numBytes(t)
                              else c_sizeof(t).safeCast(int);
      const arrsizeInBytes = s.safeCast(int) * elemsizeInBytes;
      param heuristicThresh = 2 * 1024 * 1024;
      const heuristicWantsPar = arrsizeInBytes > heuristicThresh;

      if heuristicWantsPar {
        initMethod = ArrayInit.parallelInit;
      } else {
        initMethod = ArrayInit.serialInit;
      }
    }

    return initMethod;
  }

  proc init_elts(x, s, type t, lo=0:s.type) : void {

    var initMethod = init_elts_method(s, t);

    // Q: why is the declaration of 'y' in the following loops?
    //
    // A: so that if the element type is something like an array,
    // the element can 'steal' the array rather than copying it.
    // One effect of having it in the loop is that the reference
    // count for an array element's domain gets bumped once per
    // element.  Is this good, bad, necessary?  Unclear.
    select initMethod {
      when ArrayInit.noInit {
        return;
      }
      when ArrayInit.serialInit {
        for i in lo..s-1 {
          pragma "no auto destroy" pragma "unsafe" var y: t;
          __primitive("array_set_first", x, i, y);
        }
      }
      when ArrayInit.gpuInit {
        // This branch should only occur when we're on a GPU sublocale and the
        // following `foreach` loop will become a kernel
        foreach i in lo..s-1 {
          //assertOnGpu(); TODO: this assertion fails for a hello world style
          // program investigate why (I don't think it's erroring out in user
          // code but rather something from one of our standard modules).
          pragma "no auto destroy" pragma "unsafe" var y: t;
          __primitive("array_set_first", x, i, y);
        }
      }
      when ArrayInit.parallelInit {
        forall i in lo..s-1 {
          pragma "no auto destroy" pragma "unsafe" var y: t;
          __primitive("array_set_first", x, i, y);
        }
      }
      otherwise {
        halt("ArrayInit.", initMethod, " should have been made concrete");
      }
    }
  }

  // TODO (EJR: 02/25/16): see if we can remove this explicit type declaration.
  // chpl_mem_descInt_t is really a well known compiler type since the compiler
  // emits calls for the chpl_mem_descs table. Maybe the compiler should just
  // create the type and export it to the runtime?
  extern type chpl_mem_descInt_t = int(16);

  enum chpl_ddataResizePolicy { normalInit, skipInit, skipInitButClearMem }

  // dynamic data block class
  // (note that c_ptr(type) is similar, but local only,
  //  and defined in CTypes.chpl)
  pragma "data class"
  pragma "no object"
  pragma "no default functions"
  class _ddata {
    type eltType;

    inline proc this(i: integral) ref {
      return __primitive("array_get", this, i);
    }
  }

  proc chpl_isDdata(type t:_ddata) param do return true;
  proc chpl_isDdata(type t) param do return false;

  inline operator =(ref a: _ddata(?t), b: _ddata(t)) {
    __primitive("=", a, b);
  }

  // Removing the 'eltType' arg results in errors for --baseline
  inline proc _ddata_shift(type eltType, data: _ddata(eltType), shift: integral) {
    var ret: _ddata(eltType);
     __primitive("shift_base_pointer", ret, data, shift);
    return ret;
  }

  inline proc _ddata_sizeof_element(type t: _ddata): c_size_t {
    return __primitive("sizeof_ddata_element", t):c_size_t;
  }

  inline proc _ddata_sizeof_element(x: _ddata): c_size_t {
    return _ddata_sizeof_element(x.type);
  }

  // Never initializes elements
  //
  // if callPostAlloc=true, then _ddata_allocate_postalloc should
  // be called after the elements are initialized.
  //
  //
  // Cyclic/Block will function OK if postAlloc isn't called yet
  // during initialization and a PUT e.g. occurs.
  //
  // List could never call postAlloc or call it immediately
  //   -> calling it immediately should result in allocating domain owning it
  //   -> never calling it should result in always using bounce buffers
  //
  // Associative array - makes sense to call postAlloc
  //  after touching memory in usual order
  //

  pragma "llvm return noalias"
  proc _ddata_allocate_noinit(type eltType, size: integral,
                              out callPostAlloc: bool,
                              subloc = c_sublocid_none,
                              haltOnOom:bool = true) {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_array_alloc(nmemb: c_size_t, eltSize: c_size_t,
                                     subloc: chpl_sublocID_t,
                                     ref callPostAlloc: bool,
                                     haltOnOom: bool): c_ptr(void);
    var ret: _ddata(eltType);
    ret = chpl_mem_array_alloc(size:c_size_t, _ddata_sizeof_element(ret),
                               subloc, callPostAlloc, haltOnOom):ret.type;
    return ret;
  }

  inline proc _try_ddata_allocate(type eltType, size: integral,
                                  subloc = c_sublocid_none) throws {
    var callPostAlloc: bool;
    var ret: _ddata(eltType);

    ret = _ddata_allocate_noinit(eltType, size, callPostAlloc, subloc, false);

    if ret == nil then
      throw new ArrayOomError();

    init_elts(ret, size, eltType);

    if callPostAlloc {
      _ddata_allocate_postalloc(ret, size);
    }

    return ret;
  }

  pragma "llvm return noalias"
  proc _ddata_allocate_noinit_gpu_shared(type eltType, size: integral,
                                         out callPostAlloc: bool,
                                         subloc = c_sublocid_none) {
    if CHPL_LOCALE_MODEL != "gpu" then
      compilerError("_ddata_allocate_noinit_gpu_shared can't be called in this config");

    var ret: _ddata(eltType);
    // TODO why fixed size?
    ret = __primitive("cast", ret.type, __primitive("gpu allocShared", 4096*8));
    return ret;
  }

  inline proc _ddata_allocate_postalloc(data:_ddata, size: integral) {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_array_postAlloc(data: c_ptr(void), nmemb: c_size_t,
                                         eltSize: c_size_t);
    chpl_mem_array_postAlloc(data:c_ptr(void), size:c_size_t,
                             _ddata_sizeof_element(data));
  }

  inline proc _ddata_allocate(type eltType, size: integral,
                              subloc = c_sublocid_none) {
    var callPostAlloc: bool;
    var ret: _ddata(eltType);

    ret = _ddata_allocate_noinit(eltType, size, callPostAlloc, subloc);

    init_elts(ret, size, eltType);

    if callPostAlloc {
      _ddata_allocate_postalloc(ret, size);
    }

    return ret;
  }

  inline proc _ddata_supports_reallocate(oldDdata,
                                         type eltType,
                                         oldSize: integral,
                                         newSize: integral) {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_array_supports_realloc(ptr: c_ptr(void),
                                                oldNmemb: c_size_t, newNmemb:
                                                c_size_t, eltSize: c_size_t): bool;
      return chpl_mem_array_supports_realloc(oldDdata: c_ptr(void),
                                             oldSize.safeCast(c_size_t),
                                             newSize.safeCast(c_size_t),
                                             _ddata_sizeof_element(oldDdata));
  }

  inline proc _ddata_fill(ddata,
                          type eltType,
                          lo: integral,
                          hi: integral,
                          fill: int(8)=0) {
    import OS.POSIX.memset;
    if hi > lo {
      const elemWidthInBytes: uint  = _ddata_sizeof_element(ddata);
      const numElems = (hi - lo).safeCast(uint);
      if safeMul(numElems, elemWidthInBytes) {
        const numBytes = numElems * elemWidthInBytes;
        const shiftedPtr = _ddata_shift(eltType, ddata, lo);
        memset(shiftedPtr:c_ptr(void), fill, numBytes.safeCast(c_size_t));
      } else {
        halt('internal error: Unsigned integer overflow during ' +
             'memset of dynamic block');
      }
    }
  }

  inline proc _ddata_reallocate(oldDdata,
                                type eltType,
                                oldSize: integral,
                                newSize: integral,
                                subloc = c_sublocid_none,
                                policy = chpl_ddataResizePolicy.normalInit) {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_array_realloc(ptr: c_ptr(void),
                                       oldNmemb: c_size_t, newNmemb: c_size_t,
                                       eltSize: c_size_t,
                                       subloc: chpl_sublocID_t,
                                       ref callPostAlloc: bool): c_ptr(void);
    var callPostAlloc: bool;

    // destroy any elements that are going away
    param needsDestroy = __primitive("needs auto destroy",
                                     __primitive("deref", oldDdata[0]));
    if needsDestroy && (oldSize > newSize) {
      if _deinitElementsIsParallel(eltType, oldSize) {
        forall i in newSize..oldSize-1 do
          chpl__autoDestroy(oldDdata[i]);
      } else {
        for i in newSize..oldSize-1 do
          chpl__autoDestroy(oldDdata[i]);
      }
    }

    var newDdata = chpl_mem_array_realloc(oldDdata: c_ptr(void),
                                          oldSize.safeCast(c_size_t),
                                          newSize.safeCast(c_size_t),
                                          _ddata_sizeof_element(oldDdata),
                                          subloc,
                                          callPostAlloc): oldDdata.type;

    // The resize policy dictates whether or not we should default-init,
    // skip initializing, or zero out the memory of new slots.
    select policy {
      when chpl_ddataResizePolicy.normalInit do
        if !isDefaultInitializable(eltType) {
          halt('internal error: Attempt to resize dynamic block ' +
               'containing non-default-initializable elements');
        } else {
          init_elts(newDdata, newSize, eltType, lo=oldSize);
        }
      when chpl_ddataResizePolicy.skipInit do;
      when chpl_ddataResizePolicy.skipInitButClearMem do
        _ddata_fill(newDdata, eltType, oldSize, newSize);
    }

    if (callPostAlloc) {
      pragma "fn synchronization free"
      pragma "insert line file info"
      extern proc chpl_mem_array_postRealloc(oldData: c_ptr(void),
                                             oldNmemb: c_size_t,
                                             newData: c_ptr(void),
                                             newNmemb: c_size_t,
                                             eltSize: c_size_t);
      chpl_mem_array_postRealloc(oldDdata:c_ptr(void), oldSize.safeCast(c_size_t),
                                 newDdata:c_ptr(void), newSize.safeCast(c_size_t),
                                 _ddata_sizeof_element(oldDdata));
    }
    return newDdata;
  }


  inline proc _ddata_free(data: _ddata, size: integral) {
    var subloc = chpl_sublocFromLocaleID(__primitive("_wide_get_locale", data));

    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_array_free(data: c_ptr(void),
                                    nmemb: c_size_t, eltSize: c_size_t,
                                    subloc: chpl_sublocID_t);
    chpl_mem_array_free(data:c_ptr(void), size:c_size_t,
                        _ddata_sizeof_element(data),
                        subloc);
  }

  inline operator ==(a: _ddata, b: _ddata)
      where _to_borrowed(a.eltType) == _to_borrowed(b.eltType) {
    return __primitive("ptr_eq", a, b);
  }
  inline operator ==(a: _ddata, b: _nilType) {
    return __primitive("ptr_eq", a, nil);
  }
  inline operator ==(a: _nilType, b: _ddata) {
    return __primitive("ptr_eq", nil, b);
  }

  inline operator !=(a: _ddata, b: _ddata) where a.eltType == b.eltType {
    return __primitive("ptr_neq", a, b);
  }
  inline operator !=(a: _ddata, b: _nilType) {
    return __primitive("ptr_neq", a, nil);
  }
  inline operator !=(a: _nilType, b: _ddata) {
    return __primitive("ptr_neq", nil, b);
  }


  inline proc _cond_test(x: _ddata) do return x != nil;


  //
  // internal reference type
  //
  pragma "ref"
  pragma "no default functions"
  pragma "no object"
  class _ref {
    var _val;
  }

  module currentTask {
    inline proc yieldExecution() {
      extern proc chpl_task_yield();
      chpl_task_yield();
    }
  }
  //
  // data structures for naive implementation of end used for
  // sync statements and for joining coforall and cobegin tasks
  //

  inline proc chpl_rt_reset_task_spawn() {
    pragma "fn synchronization free"
    extern proc chpl_task_reset_spawn_order();
    chpl_task_reset_spawn_order();
  }

  proc chpl_resetTaskSpawn(numTasks) {
    const dptpl = if dataParTasksPerLocale==0 then here.maxTaskPar
                  else dataParTasksPerLocale;

    if numTasks >= dptpl {
      chpl_rt_reset_task_spawn();
    } else if numTasks == 1 {
      // Don't create a task for local single iteration coforalls
      use ChapelTaskData;
      var tls = chpl_task_getInfoChapel();
      chpl_task_data_setNextCoStmtSerial(tls, true);
    }
  }

  config param useAtomicTaskCnt = defaultAtomicTaskCount();

  proc defaultAtomicTaskCount() param {
    use ChplConfig;
    return ChplConfig.CHPL_NETWORK_ATOMICS != "none";
  }

  config param commDiagsTrackEndCounts = false;

  pragma "no default functions"
  record endCountDiagsManager : contextManager {
    var taskInfo: c_ptr(chpl_task_infoChapel_t);
    var prevDiagsDisabledVal: bool;

    inline proc ref enterContext() {
      if !commDiagsTrackEndCounts {
        taskInfo = chpl_task_getInfoChapel();
        prevDiagsDisabledVal = chpl_task_data_setCommDiagsTemporarilyDisabled(taskInfo, true);
      }
    }

    inline proc exitContext(in unused: owned Error?) {
      if !commDiagsTrackEndCounts {
        chpl_task_data_setCommDiagsTemporarilyDisabled(taskInfo, prevDiagsDisabledVal);
      }
    }
  }

  // Parent class for _EndCount instances so that it's easy
  // to add non-generic fields here.
  // And to get 'errors' field from any generic instantiation.
  pragma "no default functions"
  class _EndCountBase {
    var errors: chpl_TaskErrors;
  }

  pragma "end count"
  pragma "no default functions"
  class _EndCount : _EndCountBase {
    type iType;
    type taskType;
    var i: iType;
    var taskCnt: taskType;
    proc init(type iType, type taskType) {
      this.iType = iType;
      this.taskType = taskType;
    }

    inline proc add(value: int, param order: memoryOrder) {
      manage new endCountDiagsManager() do
        this.i.add(value, order);
    }

    inline proc sub(value: int, param order: memoryOrder) {
      manage new endCountDiagsManager() do
        this.i.sub(value, order);
    }

    inline proc waitFor(value: int, param order: memoryOrder) {
      manage new endCountDiagsManager() do
        this.i.waitFor(value, order);
    }
  }

  // This function is called once by the initiating task.  No on
  // statement needed, because the task should be running on the same
  // locale as the sync/coforall/cobegin was initiated on and thus the
  // same locale on which the object is allocated.
  //
  // TODO: 'taskCnt' can sometimes be local even if 'i' has to be remote.
  // It is currently believed that only a remote-begin will want a network
  // atomic 'taskCnt'. There should be a separate argument to control the type
  // of 'taskCnt'.
  pragma "dont disable remote value forwarding"
  inline proc _endCountAlloc(param forceLocalTypes : bool) {
    type taskCntType = if !forceLocalTypes && useAtomicTaskCnt then atomic int
                                           else int;
    if forceLocalTypes {
      return new unmanaged _EndCount(iType=chpl__processorAtomicType(int),
                                     taskType=taskCntType);
    } else {
      return new unmanaged _EndCount(iType=chpl__atomicType(int),
                                     taskType=taskCntType);
    }
  }

  // Compiler looks for this variable to determine the return type of
  // the "get end count" primitive.
  type _remoteEndCountType = _endCountAlloc(false).type;

  // This function is called once by the initiating task.  As above, no
  // on statement needed.
  pragma "dont disable remote value forwarding"
  inline proc _endCountFree(e: _EndCount) {
    delete _to_unmanaged(e);
  }

  // This function is called by the initiating task once for each new
  // task *before* any of the tasks are started.  As above, no on
  // statement needed.
  pragma "dont disable remote value forwarding"
  pragma "no remote memory fence"
  pragma "task spawn impl fn"
  proc _upEndCount(e: _EndCount, param countRunningTasks=true) {
    if isAtomic(e.taskCnt) {
      e.add(1, memoryOrder.release);
      e.taskCnt.add(1, memoryOrder.release);
    } else {
      // note that this on statement does not have the usual
      // remote memory fence because of pragma "no remote memory fence"
      // above. So we do an acquire fence before it.
      chpl_rmem_consist_fence(memoryOrder.release);
      on e {
        e.add(1, memoryOrder.release);
        e.taskCnt += 1;
      }
    }
    if countRunningTasks {
      here.runningTaskCntAdd(1);  // decrement is in _waitEndCount()
      chpl_comm_task_create();    // countRunningTasks is a proxy for "is local"
                                  // here.  Comm layers are responsible for the
                                  // remote case themselves.
    }
  }

  pragma "dont disable remote value forwarding"
  pragma "no remote memory fence"
  pragma "task spawn impl fn"
  proc _upEndCount(e: _EndCount, param countRunningTasks=true, numTasks) {
    e.add(numTasks:int, memoryOrder.release);

    if countRunningTasks {
      if numTasks > 1 {
        here.runningTaskCntAdd(numTasks:int-1);  // decrement is in _waitEndCount()
      }
      chpl_comm_task_create();    // countRunningTasks is a proxy for "is local"
                                  // here.  Comm layers are responsible for the
                                  // remote case themselves.
    } else {
      here.runningTaskCntSub(1);
    }
  }

  extern proc chpl_comm_unordered_task_fence(): void;

  extern proc chpl_comm_task_create();

  pragma "task complete impl fn"
  extern proc chpl_comm_task_end(): void;

  pragma "compiler added remote fence"
  proc chpl_after_forall_fence() {
    chpl_comm_unordered_task_fence();
  }

  // This function is called once by each newly initiated task.  No on
  // statement is needed because the call to sub() will do a remote
  // fork (on) if needed.
  pragma "dont disable remote value forwarding"
  pragma "task complete impl fn"
  pragma "down end count fn"
  proc _downEndCount(e: _EndCount, err: unmanaged Error?) {
    chpl_save_task_error(e, err);
    if CHPL_LOCALE_MODEL == "gpu" {
      pragma "task complete impl fn"
      extern proc chpl_gpu_task_end(): void;

      chpl_gpu_task_end();
    }
    chpl_comm_task_end();
    // inform anybody waiting that we're done
    e.sub(1, memoryOrder.release);
  }

  // This function is called once by the initiating task.  As above, no
  // on statement needed.
  // called for sync blocks (implicit or explicit), unbounded coforalls
  pragma "dont disable remote value forwarding"
  pragma "task join impl fn"
  pragma "unchecked throws"
  proc _waitEndCount(e: _EndCount, param countRunningTasks=true) throws {
    // Remove the task that will just be waiting/yielding in the following
    // waitFor() from the running task count to let others do real work. It is
    // re-added after the waitFor().
    here.runningTaskCntSub(1);

    // Wait for all tasks to finish
    e.waitFor(0, memoryOrder.acquire);

    if countRunningTasks {
      const taskDec = if isAtomic(e.taskCnt) then e.taskCnt.read() else e.taskCnt;
      // taskDec-1 to adjust for the task that was waiting for others to finish
      here.runningTaskCntSub(taskDec-1);  // increment is in _upEndCount()
    } else {
      // re-add the task that was waiting for others to finish
      here.runningTaskCntAdd(1);
    }

    // Throw any error raised by a task this is waiting for
    if ! e.errors.empty() then
      throw new owned TaskErrors(e.errors);
  }

  // called for bounded coforalls and cobegins
  pragma "dont disable remote value forwarding"
  pragma "task join impl fn"
  pragma "unchecked throws"
  proc _waitEndCount(e: _EndCount, param countRunningTasks=true, numTasks) throws {
    // Wait for all tasks to finish
    e.waitFor(0, memoryOrder.acquire);

    if countRunningTasks {
      if numTasks > 1 {
        here.runningTaskCntSub(numTasks:int-1);
      }
    } else {
      here.runningTaskCntAdd(1);
    }

    // Throw any error raised by a task this is waiting for
    if ! e.errors.empty() then
      throw new owned TaskErrors(e.errors);
  }

  pragma "task spawn impl fn"
  proc _upDynamicEndCount(param countRunningTasks=true) {
    var e = __primitive("get dynamic end count");
    _upEndCount(e, countRunningTasks);
  }

  pragma "dont disable remote value forwarding"
  pragma "task complete impl fn"
  pragma "down end count fn"
  proc _downDynamicEndCount(err: unmanaged Error?) {
    var e = __primitive("get dynamic end count");
    _downEndCount(e, err);
  }

  // This version is called for normal sync blocks.
  pragma "task join impl fn"
  pragma "unchecked throws"
  proc chpl_waitDynamicEndCount(param countRunningTasks=true) throws {
    var e = __primitive("get dynamic end count");
    _waitEndCount(e, countRunningTasks);

    // Throw any error raised by a task this sync statement is waiting for
    if ! e.errors.empty() then
      throw new owned TaskErrors(e.errors);
  }

  // Routines for re-interpreting reals as uints and vice-versa, at
  // the bit level
  //
  @unstable("This routine may change names / signatures")
  proc param (real(64)).transmute(type t) param : t {
    if t != uint {
      compilerError("Cannot (currently) transmute from " + this.type:string +
                    " to " + t:string);
    } else {
      param ui: uint(64) = __primitive("real64 as uint64", this);
      return ui;
    }
  }

  @unstable("This routine may change names / signatures")
  proc param (real(32)).transmute(type t) param : t {
    if t != uint(32) {
      compilerError("Cannot (currently) transmute from " + this.type:string +
                    " to " + t:string);
    } else {
      param ui: uint(32) = __primitive("real32 as uint32", this);
      return ui;
    }
  }

  @unstable("This routine may change names / signatures")
  inline proc (real(?w)).transmute(type t): t {
    use CTypes;
    import OS.POSIX.memcpy;

    if t != uint(w) {
      compilerError("Cannot (currently) transmute from " + this.type:string +
                    " to " + t:string);
    } else {
      var src = this,
          dst: uint(w);
      memcpy(c_ptrTo(dst), c_ptrTo(src), numBytes(t).safeCast(c_size_t));
      return dst;
    }
  }

  @unstable("This routine may change names / signatures")
  proc param (uint(64)).transmute(type t) param : t {
    if t != real(64) {
      compilerError("Cannot (currently) transmute from " + this.type:string +
                    " to " + t:string);
    } else {
      param r: real(64) = __primitive("uint64 as real64", this);
      return r;
    }
  }

  @unstable("This routine may change names / signatures")
  inline proc param (uint(32)).transmute(type t) param : t {
    if t != real(32) {
      compilerError("Cannot (currently) transmute from " + this.type:string +
                    " to " + t:string);
    } else {
      param r: real(32) = __primitive("uint32 as real32", this);
      return r;
    }
  }

  @unstable("This routine may change names / signatures")
  inline proc (uint(?w)).transmute(type t): t {
    use CTypes;
    import OS.POSIX.memcpy;

    if t != real(w) {
      compilerError("Cannot (currently) transmute from " + this.type:string +
                    " to " + t:string);
    } else {
      var src = this,
          dst: real(w);
      memcpy(c_ptrTo(dst), c_ptrTo(src), numBytes(t).safeCast(c_size_t));
      return dst;
    }
  }


  //
  // Similar to isPrimitiveType, but excludes imaginaries because they
  // are handled within the Chapel code directly (using overloads further
  // down in the file) to save complexity in the compiler.
  //
  inline proc chpl_typeSupportsPrimitiveCast(type t) param do
    return isBoolType(t) ||
           isIntegralType(t) ||
           isRealType(t);

  inline operator :(x:bool, type t:integral) do
    return __primitive("cast", t, x);
  inline operator :(x:bool, type t:chpl_anyreal) do
    return __primitive("cast", t, x);

  inline operator :(x:integral, type t:bool) do
    return __primitive("cast", t, x);
  inline operator :(x:integral, type t:integral) do
    return __primitive("cast", t, x);
  inline operator :(x:integral, type t:chpl_anyreal) do
    return __primitive("cast", t, x);

  inline operator :(x:chpl_anyreal, type t:bool) do
    return __primitive("cast", t, x);
  inline operator :(x:chpl_anyreal, type t:integral) do
    return __primitive("cast", t, x);
  inline operator :(x:chpl_anyreal, type t:chpl_anyreal) do
    return __primitive("cast", t, x);

  proc chpl_checkCastAbstractEnumError(type enumType, type dstType) param do
    if isAbstractEnumType(enumType) then
      compilerError("cannot cast abstract enum type '" +
                    enumType:string +
                    "' to '" +
                    dstType:string +
                    "'");

  @unstable("enum-to-bool casts are likely to be deprecated in the future")
  inline operator :(x: enum, type t:bool) throws {
    chpl_checkCastAbstractEnumError(x.type, t);
    return x: int: bool;
  }
  // operator :(x: enum, type t:integral)
  // is generated for each enum in buildDefaultFunctions
  inline operator :(x: enum, type t:enum) where x.type == t do
    return x;

  @unstable("enum-to-float casts are likely to be deprecated in the future")
  inline operator :(x: enum, type t:chpl_anyreal) throws {
    chpl_checkCastAbstractEnumError(x.type, t);
    return x: int: real;
  }

  inline operator :(x:_nilType, type t:unmanaged class)
  {
      compilerError("cannot cast nil to " + t:string);
  }
  inline operator :(x:_nilType, type t:borrowed class)
  {
    compilerError("cannot cast nil to " + t:string);
  }

  proc chpl_castUnmanagedError(param typeStr: string) param do
    compilerError("cannot cast to a '" + typeStr +
                  "' with an implicit borrow; try adding an explicit '.borrow()'");


  pragma "last resort"
  operator :(x:owned class, type t:unmanaged class)
    do chpl_castUnmanagedError(t:string);
  pragma "last resort"
  operator :(x:owned class?, type t:unmanaged class)
    do chpl_castUnmanagedError(t:string);
  pragma "last resort"
  operator :(x:owned class, type t:unmanaged class?)
    do chpl_castUnmanagedError(t:string);
  pragma "last resort"
  operator :(x:owned class?, type t:unmanaged class?)
    do chpl_castUnmanagedError(t:string);

  pragma "last resort"
  operator :(x:shared class, type t:unmanaged class)
    do chpl_castUnmanagedError(t:string);
  pragma "last resort"
  operator :(x:shared class?, type t:unmanaged class)
    do chpl_castUnmanagedError(t:string);
  pragma "last resort"
  operator :(x:shared class, type t:unmanaged class?)
    do chpl_castUnmanagedError(t:string);
  pragma "last resort"
  operator :(x:shared class?, type t:unmanaged class?)
    do chpl_castUnmanagedError(t:string);

  // casting to unmanaged?, no class downcast
  pragma "last resort"
  inline operator :(xn:borrowed class?, type tn:unmanaged class?)
    where isNilableClass(xn) && isNilableClass(tn) && isSubtype(_to_unmanaged(xn.type),tn)
  {
    return __primitive("to nilable class",
        __primitive("to unmanaged class", xn));
  }

  // casting to unmanaged, no class downcast
  pragma "last resort"
  inline operator :(xn:borrowed class?, type t:unmanaged class) throws
    where isNilableClass(xn) && isNonNilableClass(t) && isSubtype(_to_nonnil(_to_unmanaged(xn.type)),t)
  {
    if xn == nil {
      throw new owned NilClassError();
    }
    return __primitive("to non nilable class",
        __primitive("to unmanaged class", xn));
  }

  // casting to unmanaged, no class downcast
  pragma "last resort"
  inline operator :(x:borrowed class, type t:unmanaged class)
    where isNonNilableClass(x) && isNonNilableClass(t) && isSubtype(_to_unmanaged(x.type),t)
  {
    return __primitive("to non nilable class",
        __primitive("to unmanaged class", x));
  }

  // casting to unmanaged, no class downcast
  pragma "last resort"
  inline operator :(x:borrowed class, type tn:unmanaged class?)
    where isNonNilableClass(x) && isNilableClass(tn) && isSubtype(_to_nilable(_to_unmanaged(x.type)),tn)
  {
    return __primitive("to nilable class",
        __primitive("to unmanaged class", x));
  }

  // casting away nilability, no class downcast
  inline operator :(x:unmanaged class?, type t:borrowed class) throws
    where isSubtype(_to_nonnil(x.type),t)
  {
    if x == nil {
      throw new owned NilClassError();
    }
    return __primitive("cast", t, x);
  }


  // casting away nilability, no class downcast
  inline operator :(x:borrowed class?, type t:borrowed class)  throws
    where isSubtype(_to_nonnil(x.type),t)
  {
    if x == nil {
      throw new owned NilClassError();
    }
    return __primitive("cast", t, x);
  }

  // this version handles downcast to non-nil borrowed
  inline operator :(x:borrowed class?, type t:borrowed class)  throws
    where isProperSubtype(t,_to_nonnil(x.type))
  {
    if x == nil {
      throw new owned NilClassError();
    }
    var tmp = __primitive("dynamic_cast", t, x);
    if tmp == nil {
      throw new owned ClassCastError();
    }

    return _to_nonnil(_to_borrowed(tmp));
  }

  // this version handles downcast to nilable borrowed
  inline operator :(x:borrowed class?, type t:borrowed class?)
    where isProperSubtype(t,x.type)
  {
    if x == nil {
      return nil;
    }
    var tmp = __primitive("dynamic_cast", t, x);
    return _to_nilable(_to_borrowed(tmp));
  }


  // this version handles downcast to non-nil unmanaged
  pragma "last resort"
  inline operator :(x:borrowed class?, type t:unmanaged class) throws
    where isProperSubtype(t,_to_nonnil(_to_unmanaged(x.type)))
  {
    if x == nil {
      throw new owned NilClassError();
    }
    var tmp = __primitive("dynamic_cast", t, x);
    if tmp == nil {
      throw new owned ClassCastError();
    }

    return _to_nonnil(_to_unmanaged(tmp));
  }

  // this version handles downcast to nilable unmanaged
  pragma "last resort"
  inline operator :(x:borrowed class?, type t:unmanaged class?)
    where isProperSubtype(t,_to_unmanaged(x.type))
  {
    if x == nil {
      return nil;
    }
    var tmp = __primitive("dynamic_cast", t, x);
    return _to_nilable(_to_unmanaged(tmp));
  }

  // this version handles downcast to nilable unmanaged
  pragma "last resort"
  inline operator :(x:borrowed class, type t:unmanaged class?)
    where isProperSubtype(_to_nonnil(_to_borrowed(t)),x.type)
  {
    if x == nil {
      return nil;
    }
    var tmp = __primitive("dynamic_cast", t, x);
    return _to_nilable(_to_unmanaged(tmp));
  }



  //
  // casts to complex
  //
  inline operator :(x: bool, type t:chpl_anycomplex) do
    return (x, 0):t;

  inline operator :(x: integral, type t:chpl_anycomplex) do
    return (x, 0):t;

  inline operator :(x: chpl_anyreal, type t:chpl_anycomplex) do
    return (x, 0):t;

  inline operator :(x: chpl_anyimag, type t:chpl_anycomplex) do
    return (0, _i2r(x)):t;

  inline operator :(x: chpl_anycomplex, type t:chpl_anycomplex) do
    return (x.re, x.im):t;

  @unstable("enum-to-float casts are likely to be deprecated in the future")
  inline operator :(x: enum, type t:chpl_anycomplex) throws {
    chpl_checkCastAbstractEnumError(x.type, t);
    return (x:real, 0):t;
  }

  //
  // casts to imag
  //
  inline operator :(x: bool, type t:chpl_anyimag) do
    return if x then 1i:t else 0i:t;

  inline operator :(x: integral, type t:chpl_anyimag) do
    return __primitive("cast", t, x);

  inline operator :(x: chpl_anyreal, type t:chpl_anyimag) do
    return __primitive("cast", t, x);

  inline operator :(x: chpl_anyimag, type t:chpl_anyimag) do
    return __primitive("cast", t, x);

  inline operator :(x: chpl_anycomplex, type t:chpl_anyimag) do
    return __primitive("cast", t, x.im);

  @unstable("enum-to-float casts are likely to be deprecated in the future")
  inline operator :(x: enum, type t:chpl_anyimag)  throws {
    chpl_checkCastAbstractEnumError(x.type, t);
    return x:real:imag;
  }

  //
  // casts from complex
  //
  inline operator :(x: chpl_anycomplex, type t:chpl_anyreal)  {
    var y: t;
    y = x.re:t;
    return y;
  }
  inline operator :(x: chpl_anycomplex, type t:integral)  {
    var y: t;
    y = x.re:t;
    return y;
  }

  //
  // casts from imag
  //
  inline operator :(x: chpl_anyimag, type t:chpl_anyreal) do
    return __primitive("cast", t, x);
  inline operator :(x: chpl_anyimag, type t:integral) do
    return __primitive("cast", t, x);

  inline operator :(x: chpl_anyimag, type t:bool) do
    return if x != 0i then true else false;

  pragma "init copy fn"
  inline proc chpl__initCopy(type t, definedConst: bool)  type {
    compilerError("illegal assignment of type to value");
    return t;
  }

  pragma "compiler generated"
  pragma "last resort"
  pragma "init copy fn"
  inline proc chpl__initCopy(x: _tuple, definedConst: bool) {
    // body inserted during generic instantiation
  }

  // Catch-all initCopy implementation:
  pragma "compiler generated"
  pragma "last resort"
  pragma "init copy fn"
  pragma "suppress lvalue error"
  inline proc chpl__initCopy(const x, definedConst: bool) {
    // body adjusted during generic instantiation
    return x;
  }

  pragma "compiler generated"
  pragma "last resort"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x: _tuple, definedConst: bool) {
    // body inserted during generic instantiation
  }

  pragma "compiler generated"
  pragma "last resort"
  pragma "unref fn"
  inline proc chpl__unref(x: _tuple) {
    // body inserted during generic instantiation
  }


  pragma "compiler generated"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(ir: _iteratorRecord, definedConst: bool) {
    // body modified during call destructors pass
    return ir;
  }

  pragma "compiler generated"
  pragma "last resort"
  pragma "auto copy fn"
  pragma "suppress lvalue error"
  inline proc chpl__autoCopy(const x, definedConst: bool) {
    return chpl__initCopy(x, definedConst);
  }

  pragma "compiler generated"
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(x: borrowed RootClass) { }

  pragma "compiler generated"
  pragma "last resort"
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(type t)  { }

  pragma "compiler generated"
  pragma "last resort"
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(x) {
    __primitive("call destructor", x);
  }
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(ir: _iteratorRecord) {
    // body inserted during call destructors pass
  }

  // These might seem the same as the generic version
  // but they currently necessary to prevent resolution from
  // using promotion (for example with an array of sync variables)
  pragma "dont disable remote value forwarding"
  pragma "removable auto destroy"
  pragma "auto destroy fn"
  proc chpl__autoDestroy(x: _distribution) {
    __primitive("call destructor", x);
  }
  pragma "dont disable remote value forwarding"
  pragma "removable auto destroy"
  pragma "auto destroy fn"
  proc chpl__autoDestroy(x: domain) {
    __primitive("call destructor", x);
  }
  pragma "dont disable remote value forwarding"
  pragma "removable auto destroy"
  pragma "auto destroy fn"
  proc chpl__autoDestroy(x: []) {
    __primitive("call destructor", x);
  }

  /*
  inline proc chpl__tounmanaged(ref arg:Owned) {
    return owned.release(arg);
  }
  inline proc chpl__tounmanaged(arg) where arg:RootClass {
    return arg;
  }*/


  // implements 'delete' statement
  pragma "no borrow convert"
  proc chpl__delete(const arg) {

    if chpl_isDdata(arg.type) then
      compilerError("cannot delete data class");
    if arg.type == _nilType then
      compilerError("should not delete 'nil'");
    if isSubtype(arg.type, _owned) then
      compilerError("'delete' is not allowed on an owned class type");
    if isSubtype(arg.type, _shared) then
      compilerError("'delete' is not allowed on a shared class type");
    if isRecord(arg) then
      // special case for records as a more likely occurrence
      compilerError("'delete' is not allowed on records");
    if !isCoercible(arg.type, borrowed class?) then
      compilerError("'delete' is not allowed on non-class type ",
                    arg.type:string);
    if !isCoercible(arg.type, unmanaged class?) then
      compilerError("'delete' can only be applied to unmanaged classes");

    if (arg != nil) {
      arg!.deinit();

      on arg do
        chpl_here_free(__primitive("_wide_get_addr", arg));
    }
  }

  proc chpl__delete(const arr: []) {
    forall a in arr do
      chpl__delete(a);
  }

  // delete two or more things
  proc chpl__delete(arg, const args...) {
    chpl__delete(arg);
    for param i in 0..args.size-1 do
      chpl__delete(args(i));
  }

  // Type functions for representing function types

  @deprecated("The 'func' procedure type constructor is deprecated, please use 'proc' syntax instead")
  inline proc func() type { return __primitive("create fn type", void); }

  @deprecated("The 'func' procedure type constructor is deprecated, please use 'proc' syntax instead")
  inline proc func(type rettype) type { return __primitive("create fn type", rettype); }

  @deprecated("The 'func' procedure type constructor is deprecated, please use 'proc' syntax instead")
  inline proc func(type t...?n, type rettype) type { return __primitive("create fn type", (...t), rettype); }

  proc isIterator(ic: _iteratorClass) param do return true;
  proc isIterator(ir: _iteratorRecord) param do return true;
  proc isIterator(not_an_iterator) param do return false;


  /* op= operators
   */
  inline operator +=(ref lhs: int(8), rhs: int(8)) do __primitive("+=", lhs, rhs);
  inline operator +=(ref lhs: int(16), rhs: int(16)) do __primitive("+=", lhs, rhs);
  inline operator +=(ref lhs: int(32), rhs: int(32)) do __primitive("+=", lhs, rhs);
  inline operator +=(ref lhs: int(64), rhs: int(64)) do __primitive("+=", lhs, rhs);

  inline operator +=(ref lhs: uint(8), rhs: uint(8)) do __primitive("+=", lhs, rhs);
  inline operator +=(ref lhs: uint(16), rhs: uint(16)) do __primitive("+=", lhs, rhs);
  inline operator +=(ref lhs: uint(32), rhs: uint(32)) do __primitive("+=", lhs, rhs);
  inline operator +=(ref lhs: uint(64), rhs: uint(64)) do __primitive("+=", lhs, rhs);

  inline operator +=(ref lhs: real(32), rhs: real(32)) do __primitive("+=", lhs, rhs);
  inline operator +=(ref lhs: real(64), rhs: real(64)) do __primitive("+=", lhs, rhs);

  inline operator +=(ref lhs: imag(32), rhs: imag(32)) do __primitive("+=", lhs, rhs);
  inline operator +=(ref lhs: imag(64), rhs: imag(64)) do __primitive("+=", lhs, rhs);

  inline operator +=(ref lhs: complex(64), rhs: complex(64)) do lhs = (lhs+rhs);
  inline operator +=(ref lhs: complex(128), rhs: complex(128)) do lhs = (lhs+rhs);
  // This function shouldn't be 'last resort'
  // because if it is, that would interfere with things like
  //  A += A or A += [i in A.domain] A[i]
  // due to the existing array += eltType overloads & those being promoted.
  // So, use a where clause so that the overloads above are used if
  // they are available.
  inline operator +=(ref lhs, rhs)
  where !(isNumericType(lhs.type) && isNumericType(rhs.type)) {
    lhs = lhs + rhs;
  }

  inline operator -=(ref lhs: int(8), rhs: int(8)) do __primitive("-=", lhs, rhs);
  inline operator -=(ref lhs: int(16), rhs: int(16)) do __primitive("-=", lhs, rhs);
  inline operator -=(ref lhs: int(32), rhs: int(32)) do __primitive("-=", lhs, rhs);
  inline operator -=(ref lhs: int(64), rhs: int(64)) do __primitive("-=", lhs, rhs);

  inline operator -=(ref lhs: uint(8), rhs: uint(8)) do __primitive("-=", lhs, rhs);
  inline operator -=(ref lhs: uint(16), rhs: uint(16)) do __primitive("-=", lhs, rhs);
  inline operator -=(ref lhs: uint(32), rhs: uint(32)) do __primitive("-=", lhs, rhs);
  inline operator -=(ref lhs: uint(64), rhs: uint(64)) do __primitive("-=", lhs, rhs);

  inline operator -=(ref lhs: real(32), rhs: real(32)) do __primitive("-=", lhs, rhs);
  inline operator -=(ref lhs: real(64), rhs: real(64)) do __primitive("-=", lhs, rhs);

  inline operator -=(ref lhs: imag(32), rhs: imag(32)) do __primitive("-=", lhs, rhs);
  inline operator -=(ref lhs: imag(64), rhs: imag(64)) do __primitive("-=", lhs, rhs);
  // this one is just here so we can use !isNumericType(t) below
  inline operator -=(ref lhs: complex(64), rhs: complex(64)) do lhs = (lhs-rhs);
  inline operator -=(ref lhs: complex(128), rhs: complex(128)) do lhs = (lhs-rhs);
  inline operator -=(ref lhs, rhs)
  where !(isNumericType(lhs.type) && isNumericType(rhs.type)) {
    lhs = lhs - rhs;
  }

  inline operator *=(ref lhs: int(8), rhs: int(8)) do __primitive("*=", lhs, rhs);
  inline operator *=(ref lhs: int(16), rhs: int(16)) do __primitive("*=", lhs, rhs);
  inline operator *=(ref lhs: int(32), rhs: int(32)) do __primitive("*=", lhs, rhs);
  inline operator *=(ref lhs: int(64), rhs: int(64)) do __primitive("*=", lhs, rhs);

  inline operator *=(ref lhs: uint(8), rhs: uint(8)) do __primitive("*=", lhs, rhs);
  inline operator *=(ref lhs: uint(16), rhs: uint(16)) do __primitive("*=", lhs, rhs);
  inline operator *=(ref lhs: uint(32), rhs: uint(32)) do __primitive("*=", lhs, rhs);
  inline operator *=(ref lhs: uint(64), rhs: uint(64)) do __primitive("*=", lhs, rhs);

  inline operator *=(ref lhs: real(32), rhs: real(32)) do __primitive("*=", lhs, rhs);
  inline operator *=(ref lhs: real(64), rhs: real(64)) do __primitive("*=", lhs, rhs);
  private proc isIntegralOrRealType(type t) param {
    return isIntegralType(t) || isRealType(t);
  }
  inline operator *=(ref lhs, rhs)
  where !(isIntegralOrRealType(lhs.type) && isIntegralOrRealType(rhs.type)) {
    lhs = lhs * rhs;
  }

  inline operator /=(ref lhs:int(8), rhs:int(8)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to divide by zero");
    __primitive("/=", lhs, rhs);
  }
  inline operator /=(ref lhs:int(16), rhs:int(16)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to divide by zero");
    __primitive("/=", lhs, rhs);
  }
  inline operator /=(ref lhs:int(32), rhs:int(32)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to divide by zero");
    __primitive("/=", lhs, rhs);
  }
  inline operator /=(ref lhs:int(64), rhs:int(64)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to divide by zero");
    __primitive("/=", lhs, rhs);
  }

  inline operator /=(ref lhs:uint(8), rhs:uint(8)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to divide by zero");
    __primitive("/=", lhs, rhs);
  }
  inline operator /=(ref lhs:uint(16), rhs:uint(16)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to divide by zero");
    __primitive("/=", lhs, rhs);
  }
  inline operator /=(ref lhs:uint(32), rhs:uint(32)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to divide by zero");
    __primitive("/=", lhs, rhs);
  }
  inline operator /=(ref lhs:uint(64), rhs:uint(64)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to divide by zero");
    __primitive("/=", lhs, rhs);
  }

  inline operator /=(ref lhs: real(32), rhs: real(32)) do __primitive("/=", lhs, rhs);
  inline operator /=(ref lhs: real(64), rhs: real(64)) do __primitive("/=", lhs, rhs);
  inline operator /=(ref lhs, rhs)
  where !(isIntegralOrRealType(lhs.type) && isIntegralOrRealType(rhs.type)) {
    lhs = lhs / rhs;
  }

  inline operator %=(ref lhs:int(8), rhs:int(8)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to compute a modulus by zero");
    __primitive("%=", lhs, rhs);
  }
  inline operator %=(ref lhs:int(16), rhs:int(16)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to compute a modulus by zero");
    __primitive("%=", lhs, rhs);
  }
  inline operator %=(ref lhs:int(32), rhs:int(32)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to compute a modulus by zero");
    __primitive("%=", lhs, rhs);
  }
  inline operator %=(ref lhs:int(64), rhs:int(64)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to compute a modulus by zero");
    __primitive("%=", lhs, rhs);
  }

  inline operator %=(ref lhs:uint(8), rhs:uint(8)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to compute a modulus by zero");
    __primitive("%=", lhs, rhs);
  }
  inline operator %=(ref lhs:uint(16), rhs:uint(16)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to compute a modulus by zero");
    __primitive("%=", lhs, rhs);
  }
  inline operator %=(ref lhs:uint(32), rhs:uint(32)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to compute a modulus by zero");
    __primitive("%=", lhs, rhs);
  }
  inline operator %=(ref lhs:uint(64), rhs:uint(64)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to compute a modulus by zero");
    __primitive("%=", lhs, rhs);
  }

  inline operator %=(ref lhs, rhs)
  where !(isIntegralOrRealType(lhs.type) && isIntegralOrRealType(rhs.type)) {
    lhs = lhs % rhs;
  }

  //
  // This overload provides param coercion for cases like uint **= true;
  //
  inline operator **=(ref lhs, rhs) {
    lhs = lhs ** rhs;
  }

  inline operator &=(ref lhs: int(8), rhs: int(8)) do __primitive("&=", lhs, rhs);
  inline operator &=(ref lhs: int(16), rhs: int(16)) do __primitive("&=", lhs, rhs);
  inline operator &=(ref lhs: int(32), rhs: int(32)) do __primitive("&=", lhs, rhs);
  inline operator &=(ref lhs: int(64), rhs: int(64)) do __primitive("&=", lhs, rhs);

  inline operator &=(ref lhs: uint(8), rhs: uint(8)) do __primitive("&=", lhs, rhs);
  inline operator &=(ref lhs: uint(16), rhs: uint(16)) do __primitive("&=", lhs, rhs);
  inline operator &=(ref lhs: uint(32), rhs: uint(32)) do __primitive("&=", lhs, rhs);
  inline operator &=(ref lhs: uint(64), rhs: uint(64)) do __primitive("&=", lhs, rhs);
  inline operator &=(ref lhs, rhs)
  where !(isNumericType(lhs.type) && isNumericType(rhs.type)) {
    lhs = lhs & rhs;
  }


  inline operator |=(ref lhs: int(8), rhs: int(8)) do __primitive("|=", lhs, rhs);
  inline operator |=(ref lhs: int(16), rhs: int(16)) do __primitive("|=", lhs, rhs);
  inline operator |=(ref lhs: int(32), rhs: int(32)) do __primitive("|=", lhs, rhs);
  inline operator |=(ref lhs: int(64), rhs: int(64)) do __primitive("|=", lhs, rhs);

  inline operator |=(ref lhs: uint(8), rhs: uint(8)) do __primitive("|=", lhs, rhs);
  inline operator |=(ref lhs: uint(16), rhs: uint(16)) do __primitive("|=", lhs, rhs);
  inline operator |=(ref lhs: uint(32), rhs: uint(32)) do __primitive("|=", lhs, rhs);
  inline operator |=(ref lhs: uint(64), rhs: uint(64)) do __primitive("|=", lhs, rhs);
  inline operator |=(ref lhs, rhs)
  where !(isNumericType(lhs.type) && isNumericType(rhs.type)) {
    lhs = lhs | rhs;
  }

  inline operator ^=(ref lhs: int(8), rhs: int(8)) do __primitive("^=", lhs, rhs);
  inline operator ^=(ref lhs: int(16), rhs: int(16)) do __primitive("^=", lhs, rhs);
  inline operator ^=(ref lhs: int(32), rhs: int(32)) do __primitive("^=", lhs, rhs);
  inline operator ^=(ref lhs: int(64), rhs: int(64)) do __primitive("^=", lhs, rhs);

  inline operator ^=(ref lhs: uint(8), rhs: uint(8)) do __primitive("^=", lhs, rhs);
  inline operator ^=(ref lhs: uint(16), rhs: uint(16)) do __primitive("^=", lhs, rhs);
  inline operator ^=(ref lhs: uint(32), rhs: uint(32)) do __primitive("^=", lhs, rhs);
  inline operator ^=(ref lhs: uint(64), rhs: uint(64)) do __primitive("^=", lhs, rhs);
  inline operator ^=(ref lhs, rhs)
  where !(isNumericType(lhs.type) && isNumericType(rhs.type)) {
    lhs = lhs ^ rhs;
  }

  inline operator >>=(ref lhs:int(?w), rhs:integral) {
    __primitive(">>=", lhs, rhs);
  }
  inline operator >>=(ref lhs:uint(?w), rhs:integral) {
    __primitive(">>=", lhs, rhs);
  }
  inline operator >>=(ref lhs, rhs)
  where !(isNumericType(lhs.type) && isNumericType(rhs.type)) {
    lhs = lhs >> rhs;
  }

  inline operator <<=(ref lhs:int(?w), rhs:integral) {
    __primitive("<<=", lhs, rhs);
  }
  inline operator <<=(ref lhs:uint(?w), rhs:integral) {
    __primitive("<<=", lhs, rhs);
  }
  inline operator <<=(ref lhs, rhs)
  where !(isNumericType(lhs.type) && isNumericType(rhs.type)) {
    lhs = lhs << rhs;
  }

  // TODO: can we remove last resort on this?
  /* swap operator */
  pragma "last resort"
  pragma "ignore transfer errors"
  inline operator <=>(ref lhs, ref rhs) {
    // It's tempting to make `tmp` a `const`, but it causes problems
    // for types where the RHS of an assignment is modified, such as a
    // record with an `owned` class field.  It's a short-lived enough
    // variable that making it `var` doesn't seem likely to thwart
    // optimization opportunities.
    var tmp = lhs;
    lhs = rhs;
    rhs = tmp;
  }


  // Everything below this comment was originally generated by the
  // program:
  //
  //   $CHPL_HOME/util/devel/gen_int_uint64_operators.chpl.
  //
  // Since then, things have been manually edited/improved, for
  // better or worse (i.e., we could've or should've improved that
  // file and re-run it).

  //
  // non-param/non-param
  //

  //
  // non-param/param and param/non-param cases -- these cases
  // are provided to support operations on runtime uint and
  // param uint combinations.  These are expressed in terms of int/uint
  // functions with one param argument and one non-param argument.
  // Since function disambiguation prefers a 'param' argument over
  // a non-param one, if the 'int' version here is not provided,
  // anEnumVariable + 1 (say) will resolve to the uint + here
  // and that would give the wrong result type (uint rather than int).
  inline operator +(a: uint(64), param b: uint(64)) {
    return __primitive("+", a, b);
  }
  inline operator +(param a: uint(64), b: uint(64)) {
    return __primitive("+", a, b);
  }
  inline operator +(a: int(64), param b: int(64)) {
    return __primitive("+", a, b);
  }
  inline operator +(param a: int(64), b: int(64)) {
    return __primitive("+", a, b);
  }


  // non-param/non-param

  // non-param/param and param/non-param
  inline operator -(a: uint(64), param b: uint(64)) {
    return __primitive("-", a, b);
  }
  inline operator -(param a: uint(64), b: uint(64)) {
    return __primitive("-", a, b);
  }
  inline operator -(a: int(64), param b: int(64)) {
    return __primitive("-", a, b);
  }
  inline operator -(param a: int(64), b: int(64)) {
    return __primitive("-", a, b);
  }


  // non-param/non-param

  // non-param/param and param/non-param
  inline operator *(a: uint(64), param b: uint(64)) {
    return __primitive("*", a, b);
  }
  inline operator *(param a: uint(64), b: uint(64)) {
    return __primitive("*", a, b);
  }
  inline operator *(a: int(64), param b: int(64)) {
    return __primitive("*", a, b);
  }
  inline operator *(param a: int(64), b: int(64)) {
    return __primitive("*", a, b);
  }


  // non-param/non-param

  // non-param/param and param/non-param
  // The int version is only defined so we can catch the divide by zero error
  // at compile time
  inline operator /(a: int(64), param b: int(64)) {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(a: uint(64), param b: uint(64)) {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(param a: uint(64), b: uint(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline operator /(param a: int(64), b: int(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }


  // non-param/non-param

  // non-param/param and param/non-param
  inline operator **(a: uint(64), param b: uint(64)) {
    return __primitive("**", a, b);
  }
  inline operator **(param a: uint(64), b: uint(64)) {
    return __primitive("**", a, b);
  }
  inline operator **(a: int(64), param b: int(64)) {
    return __primitive("**", a, b);
  }
  inline operator **(param a: int(64), b: int(64)) {
    return __primitive("**", a, b);
  }


  // non-param/non-param

  // non-param/param and param/non-param
  inline operator %(a: uint(64), param b: uint(64)) {
    if b == 0 then compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  // necessary to support e.g. 10 % myuint
  inline operator %(param a: uint(64), b: uint(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(a: int(64), param b: int(64)) {
    if b == 0 then compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline operator %(param a: int(64), b: int(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }


  // non-param/non-param

  //
  // If b's negative, these obviously aren't equal; if it's not
  // negative, it can be cast to a uint
  //
  inline operator ==(a: uint(8), b: int(8)) do return !(b < 0) && (a == b:uint(8));
  inline operator ==(a: uint(16), b: int(16)) do return !(b < 0) && (a == b:uint(16));
  inline operator ==(a: uint(32), b: int(32)) do return !(b < 0) && (a == b:uint(32));
  inline operator ==(a: uint(64), b: int(64)) do return !(b < 0) && (a == b:uint(64));

  //
  // the dual of the above
  //
  inline operator ==(a: int(8), b: uint(8)) do return !(a < 0) && a : uint(8) == b;
  inline operator ==(a: int(16), b: uint(16)) do return !(a < 0) && a : uint(16) == b;
  inline operator ==(a: int(32), b: uint(32)) do return !(a < 0) && a : uint(32) == b;
  inline operator ==(a: int(64), b: uint(64)) do return !(a < 0) && a : uint(64) == b;

  // non-param/param and param/non-param
  // not necessary since the == versions above
  // work there (and aren't an error)



  // non-param/non-param
  inline operator !=(a: uint(8), b: int(8)) do return b < 0 || a != b : uint(8);
  inline operator !=(a: uint(16), b: int(16)) do return b < 0 || a != b : uint(16);
  inline operator !=(a: uint(32), b: int(32)) do return b < 0 || a != b : uint(32);
  inline operator !=(a: uint(64), b: int(64)) do return b < 0 || a != b : uint(64);

  inline operator !=(a: int(8), b: uint(8)) do return a < 0 || a : uint(8) != b;
  inline operator !=(a: int(16), b: uint(16)) do return a < 0 || a : uint(16) != b;
  inline operator !=(a: int(32), b: uint(32)) do return a < 0 || a : uint(32) != b;
  inline operator !=(a: int(64), b: uint(64)) do return a < 0 || a : uint(64) != b;

  // non-param/param and param/non-param
  // not necessary since the == versions above
  // work there (and aren't an error)


  // non-param/non-param
  inline operator >(a: uint(8), b: int(8)) do return b < 0 || a > b : uint(8);
  inline operator >(a: uint(16), b: int(16)) do return b < 0 || a > b : uint(16);
  inline operator >(a: uint(32), b: int(32)) do return b < 0 || a > b : uint(32);
  inline operator >(a: uint(64), b: int(64)) do return b < 0 || a > b : uint(64);

  inline operator >(a: int(8), b: uint(8)) do return !(a < 0) && a : uint(8) > b;
  inline operator >(a: int(16), b: uint(16)) do return !(a < 0) && a : uint(16) > b;
  inline operator >(a: int(32), b: uint(32)) do return !(a < 0) && a : uint(32) > b;
  inline operator >(a: int(64), b: uint(64)) do return !(a < 0) && a : uint(64) > b;

  // non-param/param and param/non-param
  // non-param/param version not necessary since > above works fine for that
  inline operator >(param a: uint(8), b: uint(8)) {
    if __primitive("==", a, 0) {
      return false;
    } else {
      return __primitive(">", a, b);
    }
  }
  inline operator >(param a: uint(16), b: uint(16)) {
    if __primitive("==", a, 0) {
      return false;
    } else {
      return __primitive(">", a, b);
    }
  }
  inline operator >(param a: uint(32), b: uint(32)) {
    if __primitive("==", a, 0) {
      return false;
    } else {
      return __primitive(">", a, b);

    }
  }
  inline operator >(param a: uint(64), b: uint(64)) {
    if __primitive("==", a, 0) {
      return false;
    } else {
      return __primitive(">", a, b);
    }
  }

  inline operator >(param a: int(8), b: int(8)) do return __primitive(">", a, b);
  inline operator >(param a: int(16), b: int(16)) do return __primitive(">", a, b);
  inline operator >(param a: int(32), b: int(32)) do return __primitive(">", a, b);
  inline operator >(param a: int(64), b: int(64)) do return __primitive(">", a, b);

  // non-param/non-param
  inline operator >=(a: uint(8), b: int(8)) do return b < 0 || a >= b : uint(8);
  inline operator >=(a: uint(16), b: int(16)) do return b < 0 || a >= b : uint(16);
  inline operator >=(a: uint(32), b: int(32)) do return b < 0 || a >= b : uint(32);
  inline operator >=(a: uint(64), b: int(64)) do return b < 0 || a >= b : uint(64);

  inline operator >=(a: int(8), b: uint(8)) do return !(a < 0) && a : uint(8) >= b;
  inline operator >=(a: int(16), b: uint(16)) do return !(a < 0) && a : uint(16) >= b;
  inline operator >=(a: int(32), b: uint(32)) do return !(a < 0) && a : uint(32) >= b;
  inline operator >=(a: int(64), b: uint(64)) do return !(a < 0) && a : uint(64) >= b;

  // non-param/param and param/non-param
  inline operator >=(a: uint(8), param b: uint(8)) {
    if __primitive("==", b, 0) {
      return true;
    } else {
      return __primitive(">=", a, b);
    }
  }
  inline operator >=(a: uint(16), param b: uint(16)) {
    if __primitive("==", b, 0) {
      return true;
    } else {
      return __primitive(">=", a, b);
    }
  }
  inline operator >=(a: uint(32), param b: uint(32)) {
    if __primitive("==", b, 0) {
      return true;
    } else {
      return __primitive(">=", a, b);
    }
  }
  inline operator >=(a: uint(64), param b: uint(64)) {
    if __primitive("==", b, 0) {
      return true;

    } else {
      return __primitive(">=", a, b);
    }
  }

  inline operator >=(a: int(8), param b: int(8)) do return __primitive(">=", a, b);
  inline operator >=(a: int(16), param b: int(16)) do return __primitive(">=", a, b);
  inline operator >=(a: int(32), param b: int(32)) do return __primitive(">=", a, b);
  inline operator >=(a: int(64), param b: int(64)) do return __primitive(">=", a, b);


  // non-param/non-param
  inline operator <=(a: uint(8), b: int(8)) do return !(b < 0) && a <= b : uint(8);
  inline operator <=(a: uint(16), b: int(16)) do return !(b < 0) && a <= b : uint(16);
  inline operator <=(a: uint(32), b: int(32)) do return !(b < 0) && a <= b : uint(32);
  inline operator <=(a: uint(64), b: int(64)) do return !(b < 0) && a <= b : uint(64);

  inline operator <=(a: int(8), b: uint(8)) do return a < 0 || a : uint(8) <= b;
  inline operator <=(a: int(16), b: uint(16)) do return a < 0 || a : uint(16) <= b;
  inline operator <=(a: int(32), b: uint(32)) do return a < 0 || a : uint(32) <= b;
  inline operator <=(a: int(64), b: uint(64)) do return a < 0 || a : uint(64) <= b;

  // non-param/param and param/non-param
  inline operator <=(param a: uint(8), b: uint(8)) {
    if __primitive("==", a, 0) {
      return true;
    } else {
      return __primitive("<=", a, b);
    }
  }
  inline operator <=(param a: uint(16), b: uint(16)) {
    if __primitive("==", a, 0) {
      return true;
    } else {
      return __primitive("<=", a, b);
    }
  }
  inline operator <=(param a: uint(32), b: uint(32)) {
    if __primitive("==", a, 0) {
      return true;
    } else {
      return __primitive("<=", a, b);
    }
  }
  inline operator <=(param a: uint(64), b: uint(64)) {
    if __primitive("==", a, 0) {
      return true;
    } else {
      return __primitive("<=", a, b);
    }
  }

  inline operator <=(param a: int(8), b: int(8)) do return __primitive("<=", a, b);
  inline operator <=(param a: int(16), b: int(16)) do return __primitive("<=", a, b);
  inline operator <=(param a: int(32), b: int(32)) do return __primitive("<=", a, b);
  inline operator <=(param a: int(64), b: int(64)) do return __primitive("<=", a, b);


  pragma "suppress generic actual warning"
  proc isGenericType(type t) param do return __primitive("is generic type", t);
  pragma "suppress generic actual warning"
  proc isNilableClassType(type t) param do return __primitive("is nilable class type", t);
  pragma "suppress generic actual warning"
  proc isNonNilableClassType(type t) param do return __primitive("is non nilable class type", t);

  pragma "suppress generic actual warning"
  proc isBorrowedOrUnmanagedClassType(type t:unmanaged) param do return true;
  pragma "suppress generic actual warning"
  proc isBorrowedOrUnmanagedClassType(type t:borrowed) param do return true;
  pragma "suppress generic actual warning"
  proc isBorrowedOrUnmanagedClassType(type t) param do return false;

  // These style element #s are used in the default Writer and Reader.
  // and in e.g. implementations of those in Tuple.
  extern const QIO_STYLE_ELEMENT_STRING:int;
  extern const QIO_STYLE_ELEMENT_COMPLEX:int;
  extern const QIO_STYLE_ELEMENT_ARRAY:int;
  extern const QIO_STYLE_ELEMENT_AGGREGATE:int;
  extern const QIO_STYLE_ELEMENT_TUPLE:int;
  extern const QIO_STYLE_ELEMENT_BYTE_ORDER:int;
  extern const QIO_STYLE_ELEMENT_IS_NATIVE_BYTE_ORDER:int;
  extern const QIO_STYLE_ELEMENT_SKIP_UNKNOWN_FIELDS:int;

  extern const QIO_ARRAY_FORMAT_SPACE:int;
  extern const QIO_ARRAY_FORMAT_CHPL:int;
  extern const QIO_ARRAY_FORMAT_JSON:int;

  extern const QIO_AGGREGATE_FORMAT_BRACES:int;
  extern const QIO_AGGREGATE_FORMAT_CHPL:int;
  extern const QIO_AGGREGATE_FORMAT_JSON:int;

  extern const QIO_TUPLE_FORMAT_CHPL:int;
  extern const QIO_TUPLE_FORMAT_SPACE:int;
  extern const QIO_TUPLE_FORMAT_JSON:int;

  // Support for module deinit functions.
  class chpl_ModuleDeinit : writeSerializable {
    const moduleName: c_ptrConst(c_char); // for debugging; non-null, not owned
    const deinitFun:  chpl_c_fn_ptr;          // module deinit function
    const prevModule: unmanaged chpl_ModuleDeinit?; // singly-linked list / LIFO queue
    override proc serialize(writer, ref serializer) throws {
      try {
        writer.writef("chpl_ModuleDeinit(%s)",string.createCopyingBuffer(moduleName));
      }
      catch e: DecodeError { // let IoError propagate
        halt("Module name is not valid string!");
      }
    }
  }
  var chpl_moduleDeinitFuns = nil: unmanaged chpl_ModuleDeinit?;

  // Supports type field accessors on nilable classes - on an instance...
  inline proc chpl_checkLegalTypeFieldAccessor(thisArg, type fieldType,
                                              param fieldName) type {
    if isNilableClassType(thisArg.type) &&
       // it is a runtime type
       (isDomainType(fieldType) || isArrayType(fieldType))
    then
       compilerError("accessing the runtime-type field ", fieldName,
         " of a nilable class. Consider applying postfix-! operator",
         " to the class before accessing this field.");

    return fieldType;
  }

  // ... and on a type.
  inline proc chpl_checkLegalTypeFieldAccessor(type thisArg, type fieldType,
                                              param fieldName) type {
    if // it is a runtime type
      (isDomainType(fieldType) || isArrayType(fieldType))
    then
       compilerError("accessing the runtime-type field ", fieldName,
         " of a class type is currently unsupported"); // see #11549

    return fieldType;
  }

  // The compiler does not emit _defaultOf for numeric and class types
  // directly. If _defaultOf is required, use variable initialization
  // to access it
  //    var x: T;

  // type constructor for unmanaged pointers
  // this could in principle be just _unmanaged (similar to type
  // constructor for a record) but that is more challenging because
  // _unmanaged is a built-in non-record type.
  pragma "suppress generic actual warning"
  proc _to_unmanaged(type t) type {
    type rt = __primitive("to unmanaged class", t);
    return rt;
  }
  inline proc _to_unmanaged(arg) {
    var ret = __primitive("to unmanaged class", arg);
    return ret;
  }
  // type constructor for converting to a borrow
  pragma "suppress generic actual warning"
  proc _to_borrowed(type t) type {
    type rt = __primitive("to borrowed class", t);
    return rt;
  }
  inline proc _to_borrowed(arg) {
    var ret = __primitive("to borrowed class", arg);
    return ret;
  }
  // changing nilability
  pragma "suppress generic actual warning"
  proc _to_nonnil(type t) type {
    type rt = __primitive("to non nilable class", t);
    return rt;
  }
  inline proc _to_nonnil(arg) {
    var ret = __primitive("to non nilable class", arg);
    return ret;
  }
  pragma "suppress generic actual warning"
  proc _to_nilable(type t) type {
    type rt = __primitive("to nilable class", t);
    return rt;
  }
  inline proc _to_nilable(arg) {
    var ret = __primitive("to nilable class", arg);
    return ret;
  }

  inline proc chpl_checkBorrowIfVar(arg, param isWhile) {
    if isUnmanagedClass(arg) then
      return arg;  // preserve unmanage-ness
    else if isClass(arg) then
      return arg.borrow();
    else
      compilerError(if isWhile then '"while var/const"' else '"if var/const"',
                    " construct is available only on classes,",
                    " here it is invoked on ", arg.type:string);
  }
  proc chpl_checkBorrowIfVar(type arg, param isWhile) {
    compilerError(if isWhile then '"while var/const"' else '"if var/const"',
                  " construct cannot be invoked on a type");
  }

  pragma "no borrow convert"
  inline proc _removed_cast(in x) {
    return x;
  }

  //
  // Support for bounded coforall task counting optimizations
  //

  proc chpl_supportsBoundedCoforall(iterable, param zippered) param {
    if zippered && isTuple(iterable) then
      return chpl_supportsBoundedCoforall(iterable[0], zippered=false);
    else if isRange(iterable) || isDomain(iterable) || isArray(iterable) then
      return true;
    else
      return false;
  }

  proc chpl_boundedCoforallSize(iterable, param zippered) {
    if zippered && isTuple(iterable) then
      return chpl_boundedCoforallSize(iterable[0], zippered=false);
    else if isRange(iterable) || isDomain(iterable) || isArray(iterable) then
      return iterable.size;
    else
      compilerError("Called chpl_boundedCoforallSize on an unsupported type");
  }

  /* The following chpl_field_*() overloads support compiler-generated
     comparison operators for records with array fields */

  proc chpl_field_neq(a: [] ?t, b: [] t) {
    return || reduce (a != b);
  }

  inline proc chpl_field_neq(a, b) where !isArrayType(a.type) {
    return a != b;
  }

  proc chpl_field_lt(a: [] ?t, b: [] t) {
    compilerError("ordered comparisons not supported by default on records with array fields");
  }

  inline proc chpl_field_lt(a, b) where !isArrayType(a.type) {
    return a < b;
  }

  proc chpl_field_gt(a: [] ?t, b: [] t) {
    compilerError("ordered comparisons not supported by default on records with array fields");
  }

  inline proc chpl_field_gt(a, b) where !isArrayType(a.type) {
    return a > b;
  }

  // check if both arguments are local without `.locale` or `here`
  proc chpl__bothLocal(const ref a, const ref b) {
    extern proc chpl_equals_localeID(const ref x, const ref y): bool;

    const aLoc = __primitive("_wide_get_locale", a._value);
    const bLoc = __primitive("_wide_get_locale", b._value);

    return chpl_equals_localeID(aLoc, bLoc) &&
           chpl_equals_localeID(aLoc, here_id);
  }
}

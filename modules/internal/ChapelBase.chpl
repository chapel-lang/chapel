/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
  use ChapelStandard;
  private use ChapelEnv, SysCTypes;

  // These two are called by compiler-generated code.
  extern proc chpl_config_has_value(name:c_string, module_name:c_string): bool;
  extern proc chpl_config_get_value(name:c_string, module_name:c_string): c_string;

  // Is the cache for remote data enabled at compile time?
  config param CHPL_CACHE_REMOTE: bool = false;

  // minimum buffer size allocated for string/bytes
  config param chpl_stringMinAllocSize = 0;

  config param warnMaximalRange = false;    // Warns if integer rollover will cause
                                            // the iterator to yield zero times.
  proc _throwOpError(param op: string) {
      compilerError("illegal use of '", op, "' on operands of type uint(64) and signed integer");
  }

  //
  // compile-time diagnostics
  //
  // Note: the message printed by "error" and "warning" primitives
  // consists of the formals of the enclosing function, not their own args.
  //

  proc compilerError(param msg: string ...?n, param errorDepth: int) {
    __primitive("error");
  }

  proc compilerError(param msg: string ...?n) {
    __primitive("error");
  }

  proc compilerWarning(param msg: string ...?n, param errorDepth: int) {
    __primitive("warning");
  }

  proc compilerWarning(param msg: string ...?n) {
    __primitive("warning");
  }

  proc compilerAssert(param test: bool)
  { if !test then compilerError("assert failed"); }

  proc compilerAssert(param test: bool, param errorDepth: int)
  { if !test then compilerError("assert failed", errorDepth + 1); }

  proc compilerAssert(param test: bool, param msg: string ...?n)
  { if !test then compilerError("assert failed - ", (...msg)); }

  proc compilerAssert(param test: bool, param msg: string ...?n, param errorDepth: int)
  { if !test then compilerError("assert failed - ", (...msg), errorDepth + 1); }


  pragma "object class"
  pragma "global type symbol"
  pragma "no object"
  class _object { }


  enum iterKind {leader, follower, standalone};


  //
  // assignment on primitive types
  //
  inline proc =(ref a: bool(?), b: bool) { __primitive("=", a, b); }
  inline proc =(ref a: int(?w), b: int(w)) { __primitive("=", a, b); }
  inline proc =(ref a: uint(?w), b: uint(w)) { __primitive("=", a, b); }
  inline proc =(ref a: real(?w), b: real(w)) { __primitive("=", a, b); }
  inline proc =(ref a: imag(?w), b: imag(w)) { __primitive("=", a, b); }
  inline proc =(ref a: complex(?w), b: complex(w)) { __primitive("=", a, b); }
  inline proc =(ref a:opaque, b:opaque) {__primitive("=", a, b); }
  inline proc =(ref a:enumerated, b:enumerated) where (a.type == b.type) {__primitive("=", a, b); }

  inline proc =(ref a, b: a.type) where isBorrowedOrUnmanagedClassType(a.type)
  {
    __primitive("=", a, b);
  }

  pragma "compiler generated"
  pragma "last resort" // so user-supplied assignment will override this one.
    // The CG pragma is needed because this function interferes with
    // assignments defined for sync and single class types.
  inline proc =(ref a, b:_nilType) where isBorrowedOrUnmanagedClassType(a.type)
  {
    if isNonNilableClassType(a.type) {
      compilerError("cannot assign to " + a.type:string + " from nil");
    }
    __primitive("=", a, nil);
  }

  inline proc =(ref a: nothing, b: ?t) where t != nothing {
    compilerError("a nothing variable cannot be assigned");
  }

  inline proc =(ref a: ?t, b: nothing) where t != nothing {
    compilerError("cannot assign none to a variable of non-nothing type");
  }

  // This needs to be param so calls to it are removed after they are resolved
  inline proc =(ref a: nothing, b: nothing) param { }

  //
  // equality comparison on primitive types
  //
  inline proc ==(a: _nilType, b: _nilType) param return true;
  inline proc ==(a: bool, b: bool) return __primitive("==", a, b);
  inline proc ==(a: int(?w), b: int(w)) return __primitive("==", a, b);
  inline proc ==(a: uint(?w), b: uint(w)) return __primitive("==", a, b);
  inline proc ==(a: real(?w), b: real(w)) return __primitive("==", a, b);
  inline proc ==(a: imag(?w), b: imag(w)) return __primitive("==", a, b);
  inline proc ==(a: complex(?w), b: complex(w)) return a.re == b.re && a.im == b.im;
  inline proc ==(a: borrowed object?, b: borrowed object?) return __primitive("ptr_eq", a, b);
  inline proc ==(a: enumerated, b: enumerated) where (a.type == b.type) {
    return __primitive("==", a, b);
  }
  pragma "last resort"
  inline proc ==(a: enumerated, b: enumerated) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return false;
  }

  inline proc !=(a: _nilType, b: _nilType) param return false;
  inline proc !=(a: bool, b: bool) return __primitive("!=", a, b);
  inline proc !=(a: int(?w), b: int(w)) return __primitive("!=", a, b);
  inline proc !=(a: uint(?w), b: uint(w)) return __primitive("!=", a, b);
  inline proc !=(a: real(?w), b: real(w)) return __primitive("!=", a, b);
  inline proc !=(a: imag(?w), b: imag(w)) return __primitive("!=", a, b);
  inline proc !=(a: complex(?w), b: complex(w)) return a.re != b.re || a.im != b.im;
  inline proc !=(a: borrowed object?, b: borrowed object?) return __primitive("ptr_neq", a, b);
  inline proc !=(a: enumerated, b: enumerated) where (a.type == b.type) {
    return __primitive("!=", a, b);
  }
  pragma "last resort"
  inline proc !=(a: enumerated, b: enumerated) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return true;
  }

  inline proc ==(param a: bool, param b: bool) param return __primitive("==", a, b);
  inline proc ==(param a: int(?w), param b: int(w)) param return __primitive("==", a, b);
  inline proc ==(param a: uint(?w), param b: uint(w)) param return __primitive("==", a, b);
  //
  inline proc ==(param a: enumerated, param b: enumerated) param where (a.type == b.type) return __primitive("==", a, b);
  //
  // NOTE: For param enums, Only '==' is implemented in the compiler
  // as a primitive. It assumes that the two param enums are of the
  // same type, as guaranteed by the where clause above.  All other
  // param enum routines are defined as module code to avoid having to
  // teach the compiler how to implement all enum comparisons.

  inline proc ==(param a: real(?w), param b: real(w)) param return __primitive("==", a, b);
  inline proc ==(param a: imag(?w), param b: imag(w)) param return __primitive("==", a, b);
  inline proc ==(param a: complex(?w), param b: complex(w)) param return __primitive("==", a, b);
  inline proc ==(a: nothing, b: nothing) param return true;

  inline proc !=(param a: bool, param b: bool) param return __primitive("!=", a, b);
  inline proc !=(param a: int(?w), param b: int(w)) param return __primitive("!=", a, b);
  inline proc !=(param a: uint(?w), param b: uint(w)) param return __primitive("!=", a, b);

  inline proc !=(param a: enumerated, param b: enumerated) param where (a.type == b.type) return __primitive("!=", chpl__enumToOrder(a), chpl__enumToOrder(b));

  inline proc !=(param a: real(?w), param b: real(w)) param return __primitive("!=", a, b);
  inline proc !=(param a: imag(?w), param b: imag(w)) param return __primitive("!=", a, b);
  inline proc !=(param a: complex(?w), param b: complex(w)) param return __primitive("!=", a, b);
  inline proc !=(a: nothing, b: nothing) param return false;

  //
  // ordered comparison on primitive types
  //
  inline proc <=(a: int(?w), b: int(w)) return __primitive("<=", a, b);
  inline proc <=(a: uint(?w), b: uint(w)) return __primitive("<=", a, b);
  inline proc <=(a: real(?w), b: real(w)) return __primitive("<=", a, b);
  proc <=(a: enumerated, b: enumerated) where (a.type == b.type) {
    return __primitive("<=", chpl__enumToOrder(a), chpl__enumToOrder(b));
  }
  pragma "last resort"
  inline proc <=(a: enumerated, b: enumerated) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return false;
  }

  inline proc >=(a: int(?w), b: int(w)) return __primitive(">=", a, b);
  inline proc >=(a: uint(?w), b: uint(w)) return __primitive(">=", a, b);
  inline proc >=(a: real(?w), b: real(w)) return __primitive(">=", a, b);
  proc >=(a: enumerated, b: enumerated) where (a.type == b.type) {
    return __primitive(">=", chpl__enumToOrder(a), chpl__enumToOrder(b));
  }
  pragma "last resort"
  inline proc >=(a: enumerated, b: enumerated) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return false;
  }

  inline proc <(a: int(?w), b: int(w)) return __primitive("<", a, b);
  inline proc <(a: uint(?w), b: uint(w)) return __primitive("<", a, b);
  inline proc <(a: real(?w), b: real(w)) return __primitive("<", a, b);
  proc <(a: enumerated, b: enumerated) where (a.type == b.type) {
    return __primitive("<", chpl__enumToOrder(a), chpl__enumToOrder(b));
  }
  pragma "last resort"
  inline proc <(a: enumerated, b: enumerated) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return false;
  }

  inline proc >(a: int(?w), b: int(w)) return __primitive(">", a, b);
  inline proc >(a: uint(?w), b: uint(w)) return __primitive(">", a, b);
  inline proc >(a: real(?w), b: real(w)) return __primitive(">", a, b);
  proc >(a: enumerated, b: enumerated) where (a.type == b.type) {
    return __primitive(">", chpl__enumToOrder(a), chpl__enumToOrder(b));
  }
  pragma "last resort"
  inline proc >(a: enumerated, b: enumerated) where (a.type != b.type) {
    compilerError("Comparisons between mixed enumerated types not supported by default");
    return false;
  }

  inline proc <=(param a: int(?w), param b: int(w)) param return __primitive("<=", a, b);
  inline proc <=(param a: uint(?w), param b: uint(w)) param return __primitive("<=", a, b);
  inline proc <=(param a: enumerated, param b: enumerated) param where (a.type == b.type) return __primitive("<=", chpl__enumToOrder(a), chpl__enumToOrder(b));
  inline proc <=(param a: real(?w), param b: real(w)) param return __primitive("<=", a, b);

  inline proc >=(param a: int(?w), param b: int(w)) param return __primitive(">=", a, b);
  inline proc >=(param a: uint(?w), param b: uint(w)) param return __primitive(">=", a, b);
  inline proc >=(param a: enumerated, param b: enumerated) param where (a.type == b.type) return __primitive(">=", chpl__enumToOrder(a), chpl__enumToOrder(b));
  inline proc >=(param a: real(?w), param b: real(w)) param return __primitive(">=", a, b);

  inline proc <(param a: int(?w), param b: int(w)) param return __primitive("<", a, b);
  inline proc <(param a: uint(?w), param b: uint(w)) param return __primitive("<", a, b);
  inline proc <(param a: enumerated, param b: enumerated) param where (a.type == b.type) return __primitive("<", chpl__enumToOrder(a), chpl__enumToOrder(b));
  inline proc <(param a: real(?w), param b: real(w)) param return __primitive("<", a, b);

  inline proc >(param a: int(?w), param b: int(w)) param return __primitive(">", a, b);
  inline proc >(param a: uint(?w), param b: uint(w)) param return __primitive(">", a, b);
  inline proc >(param a: enumerated, param b: enumerated) param where (a.type == b.type) return __primitive(">", chpl__enumToOrder(a), chpl__enumToOrder(b));
  inline proc >(param a: real(?w), param b: real(w)) param return __primitive(">", a, b);

  //
  // unary + and - on primitive types
  //
  inline proc +(a: int(?w)) return a;
  inline proc +(a: uint(?w)) return a;
  inline proc +(a: real(?w)) return a;
  inline proc +(a: imag(?w)) return a;
  inline proc +(a: complex(?w)) return a;

  inline proc -(a: int(?w)) return __primitive("u-", a);
  inline proc -(a: uint(64)) { compilerError("illegal use of '-' on operand of type ", a.type:string); }
  inline proc -(a: real(?w)) return __primitive("u-", a);
  inline proc -(a: imag(?w)) return __primitive("u-", a);
  inline proc -(a: complex(?w)) return __primitive("u-", a);

  inline proc +(param a: int(?w)) param return a;
  inline proc +(param a: uint(?w)) param return a;
  inline proc +(param a: real(?w)) param return a;
  inline proc +(param a: imag(?w)) param return a;
  inline proc +(param a: complex(?w)) param return a;

  inline proc -(param a: int(?w)) param return __primitive("u-", a);
  inline proc -(param a: uint(?w)) param {
    if (a:int(w) < 0) then
      compilerError("illegal use of '-' on operand of type ", a.type:string);
    else
      return -(a:int(w));
  }

  inline proc -(param a: real(?w)) param return __primitive("u-", a);
  inline proc -(param a: imag(?w)) param return __primitive("u-", a);
  inline proc -(param a: complex(?w)) param return __primitive("u-", a);

  //
  // binary + and - on primitive types for runtime values
  //
  inline proc +(a: int(?w), b: int(w)) return __primitive("+", a, b);
  inline proc +(a: uint(?w), b: uint(w)) return __primitive("+", a, b);
  inline proc +(a: real(?w), b: real(w)) return __primitive("+", a, b);
  inline proc +(a: imag(?w), b: imag(w)) return __primitive("+", a, b);
  inline proc +(a: complex(?w), b: complex(w)) return __primitive("+", a, b);

  inline proc +(a: real(?w), b: imag(w)) return (a, _i2r(b)):complex(w*2);
  inline proc +(a: imag(?w), b: real(w)) return (b, _i2r(a)):complex(w*2);
  inline proc +(a: real(?w), b: complex(w*2)) return (a+b.re, b.im):complex(w*2);
  inline proc +(a: complex(?w), b: real(w/2)) return (a.re+b, a.im):complex(w);
  inline proc +(a: imag(?w), b: complex(w*2)) return (b.re, _i2r(a)+b.im):complex(w*2);
  inline proc +(a: complex(?w), b: imag(w/2)) return (a.re, a.im+_i2r(b)):complex(w);

  inline proc -(a: int(?w), b: int(w)) return __primitive("-", a, b);
  inline proc -(a: uint(?w), b: uint(w)) return __primitive("-", a, b);
  inline proc -(a: real(?w), b: real(w)) return __primitive("-", a, b);
  inline proc -(a: imag(?w), b: imag(w)) return __primitive("-", a, b);
  inline proc -(a: complex(?w), b: complex(w)) return __primitive("-", a, b);

  inline proc -(a: real(?w), b: imag(w)) return (a, -_i2r(b)):complex(w*2);
  inline proc -(a: imag(?w), b: real(w)) return (-b, _i2r(a)):complex(w*2);
  inline proc -(a: real(?w), b: complex(w*2)) return (a-b.re, -b.im):complex(w*2);
  inline proc -(a: complex(?w), b: real(w/2)) return (a.re-b, a.im):complex(w);
  inline proc -(a: imag(?w), b: complex(w*2)) return (-b.re, _i2r(a)-b.im):complex(w*2);
  inline proc -(a: complex(?w), b: imag(w/2)) return (a.re, a.im-_i2r(b)):complex(w);

  //
  // binary + and - on param values
  //
  inline proc +(param a: int(?w), param b: int(w)) param return __primitive("+", a, b);
  inline proc +(param a: uint(?w), param b: uint(w)) param return __primitive("+", a, b);
  inline proc +(param a: real(?w), param b: real(w)) param return __primitive("+", a, b);
  inline proc +(param a: imag(?w), param b: imag(w)) param return __primitive("+", a, b);
  inline proc +(param a: complex(?w), param b: complex(w)) param return __primitive("+", a, b);
  /*inline proc +(param a: real(?w), param b: imag(w)) param return __primitive("+", a, b);
  inline proc +(param a: imag(?w), param b: real(w)) param return __primitive("+", a, b);
  inline proc +(param a: real(?w), param b: complex(w*2)) param return
  __primitive("+", a, b);*/

  inline proc -(param a: int(?w), param b: int(w)) param return __primitive("-", a, b);
  inline proc -(param a: uint(?w), param b: uint(w)) param return __primitive("-", a, b);
  inline proc -(param a: real(?w), param b: real(w)) param return __primitive("-", a, b);
  inline proc -(param a: imag(?w), param b: imag(w)) param return __primitive("-", a, b);
  inline proc -(param a: complex(?w), param b: complex(w)) param return __primitive("-", a, b);
  /*inline proc -(param a: real(?w), param b: imag(w)) param return __primitive("-", a, b);
  inline proc -(param a: imag(?w), param b: real(w)) param return __primitive("-", a, b);
  inline proc -(param a: real(?w), param b: complex(w*2)) param return
  __primitive("-", a, b);*/

  //
  // * and / on primitive types
  //
  inline proc *(a: int(?w), b: int(w)) return __primitive("*", a, b);
  inline proc *(a: uint(?w), b: uint(w)) return __primitive("*", a, b);
  inline proc *(a: real(?w), b: real(w)) return __primitive("*", a, b);
  inline proc *(a: imag(?w), b: imag(w)) return _i2r(__primitive("*", -a, b));
  inline proc *(a: complex(?w), b: complex(w)) return __primitive("*", a, b);

  inline proc *(a: real(?w), b: imag(w)) return _r2i(a*_i2r(b));
  inline proc *(a: imag(?w), b: real(w)) return _r2i(_i2r(a)*b);
  inline proc *(a: real(?w), b: complex(w*2)) return (a*b.re, a*b.im):complex(w*2);
  inline proc *(a: complex(?w), b: real(w/2)) return (a.re*b, a.im*b):complex(w);
  inline proc *(a: imag(?w), b: complex(w*2)) return (-_i2r(a)*b.im, _i2r(a)*b.re):complex(w*2);
  inline proc *(a: complex(?w), b: imag(w/2)) return (-a.im*_i2r(b), a.re*_i2r(b)):complex(w);

  inline proc /(a: int(?w), b: int(w)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline proc /(a: uint(?w), b: uint(w)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline proc /(a: real(?w), b: real(w)) return __primitive("/", a, b);
  inline proc /(a: imag(?w), b: imag(w)) return _i2r(__primitive("/", a, b));
  inline proc /(a: complex(?w), b: complex(w)) return __primitive("/", a, b);

  inline proc /(a: real(?w), b: imag(w)) return _r2i(-a/_i2r(b));
  inline proc /(a: imag(?w), b: real(w)) return _r2i(_i2r(a)/b);
  inline proc /(a: real(?w), b: complex(w*2))
    return let d = abs(b) in
    ((a/d)*(b.re/d), (-a/d)*(b.im/d)):complex(w*2);
  inline proc /(a: complex(?w), b: real(w/2))
    return (a.re/b, a.im/b):complex(w);
  inline proc /(a: imag(?w), b: complex(w*2))
    return let d = abs(b) in
    ((_i2r(a)/d)*(b.im/d), (_i2r(a)/d)*(b.re/d)):complex(w*2);
  inline proc /(a: complex(?w), b: imag(w/2))
    return (a.im/_i2r(b), -a.re/_i2r(b)):complex(w);

  inline proc *(param a: int(?w), param b: int(w)) param return __primitive("*", a, b);
  inline proc *(param a: uint(?w), param b: uint(w)) param return __primitive("*", a, b);
  inline proc *(param a: real(?w), param b: real(w)) param return __primitive("*", a, b);
  inline proc *(param a: imag(?w), param b: imag(w)) param {
    return __primitive("*", -a, b):real(w);
  }
  inline proc *(param a: real(?w), param b: imag(w)) param {
    return __primitive("*", a, b:real(w)):imag(w);
  }
  inline proc *(param a: imag(?w), param b: real(w)) param {
    return __primitive("*", a:real(w), b):imag(w);
  }

  inline proc /(param a: int(?w), param b: int(w)) param {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline proc /(param a: uint(?w), param b: uint(w)) param {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline proc /(param a: real(?w), param b: real(w)) param {
    return __primitive("/", a, b);
  }
  inline proc /(param a: imag(?w), param b: imag(w)) param {
    return __primitive("/", a, b):real(w);
  }
  inline proc /(param a: real(?w), param b: imag(w)) param {
    return __primitive("/", -a, b:real(w)):imag(w);
  }
  inline proc /(param a: imag(?w), param b: real(w)) param {
    return __primitive("/", a:real(w), b):imag(w);
  }


  //
  // % on primitive types
  //
  inline proc %(a: int(?w), b: int(w)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline proc %(a: uint(?w), b: uint(w)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }

  inline proc %(param a: int(?w), param b: int(w)) param {
    if b == 0 then
      compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline proc %(param a: uint(?w), param b: uint(w)) param {
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

  inline proc **(a: int(?w), b: int(w)) return _intExpHelp(a, b);
  inline proc **(a: uint(?w), b: uint(w)) return _intExpHelp(a, b);
  inline proc **(a: real(?w), b: real(w)) return __primitive("**", a, b);
  inline proc **(a: complex(?w), b: complex(w)) {
    if a.type == complex(128) {
      pragma "fn synchronization free"
      extern proc cpow(x: complex(128), y: complex(128)): complex(128);
      return cpow(a, b);
    } else {
      pragma "fn synchronization free"
      extern proc cpowf(x: complex(64), y: complex(64)): complex(64);
      return cpowf(a, b);
    }
  }

  proc **(param a: int(?w), param b: int(w)) param return __primitive("**", a, b);
  proc **(param a: uint(?w), param b: uint(w)) param return __primitive("**", a, b);

  inline proc _expHelp(a, param b: integral) {
    if b == 0 then
      return 1:a.type;
    else if b == 1 then
      return a;
    else if b == 2 then
      return a*a;
    else if b == 3 then
      return a*a*a;
    else if b == 4 then
      return let t=a*a in t*t;
    else if b == 5 then
      return let t=a*a in t*t*a;
    else if b == 6 then
      return let t=a*a in t*t*t;
    else if b == 8 then
      return let t=a*a, u=t*t in u*u;
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

  proc _canOptimizeExp(param b: integral) param return b >= 0 && b <= 8 && b != 7;

  // complement and compare is an efficient way to test for a power of 2
  proc _basePowerTwo(param a: integral) param return (a > 0 && ((a & (~a + 1)) == a));

  inline proc **(a: int(?w), param b: integral) where _canOptimizeExp(b) return _expHelp(a, b);
  inline proc **(a: uint(?w), param b: integral) where _canOptimizeExp(b) return _expHelp(a, b);
  inline proc **(a: real(?w), param b: integral) where _canOptimizeExp(b) return _expHelp(a, b);
  inline proc **(param a: integral, b: int) where _basePowerTwo(a) return _expBaseHelp(a, b);

  //
  // logical operations on primitive types
  //
  inline proc !(a: bool) return __primitive("!", a);
  inline proc !(a: int(?w)) return (a == 0);
  inline proc !(a: uint(?w)) return (a == 0);

  inline proc isTrue(a: bool) return a;
  inline proc isTrue(param a: bool) param return a;

  proc isTrue(a: integral) { compilerError("short-circuiting logical operators not supported on integers"); }

  inline proc !(param a: bool) param return __primitive("!", a);
  inline proc !(param a: int(?w)) param return (a == 0);
  inline proc !(param a: uint(?w)) param return (a == 0);

  //
  // bitwise operations on primitive types
  //
  inline proc ~(a: int(?w)) return __primitive("u~", a);
  inline proc ~(a: uint(?w)) return __primitive("u~", a);
  inline proc ~(a: bool) { compilerError("~ is not supported on operands of boolean type"); }

  inline proc &(a: bool, b: bool) return __primitive("&", a, b);
  inline proc &(a: int(?w), b: int(w)) return __primitive("&", a, b);
  inline proc &(a: uint(?w), b: uint(w)) return __primitive("&", a, b);
  inline proc &(a: uint(?w), b: int(w)) return __primitive("&", a, b:uint(w));
  inline proc &(a: int(?w), b: uint(w)) return __primitive("&", a:uint(w), b);

  inline proc |(a: bool, b: bool) return __primitive("|", a, b);
  inline proc |(a: int(?w), b: int(w)) return __primitive("|", a, b);
  inline proc |(a: uint(?w), b: uint(w)) return __primitive("|", a, b);
  inline proc |(a: uint(?w), b: int(w)) return __primitive("|", a, b:uint(w));
  inline proc |(a: int(?w), b: uint(w)) return __primitive("|", a:uint(w), b);

  inline proc ^(a: bool, b: bool) return __primitive("^", a, b);
  inline proc ^(a: int(?w), b: int(w)) return __primitive("^", a, b);
  inline proc ^(a: uint(?w), b: uint(w)) return __primitive("^", a, b);
  inline proc ^(a: uint(?w), b: int(w)) return __primitive("^", a, b:uint(w));
  inline proc ^(a: int(?w), b: uint(w)) return __primitive("^", a:uint(w), b);

  inline proc ~(param a: bool) { compilerError("~ is not supported on operands of boolean type"); }
  inline proc ~(param a: int(?w)) param return __primitive("u~", a);
  inline proc ~(param a: uint(?w)) param return __primitive("u~", a);

  inline proc &(param a: bool, param b: bool) param return __primitive("&", a, b);
  inline proc &(param a: int(?w), param b: int(w)) param return __primitive("&", a, b);
  inline proc &(param a: uint(?w), param b: uint(w)) param return __primitive("&", a, b);
  inline proc &(param a: uint(?w), param b: int(w)) param return __primitive("&", a, b:uint(w));
  inline proc &(param a: int(?w), param b: uint(w)) param return __primitive("&", a:uint(w), b);

  inline proc |(param a: bool, param b: bool) param return __primitive("|", a, b);
  inline proc |(param a: int(?w), param b: int(w)) param return __primitive("|", a, b);
  inline proc |(param a: uint(?w), param b: uint(w)) param return __primitive("|", a, b);
  inline proc |(param a: uint(?w), param b: int(w)) param return __primitive("|", a, b:uint(w));
  inline proc |(param a: int(?w), param b: uint(w)) param return __primitive("|", a:uint(w), b);

  inline proc ^(param a: bool, param b: bool) param return __primitive("^", a, b);
  inline proc ^(param a: int(?w), param b: int(w)) param return __primitive("^", a, b);
  inline proc ^(param a: uint(?w), param b: uint(w)) param return __primitive("^", a, b);
  inline proc ^(param a: uint(?w), param b: int(w)) param return __primitive("^", a, b:uint(w));
  inline proc ^(param a: int(?w), param b: uint(w)) param return __primitive("^", a:uint(w), b);

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

  inline proc <<(a: int(?w), b: integral) {
    if boundsChecking then bitshiftChecks(a, b);
    // Intentionally cast `a` to `uint(w)` for an unsigned left shift.
    return __primitive("<<", a:uint(w), b):int(w);
  }

  inline proc <<(a: uint(?w), b: integral) {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive("<<", a, b);
  }

  inline proc >>(a: int(?w), b: integral) {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive(">>", a, b);
  }

  inline proc >>(a: uint(?w), b: integral) {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive(">>", a, b);
  }

  inline proc <<(param a: int(?w), param b: integral) param {
    if boundsChecking then bitshiftChecks(a, b);
    // Intentionally cast `a` to `uint(w)` for an unsigned left shift.
    return __primitive("<<", a:uint(w), b):int(w);
  }

  inline proc <<(param a: uint(?w), param b: integral) param {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive("<<", a, b);
  }

  inline proc >>(param a: int(?w), param b: integral) param {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive(">>", a, b);
  }

  inline proc >>(param a: uint(?w), param b: integral) param {
    if boundsChecking then bitshiftChecks(a, b);
    return __primitive(">>", a, b);
  }

  pragma "always propagate line file info"
  private inline proc checkNotNil(x:borrowed class?) {
    use HaltWrappers only;
    // Check only if --nil-checks is enabled
    if chpl_checkNilDereferences {
      // Add check for nilable types only.
      if x == nil {
        HaltWrappers.nilCheckHalt("argument to ! is nil");
      }
    }
  }

  inline proc postfix!(type t: class) type {
    compilerWarning("applying the postfix-! operator to a type is deprecated; instead use a cast to 'class' or 'borrowed class', e.g. 'MyType :borrowed class'");
    return _to_borrowed(_to_nonnil(t));
  }
  inline proc postfix!(type t: class?) type {
    compilerWarning("applying the postfix-! operator to a type is deprecated; instead use a cast to 'class' or 'borrowed class', e.g. 'MyType :borrowed class'");
    return _to_borrowed(_to_nonnil(t));
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
  inline proc chpl_statementLevelSymbol(a: sync)  { a.readFE(); }
  inline proc chpl_statementLevelSymbol(a: single) { a.readFF(); }
  inline proc chpl_statementLevelSymbol(param a) param { return a; }
  inline proc chpl_statementLevelSymbol(type a) type { return a; }

  //
  // If an iterator is called without capturing the result, iterate over it
  // to ensure any side effects it has will happen.
  //
  inline proc chpl_statementLevelSymbol(ir: _iteratorRecord) {
    iter _ir_copy_recursive(ir) {
      for e in ir do
        yield chpl__initCopy(e);
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

  inline proc _cond_test(x: borrowed object?) return x != nil;
  inline proc _cond_test(x: bool) return x;
  inline proc _cond_test(x: integral) return x != 0:x.type;

  inline proc _cond_test(param x: bool) param return x;
  inline proc _cond_test(param x: integral) param return x != 0:x.type;

  inline proc _cond_test(x) {
    compilerError("type '", x.type:string, "' used in if or while condition");
  }

  inline proc _cond_test(x: _iteratorRecord) {
    compilerError("iterator or promoted expression ", x.type:string, " used in if or while condition");
  }

  proc _cond_invalid(x: borrowed object) param return false;
  proc _cond_invalid(x: bool) param return false;
  proc _cond_invalid(x: integral) param return false;
  proc _cond_invalid(x) param return true;

  //
  // isNonnegative(i) == (i>=0), but is a param value if i is unsigned.
  //
  inline proc isNonnegative(i: int(?)) return i >= 0;
  inline proc isNonnegative(i: uint(?)) param return true;
  inline proc isNonnegative(param i) param return i >= 0;


  //
  // complex component methods re and im
  //
  inline proc ref chpl_anycomplex.re ref {
    return __primitive("complex_get_real", this);
  }
  inline proc chpl_anycomplex.re {
    if this.type == complex(128) {
      pragma "fn synchronization free"
      extern proc creal(x:complex(128)): real(64);
      return creal(this);
    } else {
      pragma "fn synchronization free"
      extern proc crealf(x:complex(64)): real(32);
      return crealf(this);
    }
  }
  inline proc ref chpl_anycomplex.im ref {
    return __primitive("complex_get_imag", this);
  }
  inline proc chpl_anycomplex.im {
    if this.type == complex(128) {
      pragma "fn synchronization free"
      extern proc cimag(x:complex(128)): real(64);
      return cimag(this);
    } else {
      pragma "fn synchronization free"
      extern proc cimagf(x:complex(64)): real(32);
      return cimagf(this);
    }
  }

  //
  // helper functions
  //
  inline proc _i2r(a: imag(?w)) return __primitive("cast", real(w), a);
  inline proc _r2i(a: real(?w)) return __primitive("cast", imag(w), a);

  //
  // min and max
  //
  inline proc min(x: int(?w), y: int(w)) return if x < y then x else y;
  inline proc max(x: int(?w), y: int(w)) return if x > y then x else y;

  inline proc min(x: uint(?w), y: uint(w)) return if x < y then x else y;
  inline proc max(x: uint(?w), y: uint(w)) return if x > y then x else y;

  inline proc min(x: real(?w), y: real(w)) return if (x < y) | isnan(x) then x else y;
  inline proc max(x: real(?w), y: real(w)) return if (x > y) | isnan(x) then x else y;

  inline proc min(x, y) return if x < y then x else y;
  inline proc max(x, y) return if x > y then x else y;

  inline proc min(x, y, z...?k) return min(min(x, y), (...z));
  inline proc max(x, y, z...?k) return max(max(x, y), (...z));

  inline proc min(param x: int(?w), param y: int(w)) param
    return if x < y then x else y;
  inline proc max(param x: int(?w), param y: int(w)) param
    return if x > y then x else y;

  inline proc min(param x: uint(?w), param y: uint(w)) param
    return if x < y then x else y;
  inline proc max(param x: uint(?w), param y: uint(w)) param
    return if x > y then x else y;

  inline proc min(param x: real(?w), param y: real(w)) param
    return if x < y then x else y;
  inline proc max(param x: real(?w), param y: real(w)) param
    return if x > y then x else y;

  inline proc min(param x: imag(?w), param y: imag(w)) param
    return if x < y then x else y;
  inline proc max(param x: imag(?w), param y: imag(w)) param
    return if x > y then x else y;

  inline proc min(x, y) where isAtomic(x) || isAtomic(y) {
    compilerError("min() and max() are not supported for atomic arguments - apply read() to those arguments first");
  }

  inline proc max(x, y) where isAtomic(x) || isAtomic(y) {
    compilerError("min() and max() are not supported for atomic arguments - apply read() to those arguments first");
  }

  //
  // More primitive funs
  //
  pragma "function terminates program"
  inline proc exit(status: int=0) {
    __primitive("chpl_exit_any", status);
  }

  enum ArrayInit {heuristicInit, noInit, serialInit, parallelInit};
  config param chpl_defaultArrayInitMethod = ArrayInit.heuristicInit;

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

  pragma "unsafe" // work around problems storing non-nilable classes
  proc init_elts(x, s, type t) : void {
    var initMethod = chpl_getArrayInitMethod();

    // for uints, check that s > 0, so the `s-1` below doesn't overflow
    if isUint(s) && s == 0 {
      initMethod = ArrayInit.noInit;
    } else if initMethod == ArrayInit.heuristicInit {
      // Heuristically determine if we should do parallel initialization. The
      // current heuristic really just checks that we have a POD array that's
      // at least 2MB. This value was chosen experimentally: Any smaller and the
      // cost of a forall (mostly the task creation) outweighs the benefit of
      // using multiple tasks. This was tested on a 2 core laptop, 8 core
      // workstation, and 24 core XC40.
      //
      // Ideally we want to be able to do parallel initialization for all types,
      // but we're currently blocked by an issue with arrays of arrays and thus
      // arrays of aggregate types where at one field is an array. The issue is
      // basically that an array's domain stores a linked list of all its arrays
      // and removal becomes expensive when addition and removal occur in
      // different orders.
      //
      // Long term we probably want to store the domain's arrays as an
      // associative domain or some data structure with < log(n) find/add/remove
      // times. Currently we can't do that because the domain's arrays are part
      // of the base domain, so we have a circular reference.
      if !isPODType(t) {
        initMethod = ArrayInit.serialInit;
      } else {
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
    }

    // The parallel range iter uses 'here`/rootLocale, so fallback to serial
    // initialization if the root locale hasn't been setup
    if initMethod == ArrayInit.parallelInit && !rootLocaleInitialized {
      initMethod = ArrayInit.serialInit;
    }

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
        for i in 0..s-1 {
          pragma "no auto destroy" var y: t;
          __primitive("array_set_first", x, i, y);
        }
      }
      when ArrayInit.parallelInit {
        forall i in 0..s-1 {
          pragma "no auto destroy" var y: t;
          __primitive("array_set_first", x, i, y);
        }
      }
      otherwise {
        halt("ArrayInit.heuristicInit should have been made concrete");
      }
    }
  }

  // TODO (EJR: 02/25/16): see if we can remove this explicit type declaration.
  // chpl_mem_descInt_t is really a well known compiler type since the compiler
  // emits calls for the chpl_mem_descs table. Maybe the compiler should just
  // create the type and export it to the runtime?
  pragma "no doc"
  extern type chpl_mem_descInt_t = int(16);

  // dynamic data block class
  // (note that c_ptr(type) is similar, but local only,
  //  and defined in SysBasic.chpl)
  pragma "data class"
  pragma "no object"
  pragma "no default functions"
  class _ddata {
    type eltType;

    inline proc this(i: integral) ref {
      return __primitive("array_get", this, i);
    }
  }

  proc chpl_isDdata(type t:_ddata) param return true;
  proc chpl_isDdata(type t) param return false;

  inline proc =(ref a: _ddata(?t), b: _ddata(t)) {
    __primitive("=", a, b);
  }

  // Removing the 'eltType' arg results in errors for --baseline
  inline proc _ddata_shift(type eltType, data: _ddata(eltType), shift: integral) {
    var ret: _ddata(eltType);
     __primitive("shift_base_pointer", ret, data, shift);
    return ret;
  }

  inline proc _ddata_sizeof_element(type t: _ddata): size_t {
    return __primitive("sizeof_ddata_element", t):size_t;
  }

  inline proc _ddata_sizeof_element(x: _ddata): size_t {
    return _ddata_sizeof_element(x.type);
  }

  inline proc _ddata_allocate(type eltType, size: integral,
                              subloc = c_sublocid_none,
                              param initElts: bool=true) {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_array_alloc(nmemb: size_t, eltSize: size_t,
                                     subloc: chpl_sublocID_t,
                                     ref callPostAlloc: bool): c_void_ptr;
    var ret: _ddata(eltType);
    var callPostAlloc: bool;
    ret = chpl_mem_array_alloc(size:size_t, _ddata_sizeof_element(ret),
                               subloc, callPostAlloc):ret.type;
    if initElts then
      init_elts(ret, size, eltType);
    if callPostAlloc {
      pragma "fn synchronization free"
      pragma "insert line file info"
      extern proc chpl_mem_array_postAlloc(data: c_void_ptr, nmemb: size_t,
                                           eltSize: size_t);
      chpl_mem_array_postAlloc(ret:c_void_ptr, size:size_t,
                               _ddata_sizeof_element(ret));
    }
    return ret;
  }

  inline proc _ddata_free(data: _ddata, size: integral) {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc chpl_mem_array_free(data: c_void_ptr,
                                    nmemb: size_t, eltSize: size_t);
    chpl_mem_array_free(data:c_void_ptr, size:size_t,
                        _ddata_sizeof_element(data));
  }

  inline proc ==(a: _ddata, b: _ddata) where a.eltType == b.eltType {
    return __primitive("ptr_eq", a, b);
  }
  inline proc ==(a: _ddata, b: _nilType) {
    return __primitive("ptr_eq", a, nil);
  }
  inline proc ==(a: _nilType, b: _ddata) {
    return __primitive("ptr_eq", nil, b);
  }

  inline proc !=(a: _ddata, b: _ddata) where a.eltType == b.eltType {
    return __primitive("ptr_neq", a, b);
  }
  inline proc !=(a: _ddata, b: _nilType) {
    return __primitive("ptr_neq", a, nil);
  }
  inline proc !=(a: _nilType, b: _ddata) {
    return __primitive("ptr_neq", nil, b);
  }


  inline proc _cond_test(x: _ddata) return x != nil;


  //
  // internal reference type
  //
  pragma "ref"
  pragma "no default functions"
  pragma "no object"
  class _ref {
    var _val;
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

    if numTasks >= dptpl then
      chpl_rt_reset_task_spawn();
  }

  config param useAtomicTaskCnt =  CHPL_NETWORK_ATOMICS!="none";

  // Parent class for _EndCount instances so that it's easy
  // to add non-generic fields here.
  // And to get 'errors' field from any generic instantiation.
  pragma "no default functions"
  class _EndCountBase {
    var errors: chpl_TaskErrors;
    var taskList: c_void_ptr;
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
      e.i.add(1, memoryOrder.release);
      e.taskCnt.add(1, memoryOrder.release);
    } else {
      // note that this on statement does not have the usual
      // remote memory fence because of pragma "no remote memory fence"
      // above. So we do an acquire fence before it.
      chpl_rmem_consist_fence(memoryOrder.release);
      on e {
        e.i.add(1, memoryOrder.release);
        e.taskCnt += 1;
      }
    }
    if countRunningTasks {
      here.runningTaskCntAdd(1);  // decrement is in _waitEndCount()
    }
  }

  pragma "dont disable remote value forwarding"
  pragma "no remote memory fence"
  pragma "task spawn impl fn"
  proc _upEndCount(e: _EndCount, param countRunningTasks=true, numTasks) {
    e.i.add(numTasks:int, memoryOrder.release);

    if countRunningTasks {
      here.runningTaskCntAdd(numTasks:int-1);  // decrement is in _waitEndCount()
    } else {
      here.runningTaskCntSub(1);
    }
  }

  pragma "task complete impl fn"
  extern proc chpl_comm_task_end(): void;

  pragma "task complete impl fn"
  proc chpl_after_forall_fence() {
    chpl_comm_task_end(); // TODO: change to chpl_comm_unordered_task_fence()
  }

  // This function is called once by each newly initiated task.  No on
  // statement is needed because the call to sub() will do a remote
  // fork (on) if needed.
  pragma "dont disable remote value forwarding"
  pragma "task complete impl fn"
  pragma "down end count fn"
  proc _downEndCount(e: _EndCount, err: unmanaged Error?) {
    chpl_save_task_error(e, err);
    chpl_comm_task_end();
    // inform anybody waiting that we're done
    e.i.sub(1, memoryOrder.release);
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

    // See if we can help with any of the started tasks
    chpl_taskListExecute(e.taskList);

    // Wait for all tasks to finish
    e.i.waitFor(0, memoryOrder.acquire);

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
    // See if we can help with any of the started tasks
    chpl_taskListExecute(e.taskList);

    // Wait for all tasks to finish
    e.i.waitFor(0, memoryOrder.acquire);

    if countRunningTasks {
      here.runningTaskCntSub(numTasks:int-1);
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
  proc _waitDynamicEndCount(param countRunningTasks=true) throws {
    var e = __primitive("get dynamic end count");
    _waitEndCount(e, countRunningTasks);

    // Throw any error raised by a task this sync statement is waiting for
    if ! e.errors.empty() then
      throw new owned TaskErrors(e.errors);
  }

  proc _do_command_line_cast(type t, x:c_string) throws {
    if isSyncType(t) then
      compilerError("config variables of sync type are not supported");
    if isSingleType(t) then
      compilerError("config variables of single type are not supported");
    if isAtomicType(t) then
      compilerError("config variables of atomic type are not supported");

    var str: string;
    try! {
      str = createStringWithNewBuffer(x);
    }
    if t == string {
      return str;
    } else {
      return str:t;
    }
  }
  // param s is used for error reporting
  pragma "command line setting"
  proc _command_line_cast(param s: c_string, type t, x:c_string) {
    try! {
      return _do_command_line_cast(t, x);
    }
  }


  //
  // Similar to isPrimitiveType, but excludes imaginaries because they
  // are handled within the Chapel code directly (using overloads further
  // down in the file) to save complexity in the compiler.
  //
  inline proc chpl_typeSupportsPrimitiveCast(type t) param
    return isBoolType(t) ||
           isIntegralType(t) ||
           isRealType(t);

  inline proc _cast(type t:chpl_anybool, x:chpl_anybool)
    return __primitive("cast", t, x);
  inline proc _cast(type t:integral, x:chpl_anybool)
    return __primitive("cast", t, x);
  inline proc _cast(type t:chpl_anyreal, x:chpl_anybool)
    return __primitive("cast", t, x);

  inline proc _cast(type t:chpl_anybool, x:integral)
    return __primitive("cast", t, x);
  inline proc _cast(type t:integral, x:integral)
    return __primitive("cast", t, x);
  inline proc _cast(type t:chpl_anyreal, x:integral)
    return __primitive("cast", t, x);

  inline proc _cast(type t:chpl_anybool, x:chpl_anyreal)
    return __primitive("cast", t, x);
  inline proc _cast(type t:integral, x:chpl_anyreal)
    return __primitive("cast", t, x);
  inline proc _cast(type t:chpl_anyreal, x:chpl_anyreal)
    return __primitive("cast", t, x);

  inline proc _cast(type t:chpl_anybool, x:enumerated)
    return x: int: bool;
  // _cast(type t:integral, x:enumerated)
  // is generated for each enum in buildDefaultFunctions
  inline proc _cast(type t:enumerated, x:enumerated) where x.type == t
    return x;
  inline proc _cast(type t:chpl_anyreal, x:enumerated)
    return x: int: real;

  inline proc _cast(type t:unmanaged class, x:_nilType)
  {
      compilerError("cannot cast nil to " + t:string);
  }
  inline proc _cast(type t:borrowed class, x:_nilType)
  {
    compilerError("cannot cast nil to " + t:string);
  }

  // casting to unmanaged?, no class downcast
  inline proc _cast(type t:unmanaged class?, x:borrowed class?)
    where isSubtype(_to_unmanaged(x.type),t)
  {
    return __primitive("cast", t, x);
  }
  inline proc _cast(type t:unmanaged class?, x:borrowed class)
    where isSubtype(_to_nonnil(_to_unmanaged(x.type)),t)
  {
    return __primitive("cast", t, x);
  }

  // casting to unmanaged, no class downcast
  inline proc _cast(type t:unmanaged class, x:borrowed class)
    where isSubtype(_to_unmanaged(x.type),t)
  {
    return __primitive("cast", t, x);
  }

  // casting away nilability, no class downcast
  inline proc _cast(type t:borrowed class, x:unmanaged class?) throws
    where isSubtype(_to_nonnil(x.type),t)
  {
    if x == nil {
      throw new owned NilClassError();
    }
    return __primitive("cast", t, x);
  }


  // casting away nilability, no class downcast
  inline proc _cast(type t:borrowed class, x:borrowed class?) throws
    where isSubtype(_to_nonnil(x.type),t)
  {
    if x == nil {
      throw new owned NilClassError();
    }
    return __primitive("cast", t, x);
  }

  // casting away nilability, no class downcast
  inline proc _cast(type t:unmanaged class, x:borrowed class?) throws
    where isSubtype(_to_nonnil(_to_unmanaged(x.type)),t)
  {
    if x == nil {
      throw new owned NilClassError();
    }
    return __primitive("cast", t, x);
  }

  // this version handles downcast to non-nil borrowed
  inline proc _cast(type t:borrowed class, x:borrowed class?) throws
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
  inline proc _cast(type t:borrowed class?, x:borrowed class?)
    where isProperSubtype(t,x.type)
  {
    if x == nil {
      return nil;
    }
    var tmp = __primitive("dynamic_cast", t, x);
    return _to_nilable(_to_borrowed(tmp));
  }


  // this version handles downcast to non-nil unmanaged
  inline proc _cast(type t:unmanaged class, x:borrowed class?) throws
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
  inline proc _cast(type t:unmanaged class?, x:borrowed class?)
    where isProperSubtype(t,_to_unmanaged(x.type))
  {
    if x == nil {
      return nil;
    }
    var tmp = __primitive("dynamic_cast", t, x);
    return _to_nilable(_to_unmanaged(tmp));
  }

  // this version handles downcast to nilable unmanaged
  inline proc _cast(type t:unmanaged class?, x:borrowed class)
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
  inline proc _cast(type t:chpl_anycomplex, x: bool)
    return (x, 0):t;

  inline proc _cast(type t:chpl_anycomplex, x: integral)
    return (x, 0):t;

  inline proc _cast(type t:chpl_anycomplex, x: chpl_anyreal)
    return (x, 0):t;

  inline proc _cast(type t:chpl_anycomplex, x: chpl_anyimag)
    return (0, _i2r(x)):t;

  inline proc _cast(type t:chpl_anycomplex, x: chpl_anycomplex)
    return (x.re, x.im):t;

  inline proc _cast(type t:chpl_anycomplex, x: enumerated)
    return (x:real, 0):t;

  //
  // casts to imag
  //
  inline proc _cast(type t:chpl_anyimag, x: bool)
    return if x then 1i:t else 0i:t;

  inline proc _cast(type t:chpl_anyimag, x: integral)
    return __primitive("cast", t, x);

  inline proc _cast(type t:chpl_anyimag, x: chpl_anyreal)
    return __primitive("cast", t, x);

  inline proc _cast(type t:chpl_anyimag, x: chpl_anyimag)
    return __primitive("cast", t, x);

  inline proc _cast(type t:chpl_anyimag, x: chpl_anycomplex)
    return let xim = x.im in __primitive("cast", t, xim);

  inline proc _cast(type t:chpl_anyimag, x: enumerated)
    return x:real:imag;

  //
  // casts from complex
  //
  inline proc _cast(type t:chpl_anyreal, x: chpl_anycomplex) {
    var y: t;
    y = x.re:t;
    return y;
  }
  inline proc _cast(type t:integral, x: chpl_anycomplex) {
    var y: t;
    y = x.re:t;
    return y;
  }

  //
  // casts from imag
  //
  inline proc _cast(type t:chpl_anyreal, x: chpl_anyimag)
    return __primitive("cast", t, x);
  inline proc _cast(type t:integral, x: chpl_anyimag)
    return __primitive("cast", t, x);

  inline proc _cast(type t:chpl_anybool, x: chpl_anyimag)
    return if x != 0i then true else false;

  pragma "dont disable remote value forwarding"
  pragma "no copy return"
  pragma "no borrow convert"
  pragma "suppress lvalue error"
  pragma "unsafe"
  inline proc _createFieldDefault(type t, init) {
    if isNonNilableClassType(t) && isNilableClassType(init.type) then
      compilerError("default-initializing a field with a non-nilable type ",
          t:string, " from an instance of nilable ", init.type:string);

    pragma "no auto destroy" var x: t = init;
    return x;
  }

  pragma "dont disable remote value forwarding"
  pragma "no borrow convert"
  pragma "no copy return"
  pragma "unsafe"
  inline proc _createFieldDefault(type t, param init) {
    pragma "no auto destroy" var x: t = init;
    return x;
  }

  pragma "dont disable remote value forwarding"
  pragma "no borrow convert"
  pragma "no copy return"
  pragma "unsafe"
  inline proc _createFieldDefault(type t, init: _nilType) {
    if isNonNilableClassType(t) then
      compilerError("default-initializing a field with a non-nilable type ",
                    t:string, " from nil");

    pragma "no auto destroy" var x: t;
    return x;
  }

  pragma "init copy fn"
  inline proc chpl__initCopy(type t) {
    compilerError("illegal assignment of type to value");
  }

  pragma "compiler generated"
  pragma "last resort"
  pragma "init copy fn"
  inline proc chpl__initCopy(x: _tuple) {
    // body inserted during generic instantiation
  }

  // Catch-all initCopy implementation:
  pragma "compiler generated"
  pragma "last resort"
  pragma "init copy fn"
  pragma "suppress lvalue error"
  inline proc chpl__initCopy(const x) {
    // body adjusted during generic instantiation
    return x;
  }

  pragma "compiler generated"
  pragma "last resort"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x: _tuple) {
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
  inline proc chpl__autoCopy(ir: _iteratorRecord) {
    // body modified during call destructors pass
    return ir;
  }

  pragma "compiler generated"
  pragma "last resort"
  pragma "auto copy fn"
  pragma "suppress lvalue error"
  inline proc chpl__autoCopy(const x) return chpl__initCopy(x);

  pragma "compiler generated"
  pragma "last resort"
  pragma "unalias fn"
  inline proc chpl__unalias(x) {
    pragma "no copy" var ret = x;
    return ret;
  }

  // Returns an array storing the result of the iterator
  pragma "unalias fn"
  inline proc chpl__unalias(x:_iteratorClass) {
    pragma "no copy" var ret = x;
    return ret;
  }

  // Returns an array storing the result of the iterator
  pragma "unalias fn"
  inline proc chpl__unalias(const ref x:_iteratorRecord) {
    pragma "no copy" var ret = x;
    return ret;
  }

  pragma "compiler generated"
  pragma "last resort"
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(x: object) { }

  pragma "compiler generated"
  pragma "last resort"
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(type t)  { }

  pragma "compiler generated"
  pragma "last resort"
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(x: ?t) {
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
    return arg.release();
  }
  inline proc chpl__tounmanaged(arg) where arg:object {
    return arg;
  }*/


  // implements 'delete' statement
  pragma "no borrow convert"
  inline proc chpl__delete(arg) {

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

  proc chpl__delete(arr: []) {
    forall a in arr do
      chpl__delete(a);
  }

  // delete two or more things
  inline proc chpl__delete(arg, args...) {
    chpl__delete(arg);
    for param i in 1..args.size do
      chpl__delete(args(i));
  }

  // c_void_ptr operations
  inline proc =(ref a: c_void_ptr, b: c_void_ptr) { __primitive("=", a, b); }
  inline proc ==(a: c_void_ptr, b: c_void_ptr) {
    return __primitive("ptr_eq", a, b);
  }
  inline proc !=(a: c_void_ptr, b: c_void_ptr) {
    return __primitive("ptr_neq", a, b);
  }

  // Type functions for representing function types
  inline proc func() type { return __primitive("create fn type", void); }
  inline proc func(type rettype) type { return __primitive("create fn type", rettype); }
  inline proc func(type t...?n, type rettype) type { return __primitive("create fn type", (...t), rettype); }

  proc isIterator(ic: _iteratorClass) param return true;
  proc isIterator(ir: _iteratorRecord) param return true;
  proc isIterator(not_an_iterator) param return false;


  /* op= operators
   */
  inline proc +=(ref lhs:int(?w), rhs:int(w)) {
    __primitive("+=", lhs, rhs);
  }
  inline proc +=(ref lhs:uint(?w), rhs:uint(w)) {
    __primitive("+=", lhs, rhs);
  }
  inline proc +=(ref lhs:real(?w), rhs:real(w)) {
    __primitive("+=", lhs, rhs);
  }
  inline proc +=(ref lhs:imag(?w), rhs:imag(w)) {
    __primitive("+=", lhs, rhs);
  }
  inline proc +=(ref lhs, rhs) {
    lhs = lhs + rhs;
  }

  inline proc -=(ref lhs:int(?w), rhs:int(w)) {
    __primitive("-=", lhs, rhs);
  }
  inline proc -=(ref lhs:uint(?w), rhs:uint(w)) {
    __primitive("-=", lhs, rhs);
  }
  inline proc -=(ref lhs:real(?w), rhs:real(w)) {
    __primitive("-=", lhs, rhs);
  }
  inline proc -=(ref lhs:imag(?w), rhs:imag(w)) {
    __primitive("-=", lhs, rhs);
  }
  inline proc -=(ref lhs, rhs) {
    lhs = lhs - rhs;
  }

  inline proc *=(ref lhs:int(?w), rhs:int(w)) {
    __primitive("*=", lhs, rhs);
  }
  inline proc *=(ref lhs:uint(?w), rhs:uint(w)) {
    __primitive("*=", lhs, rhs);
  }
  inline proc *=(ref lhs:real(?w), rhs:real(w)) {
    __primitive("*=", lhs, rhs);
  }
  inline proc *=(ref lhs, rhs) {
    lhs = lhs * rhs;
  }

  inline proc /=(ref lhs:int(?w), rhs:int(w)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to divide by zero");
    __primitive("/=", lhs, rhs);
  }
  inline proc /=(ref lhs:uint(?w), rhs:uint(w)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to divide by zero");
    __primitive("/=", lhs, rhs);
  }
  inline proc /=(ref lhs:real(?w), rhs:real(w)) {
    __primitive("/=", lhs, rhs);
  }
  inline proc /=(ref lhs, rhs) {
    lhs = lhs / rhs;
  }

  inline proc %=(ref lhs:int(?w), rhs:int(w)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to compute a modulus by zero");
    __primitive("%=", lhs, rhs);
  }
  inline proc %=(ref lhs:uint(?w), rhs:uint(w)) {
    if (chpl_checkDivByZero) then
      if rhs == 0 then
        halt("Attempt to compute a modulus by zero");
    __primitive("%=", lhs, rhs);
  }
  inline proc %=(ref lhs:real(?w), rhs:real(w)) {
    __primitive("%=", lhs, rhs);
  }
  inline proc %=(ref lhs, rhs) {
    lhs = lhs % rhs;
  }

  //
  // This overload provides param coercion for cases like uint **= true;
  //
  inline proc **=(ref lhs, rhs) {
    lhs = lhs ** rhs;
  }

  inline proc &=(ref lhs:int(?w), rhs:int(w)) {
    __primitive("&=", lhs, rhs);
  }
  inline proc &=(ref lhs:uint(?w), rhs:uint(w)) {
    __primitive("&=", lhs, rhs);
  }
  inline proc &=(ref lhs, rhs) {
    lhs = lhs & rhs;
  }


  inline proc |=(ref lhs:int(?w), rhs:int(w)) {
    __primitive("|=", lhs, rhs);
  }
  inline proc |=(ref lhs:uint(?w), rhs:uint(w)) {
    __primitive("|=", lhs, rhs);
  }
  inline proc |=(ref lhs, rhs) {
    lhs = lhs | rhs;
  }

  inline proc ^=(ref lhs:int(?w), rhs:int(w)) {
    __primitive("^=", lhs, rhs);
  }
  inline proc ^=(ref lhs:uint(?w), rhs:uint(w)) {
    __primitive("^=", lhs, rhs);
  }
  inline proc ^=(ref lhs, rhs) {
    lhs = lhs ^ rhs;
  }

  inline proc >>=(ref lhs:int(?w), rhs:int(w)) {
    __primitive(">>=", lhs, rhs);
  }
  inline proc >>=(ref lhs:uint(?w), rhs:uint(w)) {
    __primitive(">>=", lhs, rhs);
  }
  inline proc >>=(ref lhs, rhs) {
    lhs = lhs >> rhs;
  }

  inline proc <<=(ref lhs:int(?w), rhs:int(w)) {
    __primitive("<<=", lhs, rhs);
  }
  inline proc <<=(ref lhs:uint(?w), rhs:uint(w)) {
    __primitive("<<=", lhs, rhs);
  }
  inline proc <<=(ref lhs, rhs) {
    lhs = lhs << rhs;
  }

  /* domain += and -= add and remove indices */
  inline proc +=(ref D: domain, idx) { D.add(idx); }
  inline proc -=(ref D: domain, idx) { D.remove(idx); }
  inline proc +=(ref D: domain, param idx) { D.add(idx); }
  inline proc -=(ref D: domain, param idx) { D.remove(idx); }

  /* swap operator */
  inline proc <=>(ref lhs, ref rhs) {
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
  // non-param/non-param -- these are designed to throw an error
  // stating that these combinations are not supported; it's an
  // open topic of discussion whether we should support them and
  // have the return type default to int or uint; and/or whether
  // this error case should be made a lower-priority fallback,
  // permitting the user to define their own overloads (or 'use'
  // a module which does).
  //
  inline proc +(a: uint(64), b: int(64)) { _throwOpError("+"); }
  inline proc +(a: int(64), b: uint(64)) { _throwOpError("+"); }

  //
  // non-param/param and param/non-param cases -- these cases
  // are provided to support operations on runtime uint and
  // param uint combinations.  These are expressed in terms of int/uint
  // functions with one param argument and one non-param argument.
  // Since function disambiguation prefers a 'param' argument over
  // a non-param one, if the 'int' version here is not provided,
  // anEnumVariable + 1 (say) will resolve to the uint + here
  // and that would give the wrong result type (uint rather than int).
  inline proc +(a: uint(64), param b: uint(64)) {
    return __primitive("+", a, b);
  }
  inline proc +(param a: uint(64), b: uint(64)) {
    return __primitive("+", a, b);
  }
  inline proc +(a: int(64), param b: int(64)) {
    return __primitive("+", a, b);
  }
  inline proc +(param a: int(64), b: int(64)) {
    return __primitive("+", a, b);
  }


  // non-param/non-param
  inline proc -(a: uint(64), b: int(64)) { _throwOpError("-"); }
  inline proc -(a: int(64), b: uint(64)) { _throwOpError("-"); }

  // non-param/param and param/non-param
  inline proc -(a: uint(64), param b: uint(64)) {
    return __primitive("-", a, b);
  }
  inline proc -(param a: uint(64), b: uint(64)) {
    return __primitive("-", a, b);
  }
  inline proc -(a: int(64), param b: int(64)) {
    return __primitive("-", a, b);
  }
  inline proc -(param a: int(64), b: int(64)) {
    return __primitive("-", a, b);
  }


  // non-param/non-param
  inline proc *(a: uint(64), b: int(64)) { _throwOpError("*"); }
  inline proc *(a: int(64), b: uint(64)) { _throwOpError("*"); }

  // non-param/param and param/non-param
  inline proc *(a: uint(64), param b: uint(64)) {
    return __primitive("*", a, b);
  }
  inline proc *(param a: uint(64), b: uint(64)) {
    return __primitive("*", a, b);
  }
  inline proc *(a: int(64), param b: int(64)) {
    return __primitive("*", a, b);
  }
  inline proc *(param a: int(64), b: int(64)) {
    return __primitive("*", a, b);
  }


  // non-param/non-param
  inline proc /(a: uint(64), b: int(64)) { _throwOpError("/"); }
  inline proc /(a: int(64), b: uint(64)) { _throwOpError("/"); }

  // non-param/param and param/non-param
  // The int version is only defined so we can catch the divide by zero error
  // at compile time
  inline proc /(a: int(64), param b: int(64)) {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline proc /(a: uint(64), param b: uint(64)) {
    if b == 0 then compilerError("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline proc /(param a: uint(64), b: uint(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }
  inline proc /(param a: int(64), b: int(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to divide by zero");
    return __primitive("/", a, b);
  }


  // non-param/non-param
  inline proc **(a: uint(64), b: int(64)) { _throwOpError("**"); }
  inline proc **(a: int(64), b: uint(64)) { _throwOpError("**"); }

  // non-param/param and param/non-param
  inline proc **(a: uint(64), param b: uint(64)) {
    return __primitive("**", a, b);
  }
  inline proc **(param a: uint(64), b: uint(64)) {
    return __primitive("**", a, b);
  }
  inline proc **(a: int(64), param b: int(64)) {
    return __primitive("**", a, b);
  }
  inline proc **(param a: int(64), b: int(64)) {
    return __primitive("**", a, b);
  }


  // non-param/non-param
  inline proc %(a: uint(64), b: int(64)) { _throwOpError("%"); }
  inline proc %(a: int(64), b: uint(64)) { _throwOpError("%"); }

  // non-param/param and param/non-param
  inline proc %(a: uint(64), param b: uint(64)) {
    if b == 0 then compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  // necessary to support e.g. 10 % myuint
  inline proc %(param a: uint(64), b: uint(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline proc %(a: int(64), param b: int(64)) {
    if b == 0 then compilerError("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }
  inline proc %(param a: int(64), b: int(64)) {
    if (chpl_checkDivByZero) then
      if b == 0 then
        halt("Attempt to compute a modulus by zero");
    return __primitive("%", a, b);
  }


  // non-param/non-param
  inline proc ==(a: uint(64), b: int(64)) {
    //
    // If b's negative, these obviously aren't equal; if it's not
    // negative, it can be cast to an int
    //
    return !(b < 0) && (a == b:uint(64));
  }
  //
  // the dual of the above
  //
  inline proc ==(a: int(64), b: uint(64)) {
    return !(a < 0) && (a:uint(64) == b);
  }

  // non-param/param and param/non-param
  // not necessary since the == versions above
  // work there (and aren't an error)



  // non-param/non-param
  inline proc !=(a: uint(64), b: int(64)) {
    return (b < 0) || (a != b:uint(64));
  }
  inline proc !=(a: int(64), b: uint(64)) {
    return (a < 0) || (a:uint(64) != b);
  }

  // non-param/param and param/non-param
  // not necessary since the == versions above
  // work there (and aren't an error)


  // non-param/non-param
  inline proc >(a: uint(64), b: int(64)) {
    return (b < 0) || (a > b: uint(64));
  }
  inline proc >(a: int(64), b: uint(64)) {
    return !(a < 0) && (a: uint(64) > b);
  }

  // non-param/param and param/non-param
  // non-param/param version not necessary since > above works fine for that
  inline proc >(param a: uint(64), b: uint(64)) param where a == 0 {
    return false;
  }
  inline proc >(param a: uint(64), b: uint(64)) {
    return __primitive(">", a, b);
  }
  inline proc >(param a: int(64), b: int(64)) {
    return __primitive(">", a, b);
  }


  // non-param/non-param
  inline proc <(a: uint(64), b: int(64)) {
    return !(b < 0) && (a < b:uint(64));
  }
  inline proc <(a: int(64), b: uint(64)) {
    return (a < 0) || (a:uint(64) < b);
  }

  // non-param/param and param/non-param
  // param/non-param version not necessary since < above works fine for that
  inline proc <(a: uint(64), param b: uint(64)) param where b == 0 {
    return false;
  }
  inline proc <(a: uint(64), param b: uint(64)) {
    return __primitive("<", a, b);
  }
  inline proc <(a: int(64), param b: int(64)) {
    return __primitive("<", a, b);
  }



  // non-param/non-param
  inline proc >=(a: uint(64), b: int(64)) {
    return (b < 0) || (a >= b: uint(64));
  }
  inline proc >=(a: int(64), b: uint(64)) {
    return !(a < 0) && (a: uint(64) >= b);
  }

  // non-param/param and param/non-param
  inline proc >=(a: uint(64), param b: uint(64)) param where b == 0 {
    return true;
  }
  inline proc >=(a: uint(64), param b: uint(64)) {
    return __primitive(">=", a, b);
  }
  inline proc >=(a: int(64), param b: int(64)) {
    return __primitive(">=", a, b);
  }


  // non-param/non-param
  inline proc <=(a: uint(64), b: int(64)) {
    return !(b < 0) && (a <= b: uint(64));
  }
  inline proc <=(a: int(64), b: uint(64)) {
    return (a < 0) || (a:uint(64) <= b);
  }

  // non-param/param and param/non-param
  inline proc <=(param a: uint(64), b: uint(64)) param where a == 0 {
    return true;
  }
  inline proc <=(param a: uint(64), b: uint(64)) {
    return __primitive("<=", a, b);
  }
  inline proc <=(param a: int(64), b: int(64)) {
    return __primitive("<=", a, b);
  }


  proc isGenericType(type t) param return __primitive("is generic type", t);
  proc isClassType(type t) param return __primitive("is class type", t);
  proc isNilableClassType(type t) param return __primitive("is nilable class type", t);
  proc isNonNilableClassType(type t) param return __primitive("is non nilable class type", t);

  proc isBorrowedOrUnmanagedClassType(type t:unmanaged) param return true;
  proc isBorrowedOrUnmanagedClassType(type t:borrowed) param return true;
  proc isBorrowedOrUnmanagedClassType(type t) param return false;

  // Former support for --legacy-classes, to be removed after 1.21.
  proc chpl_legacyClasses param {
    compilerWarning("'chpl_legacyClasses' is deprecated and will be removed in the next release; it is now always false");
    return false;
  }

  proc isRecordType(type t) param {
    if __primitive("is record type", t) == false then
      return false;

    // some non-record types are implemented via records - exclude those
    else if isDmapType(t)   ||
            isDomainType(t) ||
            isArrayType(t)  ||
            isRangeType(t)  ||
            isTupleType(t)  ||
            isSyncType(t)   ||
            isSingleType(t) ||
            isStringType(t) ||
            isAtomicType(t) then
      return false;

    else
      return true;
  }

  proc isUnionType(type t) param return __primitive("is union type", t);

  proc isAtomicType(type t) param return __primitive("is atomic type", t);

  proc isRefIterType(type t) param return __primitive("is ref iter type", t);

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
  class chpl_ModuleDeinit {
    const moduleName: c_string;          // for debugging; non-null, not owned
    const deinitFun:  c_fn_ptr;          // module deinit function
    const prevModule: unmanaged chpl_ModuleDeinit?; // singly-linked list / LIFO queue
    proc writeThis(ch) throws { 
      try {
      ch.writef("chpl_ModuleDeinit(%s)",createStringWithNewBuffer(moduleName));
      }
      catch e: DecodeError { // let IOError propagate
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
  proc _to_unmanaged(type t) type {
    type rt = __primitive("to unmanaged class", t);
    return rt;
  }
  inline proc _to_unmanaged(arg) {
    var ret = __primitive("to unmanaged class", arg);
    return ret;
  }
  // type constructor for converting to a borrow
  proc _to_borrowed(type t) type {
    type rt = __primitive("to borrowed class", t);
    return rt;
  }
  inline proc _to_borrowed(arg) {
    var ret = __primitive("to borrowed class", arg);
    return ret;
  }
  // changing nilability
  proc _to_nonnil(type t) type {
    type rt = __primitive("to non nilable class", t);
    return rt;
  }
  inline proc _to_nonnil(arg) {
    var ret = __primitive("to non nilable class", arg);
    return ret;
  }
  proc _to_nilable(type t) type {
    type rt = __primitive("to nilable class", t);
    return rt;
  }
  inline proc _to_nilable(arg) {
    var ret = __primitive("to nilable class", arg);
    return ret;
  }


  pragma "no borrow convert"
  inline proc _removed_cast(in x) {
    return x;
  }

  proc chpl_checkCopyInit(lhs, rhs) param { }
}

// ChapelBase.chpl
//

pragma "no use ChapelStandard"
module ChapelBase {
  
  extern proc chpl_config_has_value(name, module_name): bool;
  extern proc chpl_config_get_value(name, module_name): string;

  config param CHPL_HOME: string = "unset";
  if (CHPL_HOME == "unset") {
    compilerWarning("CHPL_HOME not set");
  }

  config param CHPL_HOST_PLATFORM: string = "unset";
  if (CHPL_HOST_PLATFORM == "unset") {
    compilerWarning("CHPL_HOST_PLATFORM not set");
  }

  config param CHPL_TARGET_PLATFORM: string = "unset";
  if (CHPL_TARGET_PLATFORM == "unset") {
    compilerWarning("CHPL_TARGET_PLATFORM not set");
  }

  config param CHPL_HOST_COMPILER: string = "unset";
  if (CHPL_HOST_COMPILER == "unset") {
    compilerWarning("CHPL_HOST_COMPILER not set");
  }

  config param CHPL_TARGET_COMPILER: string = "unset";
  if (CHPL_TARGET_COMPILER == "unset") {
    compilerWarning("CHPL_TARGET_COMPILER not set");
  }

  config param CHPL_LOCALE_MODEL: string = "unset";
  if (CHPL_LOCALE_MODEL == "unset") {
    compilerWarning("CHPL_LOCALE_MODEL not set");
  }

  config param CHPL_ACC_CODEGEN: string = "unset";
  if (CHPL_ACC_CODEGEN == "unset") {
    compilerWarning("CHPL_ACC_CODEGEN not set");
  }

  config param CHPL_COMM: string = "unset";
  if (CHPL_COMM == "unset") {
    compilerWarning("CHPL_COMM not set");
  }

  config param CHPL_COMM_SUBSTRATE: string = "unset";
  if (CHPL_COMM_SUBSTRATE == "unset") {
    compilerWarning("CHPL_COMM_SUBSTRATE not set");
  }

  config param CHPL_GASNET_SEGMENT: string = "unset";
  if (CHPL_GASNET_SEGMENT == "unset") {
    compilerWarning("CHPL_SEGMENT not set");
  }

  config param CHPL_TASKS: string = "unset";
  if (CHPL_TASKS == "unset") {
    compilerWarning("CHPL_TASKS not set");
  }

  config param CHPL_THREADS: string = "unset";
  if (CHPL_THREADS == "unset") {
    compilerWarning("CHPL_THREADS not set");
  }

  config param CHPL_LAUNCHER: string = "unset";
  if (CHPL_LAUNCHER == "unset") {
    compilerWarning("CHPL_LAUNCHER not set");
  }

  config param CHPL_TIMERS: string = "unset";
  if (CHPL_TIMERS == "unset") {
    compilerWarning("CHPL_TIMERS not set");
  }

  config param CHPL_MEM: string = "unset";
  if (CHPL_MEM == "unset") {
    compilerWarning("CHPL_MEM not set");
  }

  config param CHPL_MAKE: string = "unset";
  if (CHPL_MAKE == "unset") {
    compilerWarning("CHPL_MAKE not set");
  }

  config param CHPL_ATOMICS: string = "unset";
  if (CHPL_ATOMICS == "unset") {
    compilerWarning("CHPL_ATOMICS not set");
  }

  config param CHPL_NETWORK_ATOMICS: string = "unset";
  if (CHPL_NETWORK_ATOMICS == "unset") {
    compilerWarning("CHPL_NETWORK_ATOMICS not set");
  }

  config param CHPL_GMP: string = "unset";
  if (CHPL_GMP == "unset") {
    compilerWarning("CHPL_GMP not set");
  }

  config param CHPL_HWLOC: string = "unset";
  if (CHPL_HWLOC == "unset") {
    compilerWarning("CHPL_HWLOC not set");
  }

  config param CHPL_REGEXP: string = "unset";
  if (CHPL_REGEXP == "unset") {
    compilerWarning("CHPL_REGEXP not set");
  }

  config param CHPL_WIDE_POINTERS: string = "unset";
  if (CHPL_WIDE_POINTERS == "unset") {
    compilerWarning("CHPL_WIDE_POINTERS not set");
  }

  config param CHPL_LLVM: string = "unset";
  if (CHPL_LLVM == "unset") {
    compilerWarning("CHPL_LLVM not set");
  }

  config param CHPL_AUX_FILESYS: string = "unset";
  if (CHPL_AUX_FILESYS == "unset") {
    compilerWarning("CHPL_AUX_FILESYS not set");
  }

  config param noRefCount = false;
  
  config param warnMaximalRange = false;    // Warns if integer rollover will cause
                    // the iterator to yield zero times.
  
  inline proc +(s: string, x: numeric)
    return s + x:string;
  
  inline proc +(x: numeric, s: string)
    return x:string + s;
  
  proc +(param s: string, param x: integral) param
    return s + x:string;
  
  proc +(param x: integral, param s: string) param
    return x:string + s;
  
  inline proc +(s: string, x: enumerated)
    return s + x:string;
  
  inline proc +(x: enumerated, s: string)
    return x:string + s;
  
  proc +(param s: string, param x: enumerated) param
    return s + x:string;
  
  proc +(param x: enumerated, param s: string) param
    return x:string + s;
  
  inline proc +(s: string, x: bool)
    return s + x:string;
  
  inline proc +(x: bool, s: string)
    return x:string + s;
  
  proc +(param s: string, param x: bool) param
    return s + x:string;
  
  proc +(param x: bool, param s: string) param
    return x:string + s;
  
  proc _throwOpError(param op: string) {
      compilerError("illegal use of '", op, "' on operands of type uint(64) and signed integer");
  }

  inline proc _throwPVFCError() {
    halt("Pure virtual function called.");
  }

  proc compilerError(param x:string ...?n, param errorDepth:int) {
    __primitive("error", (...x));
  }
  
  proc compilerError(param x:string ...?n) {
    __primitive("error", (...x));
  }
  
  proc compilerWarning(param x:string ...?n, param errorDepth:int) {
    __primitive("warning", (...x));
  }
  
  proc compilerWarning(param x:string ...?n) {
    __primitive("warning", (...x));
  }
  
  // for compilerAssert, as param tuples do not de-tuple into params yet,
  // we handle only up to 5 message args and omit the rest
  
  proc compilerAssert(param test: bool)
  { if !test then compilerError("assert failed"); }
  
  proc compilerAssert(param test: bool, param arg1:integral)
  { if !test then compilerError("assert failed", arg1:int); }
  
  proc compilerAssert(param test: bool, param arg1) where !_isIntegralType(arg1.type)
  { if !test then compilerError("assert failed - ", arg1); }
  
  proc compilerAssert(param test: bool, param arg1, param arg2)
  { if !test then compilerError("assert failed - ", arg1, arg2); }
  
  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3); }
  
  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3, param arg4)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3, arg4); }
  
  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3, param arg4, param arg5)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3, arg4, arg5); }
  
  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3, param arg4, param arg5, param arg6: integral)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3, arg4, arg5, arg6:int); }
  
  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3, param arg4, param arg5, argrest..., param arglast: integral)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3, arg4, arg5, " [...]", arglast:int); }
  
  proc compilerAssert(param test: bool, param arg1, param arg2, param arg3, param arg4, param arg5, argrest...)
  { if !test then compilerError("assert failed - ", arg1, arg2, arg3, arg4, arg5, " [...]"); }

  proc typeToString(type t) param {
    return __primitive("typeToString", t);
  }
  
  proc typeToString(x) param {
    compilerError("typeToString()'s argument must be a type, not a value");
  }
  
  enum iterKind {leader, follower};
  
  //
  // assignment on primitive types
  //
  inline proc =(a: bool, b: bool) return b;
  inline proc =(a: bool(?w), b: bool) return b:a.type;
  inline proc =(a: int(?w), b: int(w)) return b;
  inline proc =(a: uint(?w), b: uint(w)) return b;
  inline proc =(a: real(?w), b: real(w)) return b;
  inline proc =(a: imag(?w), b: imag(w)) return b;
  inline proc =(a: complex(?w), b: complex(w)) return b;
  // This implies that the *representation* of strings is shared.
  // If strings are reimplemented as classes or records, a less trivial
  // implementation for assignment will become necessary.
  inline proc =(a: string, b: string) return b;
  // Because resolution prefers user-defined versions to ones marked as "compiler
  // generated", it is desirable to add that flag to this default version.
  // In that way, a user-supplied version of assignment will override this one.
  pragma "compiler generated"
  inline proc =(a, b) return b;
  
  //
  // equality comparison on primitive types
  //
  inline proc ==(a: bool, b: bool) return __primitive("==", a, b);
  inline proc ==(a: int(?w), b: int(w)) return __primitive("==", a, b);
  inline proc ==(a: uint(?w), b: uint(w)) return __primitive("==", a, b);
  inline proc ==(a: real(?w), b: real(w)) return __primitive("==", a, b);
  inline proc ==(a: imag(?w), b: imag(w)) return __primitive("==", a, b);
  inline proc ==(a: complex(?w), b: complex(w)) return a.re == b.re && a.im == b.im;
  inline proc ==(a: string, b: string) return (__primitive("chpl_string_compare", a, b) == 0);
  inline proc ==(a: object, b: object) return __primitive("ptr_eq", a, b);
  
  inline proc !=(a: bool, b: bool) return __primitive("!=", a, b);
  inline proc !=(a: int(?w), b: int(w)) return __primitive("!=", a, b);
  inline proc !=(a: uint(?w), b: uint(w)) return __primitive("!=", a, b);
  inline proc !=(a: real(?w), b: real(w)) return __primitive("!=", a, b);
  inline proc !=(a: imag(?w), b: imag(w)) return __primitive("!=", a, b);
  inline proc !=(a: complex(?w), b: complex(w)) return a.re != b.re || a.im != b.im;
  inline proc !=(a: string, b: string) return (__primitive("chpl_string_compare", a, b) != 0);
  inline proc !=(a: object, b: object) return __primitive("ptr_neq", a, b);
  
  inline proc ==(param a: bool, param b: bool) param return __primitive("==", a, b);
  inline proc ==(param a: int(?w), param b: int(w)) param return __primitive("==", a, b);
  inline proc ==(param a: uint(?w), param b: uint(w)) param return __primitive("==", a, b);
  inline proc ==(param a: enumerated, param b: enumerated) param return __primitive("==", a, b);
  inline proc ==(param a: string, param b: string) param return __primitive("chpl_string_compare", a, b) == 0;
  
  inline proc !=(param a: bool, param b: bool) param return __primitive("!=", a, b);
  inline proc !=(param a: int(?w), param b: int(w)) param return __primitive("!=", a, b);
  inline proc !=(param a: uint(?w), param b: uint(w)) param return __primitive("!=", a, b);
  inline proc !=(param a: enumerated, param b: enumerated) param return __primitive("!=", a, b);
  inline proc !=(param a: string, param b: string) param return __primitive("chpl_string_compare", a, b) != 0;
  
  //
  // ordered comparison on primitive types
  //
  inline proc <=(a: int(?w), b: int(w)) return __primitive("<=", a, b);
  inline proc <=(a: uint(?w), b: uint(w)) return __primitive("<=", a, b);
  inline proc <=(a: real(?w), b: real(w)) return __primitive("<=", a, b);
  inline proc <=(a: imag(?w), b: imag(w)) return __primitive("<=", a, b);
  inline proc <=(a: string, b: string) return (__primitive("chpl_string_compare", a, b) <= 0);
  
  inline proc >=(a: int(?w), b: int(w)) return __primitive(">=", a, b);
  inline proc >=(a: uint(?w), b: uint(w)) return __primitive(">=", a, b);
  inline proc >=(a: real(?w), b: real(w)) return __primitive(">=", a, b);
  inline proc >=(a: imag(?w), b: imag(w)) return __primitive(">=", a, b);
  inline proc >=(a: string, b: string) return (__primitive("chpl_string_compare", a, b) >= 0);
  
  inline proc <(a: int(?w), b: int(w)) return __primitive("<", a, b);
  inline proc <(a: uint(?w), b: uint(w)) return __primitive("<", a, b);
  inline proc <(a: real(?w), b: real(w)) return __primitive("<", a, b);
  inline proc <(a: imag(?w), b: imag(w)) return __primitive("<", a, b);
  inline proc <(a: string, b: string) return (__primitive("chpl_string_compare", a, b) < 0);
  
  inline proc >(a: int(?w), b: int(w)) return __primitive(">", a, b);
  inline proc >(a: uint(?w), b: uint(w)) return __primitive(">", a, b);
  inline proc >(a: real(?w), b: real(w)) return __primitive(">", a, b);
  inline proc >(a: imag(?w), b: imag(w)) return __primitive(">", a, b);
  inline proc >(a: string, b: string) return (__primitive("chpl_string_compare", a, b) > 0);
  
  inline proc <=(param a: int(?w), param b: int(w)) param return __primitive("<=", a, b);
  inline proc <=(param a: uint(?w), param b: uint(w)) param return __primitive("<=", a, b);
  inline proc <=(param a: enumerated, param b: enumerated) param return __primitive("<=", a, b);
  inline proc <=(param a: string, param b: string) param return __primitive("chpl_string_compare", a, b) <= 0;
  
  inline proc >=(param a: int(?w), param b: int(w)) param return __primitive(">=", a, b);
  inline proc >=(param a: uint(?w), param b: uint(w)) param return __primitive(">=", a, b);
  inline proc >=(param a: enumerated, param b: enumerated) param return __primitive(">=", a, b);
  inline proc >=(param a: string, param b: string) param return __primitive("chpl_string_compare", a, b) >= 0;
  
  inline proc <(param a: int(?w), param b: int(w)) param return __primitive("<", a, b);
  inline proc <(param a: uint(?w), param b: uint(w)) param return __primitive("<", a, b);
  inline proc <(param a: enumerated, param b: enumerated) param return __primitive("<", a, b);
  inline proc <(param a: string, param b: string) param return __primitive("chpl_string_compare", a, b) < 0;
  
  inline proc >(param a: int(?w), param b: int(w)) param return __primitive(">", a, b);
  inline proc >(param a: uint(?w), param b: uint(w)) param return __primitive(">", a, b);
  inline proc >(param a: enumerated, param b: enumerated) param return __primitive(">", a, b);
  inline proc >(param a: string, param b: string) param return (__primitive("chpl_string_compare", a, b) > 0);
  
  //
  // unary + and - on primitive types
  //
  inline proc +(a: int(?w)) return a;
  inline proc +(a: uint(?w)) return a;
  inline proc +(a: real(?w)) return a;
  inline proc +(a: imag(?w)) return a;
  inline proc +(a: complex(?w)) return a;
  
  inline proc -(a: int(?w)) return __primitive("u-", a);
  inline proc -(a: uint(64)) { compilerError("illegal use of '-' on operand of type ", typeToString(a.type)); }
  inline proc -(a: real(?w)) return __primitive("u-", a);
  inline proc -(a: imag(?w)) return __primitive("u-", a);
  inline proc -(a: complex(?w)) return (-a.re, -a.im):complex(w);
  
  inline proc +(param a: int(?w)) param return a;
  inline proc +(param a: uint(?w)) param return a;
  
  inline proc -(param a: int(?w)) param return __primitive("u-", a);
  inline proc -(param a: uint(?w)) param {
    if (a:int(w) < 0) then
      compilerError("illegal use of '-' on operand of type ", typeToString(a.type));
    else
      return -(a:int(w));
  }
  
  //
  // binary + and - on primitive types for runtime values
  //
  inline proc +(a: int(?w), b: int(w)) return __primitive("+", a, b);
  inline proc +(a: uint(?w), b: uint(w)) return __primitive("+", a, b);
  inline proc +(a: real(?w), b: real(w)) return __primitive("+", a, b);
  inline proc +(a: imag(?w), b: imag(w)) return __primitive("+", a, b);
  inline proc +(a: complex(?w), b: complex(w)) return (a.re+b.re, a.im+b.im):complex(w);
  inline proc +(a: string, b: string) return __primitive("string_concat", a, b);
  
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
  inline proc -(a: complex(?w), b: complex(w)) return (a.re-b.re, a.im-b.im):complex(w);
  
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
  inline proc +(param a: string, param b: string) param return __primitive("string_concat", a, b);
  
  inline proc -(param a: int(?w), param b: int(w)) param return __primitive("-", a, b);
  inline proc -(param a: uint(?w), param b: uint(w)) param return __primitive("-", a, b);
  
  //
  // * and / on primitive types
  //
  inline proc *(a: int(?w), b: int(w)) return __primitive("*", a, b);
  inline proc *(a: uint(?w), b: uint(w)) return __primitive("*", a, b);
  inline proc *(a: real(?w), b: real(w)) return __primitive("*", a, b);
  inline proc *(a: imag(?w), b: imag(w)) return _i2r(__primitive("*", -a, b));
  inline proc *(a: complex(?w), b: complex(w)) return (a.re*b.re-a.im*b.im, a.im*b.re+a.re*b.im):complex(w);
  
  inline proc *(a: real(?w), b: imag(w)) return _r2i(a*_i2r(b));
  inline proc *(a: imag(?w), b: real(w)) return _r2i(_i2r(a)*b);
  inline proc *(a: real(?w), b: complex(w*2)) return (a*b.re, a*b.im):complex(w*2);
  inline proc *(a: complex(?w), b: real(w/2)) return (a.re*b, a.im*b):complex(w);
  inline proc *(a: imag(?w), b: complex(w*2)) return (-_i2r(a)*b.im, _i2r(a)*b.re):complex(w*2);
  inline proc *(a: complex(?w), b: imag(w/2)) return (-a.im*_i2r(b), a.re*_i2r(b)):complex(w);
  
  inline proc /(a: int(?w), b: int(w)) return __primitive("/", a, b);
  inline proc /(a: uint(?w), b: uint(w)) return __primitive("/", a, b);
  inline proc /(a: real(?w), b: real(w)) return __primitive("/", a, b);
  inline proc /(a: imag(?w), b: imag(w)) return _i2r(__primitive("/", a, b));
  inline proc /(a: complex(?w), b: complex(w))
    return let d = b.re*b.re+b.im*b.im in
    ((a.re*b.re+a.im*b.im)/d, (a.im*b.re-a.re*b.im)/d):complex(w);
  
  inline proc /(a: real(?w), b: imag(w)) return _r2i(-a/_i2r(b));
  inline proc /(a: imag(?w), b: real(w)) return _r2i(_i2r(a)/b);
  inline proc /(a: real(?w), b: complex(w*2))
    return let d = b.re*b.re+b.im*b.im in
    (a*b.re/d, -a*b.im/d):complex(w*2);
  inline proc /(a: complex(?w), b: real(w/2))
  return (a.re/b, a.im/b):complex(w);
  inline proc /(a: imag(?w), b: complex(w*2))
    return let d = b.re*b.re+b.im*b.im in
    (_i2r(a)*b.im/d, _i2r(a)*b.re/d):complex(w*2);
  inline proc /(a: complex(?w), b: imag(w/2))
    return let d = _i2r(b)*_i2r(b) in
    (a.im/_i2r(b), -a.re/_i2r(b)):complex(w);
  
  inline proc *(param a: int(?w), param b: int(w)) param return __primitive("*", a, b);
  inline proc *(param a: uint(?w), param b: uint(w)) param return __primitive("*", a, b);
  
  inline proc /(param a: int(?w), param b: int(w)) param return __primitive("/", a, b);
  inline proc /(param a: uint(?w), param b: uint(w)) param return __primitive("/", a, b);
  
  //
  // % on primitive types
  //
  inline proc %(a: int(?w), b: int(w)) return __primitive("%", a, b);
  inline proc %(a: uint(?w), b: uint(w)) return __primitive("%", a, b);
  
  inline proc %(param a: int(?w), param b: int(w)) param return __primitive("%", a, b);
  inline proc %(param a: uint(?w), param b: uint(w)) param return __primitive("%", a, b);
  
  //
  // ** on primitive types
  //
  
  inline proc _intExpHelp(a: integral, b) where a.type == b.type {
    if b < 0 then
      if a == 0 then
        halt("cannot compute ", a, " ** ", b);
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
  
  proc _canOptimizeExp(param b: integral) param return b >= 0 && b <= 8 && b != 7;
  
  inline proc **(a: int(?w), param b: integral) where _canOptimizeExp(b) return _expHelp(a, b);
  inline proc **(a: uint(?w), param b: integral) where _canOptimizeExp(b) return _expHelp(a, b);
  inline proc **(a: real(?w), param b: integral) where _canOptimizeExp(b) return _expHelp(a, b);
  
  //
  // logical operations on primitive types
  //
  inline proc !(a: bool) return __primitive("!", a);
  
  inline proc isTrue(a: bool) return a;
  inline proc isTrue(param a: bool) param return a;
  
  proc isTrue(a: integral) { compilerError("short-circuiting logical operators not supported on integers"); }
  
  inline proc !(param a: bool) param return __primitive("!", a);
  
  //
  // bitwise operations on primitive types
  //
  inline proc ~(a: bool) return __primitive("u~", a);
  inline proc ~(a: int(?w)) return __primitive("u~", a);
  inline proc ~(a: uint(?w)) return __primitive("u~", a);
  
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
  
  inline proc ~(param a: bool) param return __primitive("u~", a);
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
  inline proc <<(a: int(?w), b: integral) return __primitive("<<", a, b);
  inline proc <<(a: uint(?w), b: integral) return __primitive("<<", a, b);
  
  inline proc >>(a: int(?w), b: integral) return __primitive(">>", a, b);
  inline proc >>(a: uint(?w), b: integral) return __primitive(">>", a, b);
  
  inline proc <<(param a: int(?w), param b: integral) param return __primitive("<<", a, b);
  inline proc <<(param a: uint(?w), param b: integral) param return __primitive("<<", a, b);
  
  inline proc >>(param a: int(?w), param b: integral) param return __primitive(">>", a, b);
  inline proc >>(param a: uint(?w), param b: integral) param return __primitive(">>", a, b);
  
  //
  // These functions are to handle symbols at statement level such as
  // the second statement here: var a: sync int = 1; a;
  //
  inline proc _statementLevelSymbol(a) { return a; }
  inline proc _statementLevelSymbol(param a) param { return a; }
  inline proc _statementLevelSymbol(type a) type { return a; }
  
  //
  // If an iterator is called without capturing the result, iterate over it
  // to ensure any side effects it has will happen.
  //
  inline proc _statementLevelSymbol(ir: _iteratorRecord) {
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
  inline proc _cond_test(x: object) return x != nil;
  inline proc _cond_test(x: bool) return x;
  inline proc _cond_test(x: integral) return x != 0:x.type;
  
  inline proc _cond_test(param x: bool) param return x;
  inline proc _cond_test(param x: integral) param return x != 0:x.type;
  
  inline proc _cond_test(x) {
    compilerError("type '", typeToString(x.type), "' used in if or while condition");
  }
  
  inline proc _cond_test(x: _iteratorRecord) {
    compilerError("iterator or promoted expression ", typeToString(x.type), " used in if or while condition");
  }
  
  proc _cond_invalid(x: object) param return false;
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
  //  bug?  in setters, parameterize real argument over complex bit width
  //
  pragma "ref this"
  inline proc chpl_anycomplex.re var return __primitive("complex_get_real", this);
  pragma "ref this"
  inline proc chpl_anycomplex.im var return __primitive("complex_get_imag", this);
  
  //
  // helper functions
  //
  inline proc _i2r(a: imag(?w)) return __primitive("cast", real(w), a);
  inline proc _r2i(a: real(?w)) return __primitive("cast", imag(w), a);
  
  //
  // primitive string functions and methods
  //
  inline proc ascii(a: string) return __primitive("ascii", a);
  inline proc string.length return __primitive("string_length", this);
  inline proc string.size return this.length;
  inline proc string.substring(i: int) return __primitive("string_index", this, i);
  inline proc _string_contains(a: string, b: string) return __primitive("string_contains", a, b);
  
  inline proc ascii(param a: string) param return __primitive("ascii", a);
  inline proc param string.length param return __primitive("string_length", this);
  inline proc _string_contains(param a: string, param b: string) param return __primitive("string_contains", a, b);
  
  //
  // min and max
  //
  inline proc min(x, y) return if x < y then x else y;
  inline proc max(x, y) return if x > y then x else y;
  inline proc min(x, y, z...?k) return min(min(x, y), (...z));
  inline proc max(x, y, z...?k) return max(max(x, y), (...z));
  
  //
  // More primitive funs
  //
  inline proc exit(status: int) {
    __primitive("chpl_exit_any", status);
  }
  
  proc init_elts(x, s, type t) {
    for i in 1..s {
      //
      // Q: why is the following declaration of 'y' in the loop?
      //
      // A: so that if the element type is something like an array,
      // the element can 'steal' the array rather than copying it.
      // One effect of having it in the loop is that the reference
      // count for an array element's domain gets bumped once per
      // element.  Is this good, bad, necessary?  Unclear.
      //
      pragma "no auto destroy" var y: t;  
      __primitive("array_set_first", x, i-1, y);
    }
  }
  
  // Make sure that the compiler constructs the type _ref(t).
  pragma "no codegen"
  proc _ensure_reference_type(type t)
  {
    // type t where we have _ddata(t) needs to also have
    // a reference type created. So here we just add
    // some otherwise useless code to do that.
    // This code could be removed if we had some other way
    // making sure the reference type is created.
    pragma "no codegen"
    proc setbyref(inout zz:t) { }
    var unused:t;
    setbyref(unused);
  }
  
  // dynamic data block class
  // (note that c_ptr(type) is similar, but local only,
  //  and defined in SysBasic.chpl)
  pragma "data class"
  pragma "no object"
  pragma "no default functions"
  class _ddata {
    type eltType;
    /*
       If we had a way to do 'static' routines, this
       could stay here, but since we don't at the moment,
       we've wired the modules to call _ddata_free().
  
    proc ~_ddata() {
      __primitive("array_free", this);
    }
  
     If we had a way to do 'static' routines, this
       could stay here, but since we don't at the moment,
       we've wired the modules to call _ddata_allocate().
    inline proc init(size: integral) {
      __primitive("array_alloc", this, eltType, size);
      init_elts(this, size, eltType);
    }*/
    inline proc this(i: integral) var {
      return __primitive("array_get", this, i);
    }
  }
  
  
  inline proc _cast(type t, x) where t:_ddata && x:_nilType {
    return __primitive("cast", t, x);
  }

  inline proc _ddata_shift(type eltType, data: _ddata(eltType), shift: integral) {
    var ret: _ddata(eltType);
     __primitive("shift_base_pointer", ret, data, shift);
    return ret;
  }

  inline proc _ddata_allocate(type eltType, size: integral) {
    var ret:_ddata(eltType);
    __primitive("array_alloc", ret, eltType, size);
    init_elts(ret, size, eltType);
    return ret;
  }
  
  inline proc _ddata_free(data: _ddata) {
    __primitive("array_free", data);
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
  
  pragma "no default functions"
  class _EndCount {
    var i: atomic int,
        taskList: _task_list = _nullTaskList;
  }
  
  // This function is called once by the initiating task.  No on
  // statement needed, because the task should be running on the same
  // locale as the sync/cofall/cobegin was initiated on and thus the
  // same locale on which the object is allocated.
  pragma "dont disable remote value forwarding"
  inline proc _endCountAlloc() return new _EndCount();
  
  // This function is called once by the initiating task.  As above, no
  // on statement needed.
  pragma "dont disable remote value forwarding"
  inline proc _endCountFree(e: _EndCount) {
    delete e;
  }
  
  // This function is called by the initiating task once for each new
  // task *before* any of the tasks are started.  As above, no on
  // statement needed.
  pragma "dont disable remote value forwarding"
  proc _upEndCount(e: _EndCount) {
    e.i.add(1);
  }
  
  // This function is called once by each newly initiated task.  No on
  // statement is needed because the call to sub() will do a remote
  // fork (on) if needed.
  pragma "dont disable remote value forwarding"
  proc _downEndCount(e: _EndCount) {
    e.i.sub(1);
  }
  
  // This function is called once by the initiating task.  As above, no
  // on statement needed.
  pragma "dont disable remote value forwarding"
  proc _waitEndCount(e: _EndCount) {
    // See if we can help with any of the started tasks
    __primitive("execute tasks in list", e.taskList);
  
    // Wait for all tasks to finish
    e.i.waitFor(0);
  
    // It is now safe to free the task list, because we know that all the
    // tasks have been completed.  We could free this list when all the
    // tasks have been started, but this seems cleaner.  The alternative
    // would be for the tasking layer to free the elements of the list
    // when when they are no longer needed, but then every tasking layer
    // would have to implement the free, and it's not clear that it
    // would be of any benefit.  Another option would be for the
    // starting task to free its own list element.
    __primitive("free task list", e.taskList);
  }
  
  proc _upEndCount() {
    var e = __primitive("get end count");
    _upEndCount(e);
  }
  
  proc _downEndCount() {
    var e = __primitive("get end count");
    _downEndCount(e);
  }
  
  proc _waitEndCount() {
    var e = __primitive("get end count");
    _waitEndCount(e);
  }
  
  pragma "command line setting"
  proc _command_line_cast(param s: string, type t, x) return _cast(t, x);
  
  inline proc _cast(type t, x: bool) where _isPrimitiveType(t)
    return __primitive("cast", t, x);
  
  inline proc _cast(type t, x: bool(?w)) where _isPrimitiveType(t)
    return __primitive("cast", t, x);
  
  inline proc _cast(type t, x: int(?w)) where _isPrimitiveType(t)
    return __primitive("cast", t, x);
  
  inline proc _cast(type t, x: uint(?w)) where _isPrimitiveType(t)
    return __primitive("cast", t, x);
  
  inline proc _cast(type t, x: real(?w)) where _isPrimitiveType(t)
    return __primitive("cast", t, x);
  
  inline proc _cast(type t, x: string) where _isPrimitiveType(t)
    return __primitive("cast", t, x);
  
  inline proc _cast(type t, x: enumerated) where _isPrimitiveType(t) && t != string
    return __primitive("cast", t, x);
  
  inline proc _cast(type t, x) where t:object && x:t
    return __primitive("cast", t, x);
  
  inline proc _cast(type t, x) where t:object && x:_nilType
    return __primitive("cast", t, x);
  
  inline proc _cast(type t, x) where x:object && t:x && (x.type != t)
    return if x != nil then __primitive("dynamic_cast", t, x) else __primitive("cast", t, nil);
  
  inline proc _cast(type t, x:_nilType) where t == _nilType
    return nil;
  
  //
  // casts to complex
  //
  inline proc _cast(type t, x: bool) where _isComplexType(t)
    return (x, 0):t;
  
  inline proc _cast(type t, x: int(?w)) where _isComplexType(t)
    return (x, 0):t;
  
  inline proc _cast(type t, x: uint(?w)) where _isComplexType(t)
    return (x, 0):t;
  
  inline proc _cast(type t, x: real(?w)) where _isComplexType(t)
    return (x, 0):t;
  
  inline proc _cast(type t, x: imag(?w)) where _isComplexType(t)
    return (0, _i2r(x)):t;
  
  inline proc _cast(type t, x: complex(?w)) where _isComplexType(t)
    return (x.re, x.im):t;
  
  inline proc _cast(type t, x: string) where _isComplexType(t)
    return __primitive("cast", t, x);
  
  //
  // casts to imag
  //
  inline proc _cast(type t, x: bool) where _isImagType(t)
    return if x then 1i:t else 0i:t;
  
  inline proc _cast(type t, x: int(?w)) where _isImagType(t)
    return 0i:t;
  
  inline proc _cast(type t, x: uint(?w)) where _isImagType(t)
    return 0i:t;
  
  inline proc _cast(type t, x: real(?w)) where _isImagType(t)
    return 0i:t;
  
  inline proc _cast(type t, x: imag(?w)) where _isImagType(t)
    return __primitive("cast", t, x);
  
  inline proc _cast(type t, x: complex(?w)) where _isImagType(t)
    return let xim = x.im in __primitive("cast", t, xim);
  
  inline proc _cast(type t, x: string) where _isImagType(t)
    return __primitive("cast", t, x);
  
  //
  // casts from complex
  //
  inline proc _cast(type t, x: complex(?w)) where t == string {
    if isnan(x.re) || isnan(x.im) then
      return "nan";
    var re = x.re:string, op = " + ", im = x.im:string;
    if x.im < 0 {
      im = (-x.im):string;
      op = " - ";
    } else if im == "-0.0" {
      im = "0.0";
      op = " - ";
    }
    return re + op + im + "i";
  }
  
  inline proc _cast(type t, x: complex(?w)) where _isRealType(t) || _isIntegralType(t) {
    var y: t;
    y = x.re:t;
    return y;
  }
  
  //
  // casts from imag
  //
  inline proc _cast(type t, x: imag(?w)) where t == string
    return __primitive("cast", t, x);
  
  inline proc _cast(type t, x: imag(?w)) where _isRealType(t) || _isIntegralType(t)
    return 0:t;
  
  inline proc _cast(type t, x: imag(?w)) where _isBooleanType(t)
    return if x != 0i then true else false;
  
  inline proc chpl__typeAliasInit(type t) type return t;
  inline proc chpl__typeAliasInit(v) {
    compilerError("illegal assignment of value to type");
  }
  
  pragma "dont disable remote value forwarding"
  inline proc _createFieldDefault(type t, init) {
    pragma "no auto destroy" var x: t;
    x = init;
    return x;
  }
  
  pragma "dont disable remote value forwarding"
  inline proc _createFieldDefault(type t, param init) {
    pragma "no auto destroy" var x: t;
    x = init;
    return x;
  }
  
  pragma "dont disable remote value forwarding"
  inline proc _createFieldDefault(type t, init: _nilType) {
    pragma "no auto destroy" var x: t;
    return x;
  }
  
  pragma "compiler generated"
  pragma "init copy fn"
  inline proc chpl__initCopy(a) {
    // Currently, string representations are shared.
    // (See note on proc =(a:string, b:string) above.)
      return a;
  }
  
  pragma "init copy fn"
  inline proc chpl__initCopy(type t) {
    compilerError("illegal assignment of type to value");
  }
  
  pragma "ref"
  pragma "init copy fn"
  inline proc chpl__initCopy(r: _ref) return chpl__initCopy(__primitive("deref", r));
  
  pragma "init copy fn"
  inline proc chpl__initCopy(x: _tuple) { 
    // body inserted during generic instantiation
  }
  
  pragma "dont disable remote value forwarding"
  pragma "removable auto copy"
  pragma "donor fn"
  pragma "auto copy fn" proc chpl__autoCopy(x: _distribution) {
    if !noRefCount then x._value.incRefCount();
    return x;
  }
  
  pragma "dont disable remote value forwarding"
  pragma "removable auto copy"
  pragma "donor fn"
  pragma "auto copy fn"  proc chpl__autoCopy(x: domain) {
    if !noRefCount then x._value.incRefCount();
    return x;
  }
  
  pragma "dont disable remote value forwarding"
  pragma "removable auto copy"
  pragma "donor fn"
  pragma "auto copy fn" proc chpl__autoCopy(x: []) {
    if !noRefCount then x._value.incRefCount();
    return x;
  }
  

  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x: _tuple) {
    // body inserted during generic instantiation
  }
  
  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(ir: _iteratorRecord) {
    // body modified during call destructors pass
    return ir;
  }
  
  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x) return chpl__initCopy(x);
  
  pragma "ref" 
  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(r: _ref) var return r;
  
  inline proc chpl__maybeAutoDestroyed(x: numeric) param return false;
  inline proc chpl__maybeAutoDestroyed(x: enumerated) param return false;
  inline proc chpl__maybeAutoDestroyed(x: object) param return false;
  inline proc chpl__maybeAutoDestroyed(x) param return true;

  pragma "auto destroy fn" inline proc chpl__autoDestroy(x: object) { }
  pragma "auto destroy fn" inline proc chpl__autoDestroy(type t)  { }
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(x: ?t) {
    __primitive("call destructor", x);
  }
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(ir: _iteratorRecord) {
    // body inserted during call destructors pass
  }
  pragma "dont disable remote value forwarding"
  pragma "removable auto destroy"
  pragma "auto destroy fn" proc chpl__autoDestroy(x: _distribution) {
    __primitive("call destructor", x);
  }
  pragma "dont disable remote value forwarding"
  pragma "removable auto destroy"
  pragma "auto destroy fn" proc chpl__autoDestroy(x: domain) {
    __primitive("call destructor", x);
  }
  pragma "dont disable remote value forwarding"
  pragma "removable auto destroy"
  pragma "auto destroy fn" proc chpl__autoDestroy(x: []) {
    __primitive("call destructor", x);
  }
  
  // Type functions for representing function types
  inline proc func() type { return __primitive("create fn type", void); }
  inline proc func(type rettype) type { return __primitive("create fn type", rettype); }
  inline proc func(type t...?n, type rettype) type { return __primitive("create fn type", (...t), rettype); }
  
  //
  // BLC: The inout is used here not because it is necessary, but in
  // order to ensure that the reference to the variable is passed in
  // rather than the value.  A pragma "ref" almost results in similar
  // behavior, but causes the expression being passed into this to be
  // stored as an rvalue in the generated code rather than an lvalue.
  // The correct implementation of this routine requires this function
  // to be special-cased in functionResolution.cpp such that the inout
  // does not actually result in temps.
  //
  proc chpldev_refToString(inout arg) {
  
    //
    // print out the address of class references as well
    //
    proc chpldev_classToString(x: object)
      return " (class = " + __primitive("ref to string", x) + ")";
    proc chpldev_classToString(x) return "";
  
    return __primitive("ref to string", arg) + chpldev_classToString(arg);
  }
  
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
    __primitive("/=", lhs, rhs);
  }
  inline proc /=(ref lhs:uint(?w), rhs:uint(w)) {
    __primitive("/=", lhs, rhs);
  }
  inline proc /=(ref lhs:real(?w), rhs:real(w)) {
    __primitive("/=", lhs, rhs);
  }
  inline proc /=(ref lhs, rhs) {
    lhs = lhs / rhs;
  }

  inline proc %=(ref lhs:int(?w), rhs:int(w)) {
    __primitive("%=", lhs, rhs);
  }
  inline proc %=(ref lhs:uint(?w), rhs:uint(w)) {
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
  inline proc +=(D: domain, idx) { D.add(idx); }
  inline proc -=(D: domain, idx) { D.remove(idx); }
  inline proc +=(D: domain, param idx) { D.add(idx); }
  inline proc -=(D: domain, param idx) { D.remove(idx); }
  
  /* swap operator */
  inline proc <=>(ref lhs, ref rhs) {
    const tmp = lhs;
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
  // param uint combinations.  The param/non-param int cases
  // don't need to be explicitly given because they will default
  // to the normal int/int cases above.  If these overloads are
  // not provided, these functions will dispatch to the error
  // cases just above, which is inconsistent (i.e., one should
  // be able to add "1" to a uint variable given that "1" is a
  // legal uint param).
  //
  inline proc +(a: uint(64), param b: uint(64)) {
    return __primitive("+", a, b);
  }
  inline proc +(param a: uint(64), b: uint(64)) {
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
  
  
  // non-param/non-param
  inline proc /(a: uint(64), b: int(64)) { _throwOpError("/"); }
  inline proc /(a: int(64), b: uint(64)) { _throwOpError("/"); }
  
  // non-param/param and param/non-param
  inline proc /(a: uint(64), param b: uint(64)) {
    return __primitive("/", a, b);
  }
  inline proc /(param a: uint(64), b: uint(64)) {
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
  
  
  // non-param/non-param
  inline proc %(a: uint(64), b: int(64)) { _throwOpError("%"); }
  inline proc %(a: int(64), b: uint(64)) { _throwOpError("%"); }
  
  // non-param/param and param/non-param
  inline proc %(a: uint(64), param b: uint(64)) {
    return __primitive("%", a, b);
  }
  inline proc %(param a: uint(64), b: uint(64)) {
    return __primitive("%", a, b);
  }
  
  
  // non-param/non-param
  inline proc ==(a: uint(64), b: int(64)) { _throwOpError("=="); }
  inline proc ==(a: int(64), b: uint(64)) { _throwOpError("=="); }
  
  // non-param/param and param/non-param
  inline proc ==(a: uint(64), param b: uint(64)) {
    return __primitive("==", a, b);
  }
  inline proc ==(param a: uint(64), b: uint(64)) {
    return __primitive("==", a, b);
  }
  
  
  // non-param/non-param
  inline proc !=(a: uint(64), b: int(64)) { _throwOpError("!="); }
  inline proc !=(a: int(64), b: uint(64)) { _throwOpError("!="); }
  
  // non-param/param and param/non-param
  inline proc !=(a: uint(64), param b: uint(64)) {
    return __primitive("!=", a, b);
  }
  inline proc !=(param a: uint(64), b: uint(64)) {
    return __primitive("!=", a, b);
  }
  
  
  // non-param/non-param
  inline proc >(a: uint(64), b: int(64)) { _throwOpError(">"); }
  inline proc >(a: int(64), b: uint(64)) { _throwOpError(">"); }
  
  // non-param/param and param/non-param
  inline proc >(a: uint(64), param b: uint(64)) {
    return __primitive(">", a, b);
  }
  inline proc >(param a: uint(64), b: uint(64)) {
    if a == 0 then return false; else return __primitive(">", a, b);
  }
  
  // non-param/non-param
  inline proc <(a: uint(64), b: int(64)) { _throwOpError("<"); }
  inline proc <(a: int(64), b: uint(64)) { _throwOpError("<"); }
  
  // non-param/param and param/non-param
  inline proc <(a: uint(64), param b: uint(64)) {
    if b == 0 then return false; else return __primitive("<", a, b);
  }
  inline proc <(param a: uint(64), b: uint(64)) {
    return __primitive("<", a, b);
  }
  
  
  // non-param/non-param
  inline proc >=(a: uint(64), b: int(64)) { _throwOpError(">="); }
  inline proc >=(a: int(64), b: uint(64)) { _throwOpError(">="); }
  
  // non-param/param and param/non-param
  inline proc >=(a: uint(64), param b: uint(64)) {
    if b == 0 then return true; else return __primitive(">=", a, b);
  }
  inline proc >=(param a: uint(64), b: uint(64)) {
    return __primitive(">=", a, b);
  }
  
  
  // non-param/non-param
  inline proc <=(a: uint(64), b: int(64)) { _throwOpError("<="); }
  inline proc <=(a: int(64), b: uint(64)) { _throwOpError("<="); }
  
  // non-param/param and param/non-param
  inline proc <=(a: uint(64), param b: uint(64)) {
    return __primitive("<=", a, b);
  }
  inline proc <=(param a: uint(64), b: uint(64)) {
    if a == 0 then return true; else return __primitive("<=", a, b);
  }

  proc numFields(type t) param {
    return __primitive("num fields", t);
  }
  
  proc fieldNumToName(type t, param i) param {
    return __primitive("field num to name", t, i);
  }
  
  proc fieldValueByNum(x, param i) {
    return __primitive("field value by num", x, i);
  }
  
  proc fieldValueByName(x, param name) {
    compilerError("Not yet implemented");
    return __primitive("field value by name", x, name);
  }
  
  proc isClassType(type t) param where t:object return true;
  proc isClassType(type t) param return false;
  
  proc isRecordType(type t) param where t: value {
    // some non-record types are implemented via records - exclude those
    if
      isDmapType(t)   ||
      isDomainType(t) ||
      isArrayType(t)  ||
      isRangeType(t)  ||
      isTupleType(t)  ||
      isAtomicType(t)
    then
      return false;
    else
      return true;
  }
  proc isRecordType(type t) param return false;
  
  proc isUnionType(type t) param return __primitive("is union type", t);

  proc isAtomicType(type t) param return __primitive("is atomic type", t);
  
  _ensure_reference_type(int(8));
  _ensure_reference_type(int(16));
  _ensure_reference_type(int(32));
  _ensure_reference_type(int(64));
  _ensure_reference_type(uint(8));
  _ensure_reference_type(uint(16));
  _ensure_reference_type(uint(32));
  _ensure_reference_type(uint(64));
  _ensure_reference_type(real(32));
  _ensure_reference_type(real(64));
  _ensure_reference_type(imag(32));
  _ensure_reference_type(imag(64));
  _ensure_reference_type(complex(64));
  _ensure_reference_type(complex(128));
  _ensure_reference_type(bool);
  _ensure_reference_type(_task_list);
  _ensure_reference_type(string);
  _ensure_reference_type(chpl_taskID_t);

  // These style element #s are used in the default Writer and Reader.
  // and in e.g. implementations of those in Tuple.
  extern const QIO_STYLE_ELEMENT_STRING:int;
  extern const QIO_STYLE_ELEMENT_COMPLEX:int;
  extern const QIO_STYLE_ELEMENT_ARRAY:int;
  extern const QIO_STYLE_ELEMENT_AGGREGATE:int;
  extern const QIO_STYLE_ELEMENT_TUPLE:int;

  extern const QIO_ARRAY_FORMAT_SPACE:int;
  extern const QIO_ARRAY_FORMAT_CHPL:int;
  extern const QIO_ARRAY_FORMAT_JSON:int;

  extern const QIO_AGGREGATE_FORMAT_BRACES:int;
  extern const QIO_AGGREGATE_FORMAT_CHPL:int;
  extern const QIO_AGGREGATE_FORMAT_JSON:int;

  extern const QIO_TUPLE_FORMAT_CHPL:int;
  extern const QIO_TUPLE_FORMAT_SPACE:int;
  extern const QIO_TUPLE_FORMAT_JSON:int;

}

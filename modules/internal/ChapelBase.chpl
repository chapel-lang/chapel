_extern def chpl_config_has_value(name, module_name): bool;
_extern def chpl_config_get_value(name, module_name): string;

config param CHPL_HOST_PLATFORM: string = "unset";
config param CHPL_TARGET_PLATFORM: string = "unset";
if (CHPL_HOST_PLATFORM == "unset") {
  compilerWarning("CHPL_HOST_PLATFORM not set");
}
if (CHPL_TARGET_PLATFORM == "unset") {
  compilerWarning("CHPL_TARGET_PLATFORM not set");
}

config param CHPL_HOST_COMPILER: string = "unset";
config param CHPL_TARGET_COMPILER: string = "unset";
if (CHPL_HOST_COMPILER == "unset") {
  compilerWarning("CHPL_HOST_COMPILER not set");
}
if (CHPL_TARGET_COMPILER == "unset") {
  compilerWarning("CHPL_TARGET_COMPILER not set");
}

config param CHPL_TASKS: string = "unset";
if (CHPL_TASKS == "unset") {
  compilerWarning("CHPL_TASKS not set");
}

config param CHPL_COMM: string = "unset";
if (CHPL_COMM == "unset") {
  compilerWarning("CHPL_COMM not set");
}

pragma "inline" def +(s: string, x: numeric)
  return s + x:string;

pragma "inline" def +(x: numeric, s: string)
  return x:string + s;

def +(param s: string, param x: integral) param
  return s + x:string;

def +(param x: integral, param s: string) param
  return x:string + s;

pragma "inline" def +(s: string, x: enumerated)
  return s + x:string;

pragma "inline" def +(x: enumerated, s: string)
  return x:string + s;

def +(param s: string, param x: enumerated) param
  return s + x:string;

def +(param x: enumerated, param s: string) param
  return x:string + s;

pragma "inline" def +(s: string, x: bool)
  return s + x:string;

pragma "inline" def +(x: bool, s: string)
  return x:string + s;

def +(param s: string, param x: bool) param
  return s + x:string;

def +(param x: bool, param s: string) param
  return x:string + s;

config param realmTypes: string = CHPL_TARGET_PLATFORM;
param numRealms: int(32) = __primitive("get num realms"); // defined by realmTypes

def _throwOpError(param op: string) {
    compilerError("illegal use of '", op, "' on operands of type uint(64) and signed integer");
}

def compilerError(param x:string ...?n, param errorDepth:int) {
  __primitive("error", (...x));
}

def compilerError(param x:string ...?n) {
  __primitive("error", (...x));
}

def compilerWarning(param x:string ...?n, param errorDepth:int) {
  __primitive("warning", (...x));
}

def compilerWarning(param x:string ...?n) {
  __primitive("warning", (...x));
}

def typeToString(type t) param {
  return __primitive("typeToString", t);
}

enum iterator {leader, follower};

//
// assignment on primitive types
//
pragma "inline" def =(a: bool, b: bool) return b;
pragma "inline" def =(a: bool(?w), b: bool) return b:a.type;
pragma "inline" def =(a: int(?w), b: int(w)) return b;
pragma "inline" def =(a: uint(?w), b: uint(w)) return b;
pragma "inline" def =(a: real(?w), b: real(w)) return b;
pragma "inline" def =(a: imag(?w), b: imag(w)) return b;
pragma "inline" def =(a: complex(?w), b: complex(w)) return b;
pragma "inline" def =(a: string, b: string) return __primitive("string_copy", b);
pragma "inline" def =(a, b) return b;

pragma "inline" def ==(a: chpl_taskID_t, b: chpl_taskID_t) return __primitive("==", a, b);

//
// equality comparison on primitive types
//
pragma "inline" def ==(a: bool, b: bool) return __primitive("==", a, b);
pragma "inline" def ==(a: int(32), b: int(32)) return __primitive("==", a, b);
pragma "inline" def ==(a: int(64), b: int(64)) return __primitive("==", a, b);
pragma "inline" def ==(a: uint(32), b: uint(32)) return __primitive("==", a, b);
pragma "inline" def ==(a: uint(64), b: uint(64)) return __primitive("==", a, b);
pragma "inline" def ==(a: real(?w), b: real(w)) return __primitive("==", a, b);
pragma "inline" def ==(a: imag(?w), b: imag(w)) return __primitive("==", a, b);
pragma "inline" def ==(a: complex(?w), b: complex(w)) return a.re == b.re && a.im == b.im;
pragma "inline" def ==(a: string, b: string) return (__primitive("chpl_string_compare", a, b) == 0);
pragma "inline" def ==(a: object, b: object) return __primitive("ptr_eq", a, b);

pragma "inline" def !=(a: bool, b: bool) return __primitive("!=", a, b);
pragma "inline" def !=(a: int(32), b: int(32)) return __primitive("!=", a, b);
pragma "inline" def !=(a: int(64), b: int(64)) return __primitive("!=", a, b);
pragma "inline" def !=(a: uint(32), b: uint(32)) return __primitive("!=", a, b);
pragma "inline" def !=(a: uint(64), b: uint(64)) return __primitive("!=", a, b);
pragma "inline" def !=(a: real(?w), b: real(w)) return __primitive("!=", a, b);
pragma "inline" def !=(a: imag(?w), b: imag(w)) return __primitive("!=", a, b);
pragma "inline" def !=(a: complex(?w), b: complex(w)) return a.re != b.re || a.im != b.im;
pragma "inline" def !=(a: string, b: string) return (__primitive("chpl_string_compare", a, b) != 0);
pragma "inline" def !=(a: object, b: object) return __primitive("ptr_neq", a, b);

pragma "inline" def ==(param a: bool, param b: bool) param return __primitive("==", a, b);
pragma "inline" def ==(param a: int(32), param b: int(32)) param return __primitive("==", a, b);
pragma "inline" def ==(param a: int(64), param b: int(64)) param return __primitive("==", a, b);
pragma "inline" def ==(param a: uint(32), param b: uint(32)) param return __primitive("==", a, b);
pragma "inline" def ==(param a: uint(64), param b: uint(64)) param return __primitive("==", a, b);
pragma "inline" def ==(param a: enumerated, param b: enumerated) param return __primitive("==", a, b);
pragma "inline" def ==(param a: string, param b: string) param return __primitive("chpl_string_compare", a, b) == 0;

pragma "inline" def !=(param a: bool, param b: bool) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: int(32), param b: int(32)) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: int(64), param b: int(64)) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: uint(32), param b: uint(32)) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: uint(64), param b: uint(64)) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: enumerated, param b: enumerated) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: string, param b: string) param return __primitive("chpl_string_compare", a, b) != 0;

//
// ordered comparison on primitive types
//
pragma "inline" def <=(a: int(32), b: int(32)) return __primitive("<=", a, b);
pragma "inline" def <=(a: int(64), b: int(64)) return __primitive("<=", a, b);
pragma "inline" def <=(a: uint(32), b: uint(32)) return __primitive("<=", a, b);
pragma "inline" def <=(a: uint(64), b: uint(64)) return __primitive("<=", a, b);
pragma "inline" def <=(a: real(?w), b: real(w)) return __primitive("<=", a, b);
pragma "inline" def <=(a: imag(?w), b: imag(w)) return __primitive("<=", a, b);
pragma "inline" def <=(a: string, b: string) return (__primitive("chpl_string_compare", a, b) <= 0);

pragma "inline" def >=(a: int(32), b: int(32)) return __primitive(">=", a, b);
pragma "inline" def >=(a: int(64), b: int(64)) return __primitive(">=", a, b);
pragma "inline" def >=(a: uint(32), b: uint(32)) return __primitive(">=", a, b);
pragma "inline" def >=(a: uint(64), b: uint(64)) return __primitive(">=", a, b);
pragma "inline" def >=(a: real(?w), b: real(w)) return __primitive(">=", a, b);
pragma "inline" def >=(a: imag(?w), b: imag(w)) return __primitive(">=", a, b);
pragma "inline" def >=(a: string, b: string) return (__primitive("chpl_string_compare", a, b) >= 0);

pragma "inline" def <(a: int(32), b: int(32)) return __primitive("<", a, b);
pragma "inline" def <(a: int(64), b: int(64)) return __primitive("<", a, b);
pragma "inline" def <(a: uint(32), b: uint(32)) return __primitive("<", a, b);
pragma "inline" def <(a: uint(64), b: uint(64)) return __primitive("<", a, b);
pragma "inline" def <(a: real(?w), b: real(w)) return __primitive("<", a, b);
pragma "inline" def <(a: imag(?w), b: imag(w)) return __primitive("<", a, b);
pragma "inline" def <(a: string, b: string) return (__primitive("chpl_string_compare", a, b) < 0);

pragma "inline" def >(a: int(32), b: int(32)) return __primitive(">", a, b);
pragma "inline" def >(a: int(64), b: int(64)) return __primitive(">", a, b);
pragma "inline" def >(a: uint(32), b: uint(32)) return __primitive(">", a, b);
pragma "inline" def >(a: uint(64), b: uint(64)) return __primitive(">", a, b);
pragma "inline" def >(a: real(?w), b: real(w)) return __primitive(">", a, b);
pragma "inline" def >(a: imag(?w), b: imag(w)) return __primitive(">", a, b);
pragma "inline" def >(a: string, b: string) return (__primitive("chpl_string_compare", a, b) > 0);

pragma "inline" def <=(param a: int(32), param b: int(32)) param return __primitive("<=", a, b);
pragma "inline" def <=(param a: int(64), param b: int(64)) param return __primitive("<=", a, b);
pragma "inline" def <=(param a: uint(32), param b: uint(32)) param return __primitive("<=", a, b);
pragma "inline" def <=(param a: uint(64), param b: uint(64)) param return __primitive("<=", a, b);
pragma "inline" def <=(param a: enumerated, param b: enumerated) param return __primitive("<=", a, b);
pragma "inline" def <=(param a: string, param b: string) param return __primitive("chpl_string_compare", a, b) <= 0;

pragma "inline" def >=(param a: int(32), param b: int(32)) param return __primitive(">=", a, b);
pragma "inline" def >=(param a: int(64), param b: int(64)) param return __primitive(">=", a, b);
pragma "inline" def >=(param a: uint(32), param b: uint(32)) param return __primitive(">=", a, b);
pragma "inline" def >=(param a: uint(64), param b: uint(64)) param return __primitive(">=", a, b);
pragma "inline" def >=(param a: enumerated, param b: enumerated) param return __primitive(">=", a, b);
pragma "inline" def >=(param a: string, param b: string) param return __primitive("chpl_string_compare", a, b) >= 0;

pragma "inline" def <(param a: int(32), param b: int(32)) param return __primitive("<", a, b);
pragma "inline" def <(param a: int(64), param b: int(64)) param return __primitive("<", a, b);
pragma "inline" def <(param a: uint(32), param b: uint(32)) param return __primitive("<", a, b);
pragma "inline" def <(param a: uint(64), param b: uint(64)) param return __primitive("<", a, b);
pragma "inline" def <(param a: enumerated, param b: enumerated) param return __primitive("<", a, b);
pragma "inline" def <(param a: string, param b: string) param return __primitive("chpl_string_compare", a, b) < 0;

pragma "inline" def >(param a: int(32), param b: int(32)) param return __primitive(">", a, b);
pragma "inline" def >(param a: int(64), param b: int(64)) param return __primitive(">", a, b);
pragma "inline" def >(param a: uint(32), param b: uint(32)) param return __primitive(">", a, b);
pragma "inline" def >(param a: uint(64), param b: uint(64)) param return __primitive(">", a, b);
pragma "inline" def >(param a: enumerated, param b: enumerated) param return __primitive(">", a, b);
pragma "inline" def >(param a: string, param b: string) param return (__primitive("chpl_string_compare", a, b) > 0);

//
// unary + and - on primitive types
//
pragma "inline" def +(a: int(32)) return a;
pragma "inline" def +(a: int(64)) return a;
pragma "inline" def +(a: uint(32)) return a;
pragma "inline" def +(a: uint(64)) return a;
pragma "inline" def +(a: real(?w)) return a;
pragma "inline" def +(a: imag(?w)) return a;
pragma "inline" def +(a: complex(?w)) return a;

pragma "inline" def -(a: int(32)) return __primitive("u-", a);
pragma "inline" def -(a: int(64)) return __primitive("u-", a);
pragma "inline" def -(a: uint(64)) { compilerError("illegal use of '-' on operand of type ", typeToString(a.type)); }
pragma "inline" def -(a: real(?w)) return __primitive("u-", a);
pragma "inline" def -(a: imag(?w)) return __primitive("u-", a);
pragma "inline" def -(a: complex(?w)) return (-a.re, -a.im):complex;

pragma "inline" def +(param a: int(32)) param return a;
pragma "inline" def +(param a: int(64)) param return a;
pragma "inline" def +(param a: uint(32)) param return a;
pragma "inline" def +(param a: uint(64)) param return a;

pragma "inline" def -(param a: int(32)) param return __primitive("u-", a);
pragma "inline" def -(param a: int(64)) param return __primitive("u-", a);
pragma "inline" def -(param a: uint(64)) param {
  if (a:int(64) < 0) then
    compilerError("illegal use of '-' on operand of type ", typeToString(a.type));
  else
    return -(a:int(64));
}

//
// binary + and - on primitive types
//
pragma "inline" def +(a: int(32), b: int(32)) return __primitive("+", a, b);
pragma "inline" def +(a: int(64), b: int(64)) return __primitive("+", a, b);
pragma "inline" def +(a: uint(32), b: uint(32)) return __primitive("+", a, b);
pragma "inline" def +(a: uint(64), b: uint(64)) return __primitive("+", a, b);
pragma "inline" def +(a: real(?w), b: real(w)) return __primitive("+", a, b);
pragma "inline" def +(a: imag(?w), b: imag(w)) return __primitive("+", a, b);
pragma "inline" def +(a: complex(?w), b: complex(w)) return (a.re+b.re, a.im+b.im):complex;
pragma "inline" def +(a: string, b: string) return __primitive("string_concat", a, b);

pragma "inline" def +(a: real(?w), b: imag(w)) return (a, _i2r(b)):complex;
pragma "inline" def +(a: imag(?w), b: real(w)) return (b, _i2r(a)):complex;
pragma "inline" def +(a: real(?w), b: complex(w*2)) return (a+b.re, b.im):complex;
pragma "inline" def +(a: complex(?w), b: real(w/2)) return (a.re+b, a.im):complex;
pragma "inline" def +(a: imag(?w), b: complex(w*2)) return (b.re, _i2r(a)+b.im):complex;
pragma "inline" def +(a: complex(?w), b: imag(w/2)) return (a.re, a.im+_i2r(b)):complex;

pragma "inline" def -(a: int(32), b: int(32)) return __primitive("-", a, b);
pragma "inline" def -(a: int(64), b: int(64)) return __primitive("-", a, b);
pragma "inline" def -(a: uint(32), b: uint(32)) return __primitive("-", a, b);
pragma "inline" def -(a: uint(64), b: uint(64)) return __primitive("-", a, b);
pragma "inline" def -(a: real(?w), b: real(w)) return __primitive("-", a, b);
pragma "inline" def -(a: imag(?w), b: imag(w)) return __primitive("-", a, b);
pragma "inline" def -(a: complex(?w), b: complex(w)) return (a.re-b.re, a.im-b.im):complex;

pragma "inline" def -(a: real(?w), b: imag(w)) return (a, -_i2r(b)):complex;
pragma "inline" def -(a: imag(?w), b: real(w)) return (-b, _i2r(a)):complex;
pragma "inline" def -(a: real(?w), b: complex(w*2)) return (a-b.re, -b.im):complex;
pragma "inline" def -(a: complex(?w), b: real(w/2)) return (a.re-b, a.im):complex;
pragma "inline" def -(a: imag(?w), b: complex(w*2)) return (-b.re, _i2r(a)-b.im):complex;
pragma "inline" def -(a: complex(?w), b: imag(w/2)) return (a.re, a.im-_i2r(b)):complex;

pragma "inline" def +(param a: int(32), param b: int(32)) param return __primitive("+", a, b);
pragma "inline" def +(param a: int(64), param b: int(64)) param return __primitive("+", a, b);
pragma "inline" def +(param a: uint(32), param b: uint(32)) param return __primitive("+", a, b);
pragma "inline" def +(param a: uint(64), param b: uint(64)) param return __primitive("+", a, b);
pragma "inline" def +(param a: string, param b: string) param return __primitive("string_concat", a, b);

pragma "inline" def -(param a: int(32), param b: int(32)) param return __primitive("-", a, b);
pragma "inline" def -(param a: int(64), param b: int(64)) param return __primitive("-", a, b);
pragma "inline" def -(param a: uint(32), param b: uint(32)) param return __primitive("-", a, b);
pragma "inline" def -(param a: uint(64), param b: uint(64)) param return __primitive("-", a, b);

//
// * and / on primitive types
//
pragma "inline" def *(a: int(32), b: int(32)) return __primitive("*", a, b);
pragma "inline" def *(a: int(64), b: int(64)) return __primitive("*", a, b);
pragma "inline" def *(a: uint(32), b: uint(32)) return __primitive("*", a, b);
pragma "inline" def *(a: uint(64), b: uint(64)) return __primitive("*", a, b);
pragma "inline" def *(a: real(?w), b: real(w)) return __primitive("*", a, b);
pragma "inline" def *(a: imag(?w), b: imag(w)) return _i2r(__primitive("*", a, b));
pragma "inline" def *(a: complex(?w), b: complex(w)) return (a.re*b.re-a.im*b.im, a.im*b.re+a.re*b.im):complex;

pragma "inline" def *(a: real(?w), b: imag(w)) return _r2i(a*_i2r(b));
pragma "inline" def *(a: imag(?w), b: real(w)) return _r2i(_i2r(a)*b);
pragma "inline" def *(a: real(?w), b: complex(w*2)) return (a*b.re, a*b.im):complex;
pragma "inline" def *(a: complex(?w), b: real(w/2)) return (a.re*b, a.im*b):complex;
pragma "inline" def *(a: imag(?w), b: complex(w*2)) return (-_i2r(a)*b.im, _i2r(a)*b.re):complex;
pragma "inline" def *(a: complex(?w), b: imag(w/2)) return (-a.im*_i2r(b), a.re*_i2r(b)):complex;

pragma "inline" def /(a: int(32), b: int(32)) return __primitive("/", a, b);
pragma "inline" def /(a: int(64), b: int(64)) return __primitive("/", a, b);
pragma "inline" def /(a: uint(32), b: uint(32)) return __primitive("/", a, b);
pragma "inline" def /(a: uint(64), b: uint(64)) return __primitive("/", a, b);
pragma "inline" def /(a: real(?w), b: real(w)) return __primitive("/", a, b);
pragma "inline" def /(a: imag(?w), b: imag(w)) return _i2r(__primitive("/", a, b));
pragma "inline" def /(a: complex(?w), b: complex(w))
  return let d = b.re*b.re+b.im*b.im in
    ((a.re*b.re+a.im*b.im)/d, (a.im*b.re-a.re*b.im)/d):complex;

pragma "inline" def /(a: real(?w), b: imag(w)) return _r2i(-a/_i2r(b));
pragma "inline" def /(a: imag(?w), b: real(w)) return _r2i(_i2r(a)/b);
pragma "inline" def /(a: real(?w), b: complex(w*2))
  return let d = b.re*b.re+b.im*b.im in
    (a*b.re/d, -a*b.im/d):complex;
pragma "inline" def /(a: complex(?w), b: real(w/2))
  return (a.re/b, a.im/b):complex;
pragma "inline" def /(a: imag(?w), b: complex(w*2))
  return let d = b.re*b.re+b.im*b.im in
    (_i2r(a)*b.im/d, _i2r(a)*b.re/d):complex;
pragma "inline" def /(a: complex(?w), b: imag(w/2))
  return let d = _i2r(b)*_i2r(b) in
    (a.im/_i2r(b), -a.re/_i2r(b)):complex;

pragma "inline" def *(param a: int(32), param b: int(32)) param return __primitive("*", a, b);
pragma "inline" def *(param a: int(64), param b: int(64)) param return __primitive("*", a, b);
pragma "inline" def *(param a: uint(32), param b: uint(32)) param return __primitive("*", a, b);
pragma "inline" def *(param a: uint(64), param b: uint(64)) param return __primitive("*", a, b);

pragma "inline" def /(param a: int(32), param b: int(32)) param return __primitive("/", a, b);
pragma "inline" def /(param a: int(64), param b: int(64)) param return __primitive("/", a, b);
pragma "inline" def /(param a: uint(32), param b: uint(32)) param return __primitive("/", a, b);
pragma "inline" def /(param a: uint(64), param b: uint(64)) param return __primitive("/", a, b);

//
// % on primitive types
//
pragma "inline" def %(a: int(32), b: int(32)) return __primitive("%", a, b);
pragma "inline" def %(a: int(64), b: int(64)) return __primitive("%", a, b);
pragma "inline" def %(a: uint(32), b: uint(32)) return __primitive("%", a, b);
pragma "inline" def %(a: uint(64), b: uint(64)) return __primitive("%", a, b);

pragma "inline" def %(param a: int(32), param b: int(32)) param return __primitive("%", a, b);
pragma "inline" def %(param a: int(64), param b: int(64)) param return __primitive("%", a, b);
pragma "inline" def %(param a: uint(32), param b: uint(32)) param return __primitive("%", a, b);
pragma "inline" def %(param a: uint(64), param b: uint(64)) param return __primitive("%", a, b);

//
// ** on primitive types
//

pragma "inline" def _intExpHelp(a: integral, b) where a.type == b.type {
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

pragma "inline" def **(a: int(32), b: int(32)) return _intExpHelp(a, b);
pragma "inline" def **(a: int(64), b: int(64)) return _intExpHelp(a, b);
pragma "inline" def **(a: uint(32), b: uint(32)) return _intExpHelp(a, b);
pragma "inline" def **(a: uint(64), b: uint(64)) return _intExpHelp(a, b);
pragma "inline" def **(a: real(?w), b: real(w)) return __primitive("**", a, b);

def **(param a: int(32), param b: int(32)) param return __primitive("**", a, b);
def **(param a: int(64), param b: int(64)) param return __primitive("**", a, b);
def **(param a: uint(32), param b: uint(32)) param return __primitive("**", a, b);
def **(param a: uint(64), param b: uint(64)) param return __primitive("**", a, b);

pragma "inline" def _expHelp(a, param b: integral) {
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

def _canOptimizeExp(param b: integral) param return b >= 0 && b <= 8 && b != 7;

pragma "inline" def **(a: int(32), param b: integral) where _canOptimizeExp(b) return _expHelp(a, b);
pragma "inline" def **(a: int(64), param b: integral) where _canOptimizeExp(b) return _expHelp(a, b);
pragma "inline" def **(a: uint(32), param b: integral) where _canOptimizeExp(b) return _expHelp(a, b);
pragma "inline" def **(a: uint(64), param b: integral) where _canOptimizeExp(b) return _expHelp(a, b);
pragma "inline" def **(a: real(?w), param b: integral) where _canOptimizeExp(b) return _expHelp(a, b);

//
// logical operations on primitive types
//
pragma "inline" def !(a: bool) return __primitive("!", a);

pragma "inline" def isTrue(a: bool) return a;
pragma "inline" def isTrue(param a: bool) param return a;

def isTrue(a: integral) { compilerError("short-circuiting logical operators not supported on integers"); }

pragma "inline" def !(param a: bool) param return __primitive("!", a);

//
// bitwise operations on primitive types
//
pragma "inline" def ~(a: bool) return __primitive("u~", a);
pragma "inline" def ~(a: int(32)) return __primitive("u~", a);
pragma "inline" def ~(a: int(64)) return __primitive("u~", a);
pragma "inline" def ~(a: uint(32)) return __primitive("u~", a);
pragma "inline" def ~(a: uint(64)) return __primitive("u~", a);

pragma "inline" def &(a: bool, b: bool) return __primitive("&", a, b);
pragma "inline" def &(a: int(32), b: int(32)) return __primitive("&", a, b);
pragma "inline" def &(a: int(64), b: int(64)) return __primitive("&", a, b);
pragma "inline" def &(a: uint(32), b: uint(32)) return __primitive("&", a, b);
pragma "inline" def &(a: uint(64), b: uint(64)) return __primitive("&", a, b);
pragma "inline" def &(a: uint(32), b: int(32)) return __primitive("&", a, b:uint(32));
pragma "inline" def &(a: int(32), b: uint(32)) return __primitive("&", a:uint(32), b);
pragma "inline" def &(a: uint(64), b: int(64)) return __primitive("&", a, b:uint(64));
pragma "inline" def &(a: int(64), b: uint(64)) return __primitive("&", a:uint(64), b);

pragma "inline" def |(a: bool, b: bool) return __primitive("|", a, b);
pragma "inline" def |(a: int(32), b: int(32)) return __primitive("|", a, b);
pragma "inline" def |(a: int(64), b: int(64)) return __primitive("|", a, b);
pragma "inline" def |(a: uint(32), b: uint(32)) return __primitive("|", a, b);
pragma "inline" def |(a: uint(64), b: uint(64)) return __primitive("|", a, b);
pragma "inline" def |(a: uint(32), b: int(32)) return __primitive("|", a, b:uint(32));
pragma "inline" def |(a: int(32), b: uint(32)) return __primitive("|", a:uint(32), b);
pragma "inline" def |(a: uint(64), b: int(64)) return __primitive("|", a, b:uint(64));
pragma "inline" def |(a: int(64), b: uint(64)) return __primitive("|", a:uint(64), b);

pragma "inline" def ^(a: bool, b: bool) return __primitive("^", a, b);
pragma "inline" def ^(a: int(32), b: int(32)) return __primitive("^", a, b);
pragma "inline" def ^(a: int(64), b: int(64)) return __primitive("^", a, b);
pragma "inline" def ^(a: uint(32), b: uint(32)) return __primitive("^", a, b);
pragma "inline" def ^(a: uint(64), b: uint(64)) return __primitive("^", a, b);
pragma "inline" def ^(a: uint(32), b: int(32)) return __primitive("^", a, b:uint(32));
pragma "inline" def ^(a: int(32), b: uint(32)) return __primitive("^", a:uint(32), b);
pragma "inline" def ^(a: uint(64), b: int(64)) return __primitive("^", a, b:uint(64));
pragma "inline" def ^(a: int(64), b: uint(64)) return __primitive("^", a:uint(64), b);

pragma "inline" def ~(param a: bool) param return __primitive("u~", a);
pragma "inline" def ~(param a: int(32)) param return __primitive("u~", a);
pragma "inline" def ~(param a: int(64)) param return __primitive("u~", a);
pragma "inline" def ~(param a: uint(32)) param return __primitive("u~", a);
pragma "inline" def ~(param a: uint(64)) param return __primitive("u~", a);

pragma "inline" def &(param a: bool, param b: bool) param return __primitive("&", a, b);
pragma "inline" def &(param a: int(32), param b: int(32)) param return __primitive("&", a, b);
pragma "inline" def &(param a: int(64), param b: int(64)) param return __primitive("&", a, b);
pragma "inline" def &(param a: uint(32), param b: uint(32)) param return __primitive("&", a, b);
pragma "inline" def &(param a: uint(64), param b: uint(64)) param return __primitive("&", a, b);
pragma "inline" def &(param a: uint(32), param b: int(32)) param return __primitive("&", a, b:uint(32));
pragma "inline" def &(param a: int(32), param b: uint(32)) param return __primitive("&", a:uint(32), b);
pragma "inline" def &(param a: uint(64), param b: int(64)) param return __primitive("&", a, b:uint(64));
pragma "inline" def &(param a: int(64), param b: uint(64)) param return __primitive("&", a:uint(64), b);

pragma "inline" def |(param a: bool, param b: bool) param return __primitive("|", a, b);
pragma "inline" def |(param a: int(32), param b: int(32)) param return __primitive("|", a, b);
pragma "inline" def |(param a: int(64), param b: int(64)) param return __primitive("|", a, b);
pragma "inline" def |(param a: uint(32), param b: uint(32)) param return __primitive("|", a, b);
pragma "inline" def |(param a: uint(64), param b: uint(64)) param return __primitive("|", a, b);
pragma "inline" def |(param a: uint(32), param b: int(32)) param return __primitive("|", a, b:uint(32));
pragma "inline" def |(param a: int(32), param b: uint(32)) param return __primitive("|", a:uint(32), b);
pragma "inline" def |(param a: uint(64), param b: int(64)) param return __primitive("|", a, b:uint(64));
pragma "inline" def |(param a: int(64), param b: uint(64)) param return __primitive("|", a:uint(64), b);

pragma "inline" def ^(param a: bool, param b: bool) param return __primitive("^", a, b);
pragma "inline" def ^(param a: int(32), param b: int(32)) param return __primitive("^", a, b);
pragma "inline" def ^(param a: int(64), param b: int(64)) param return __primitive("^", a, b);
pragma "inline" def ^(param a: uint(32), param b: uint(32)) param return __primitive("^", a, b);
pragma "inline" def ^(param a: uint(64), param b: uint(64)) param return __primitive("^", a, b);
pragma "inline" def ^(param a: uint(32), param b: int(32)) param return __primitive("^", a, b:uint(32));
pragma "inline" def ^(param a: int(32), param b: uint(32)) param return __primitive("^", a:uint(32), b);
pragma "inline" def ^(param a: uint(64), param b: int(64)) param return __primitive("^", a, b:uint(64));
pragma "inline" def ^(param a: int(64), param b: uint(64)) param return __primitive("^", a:uint(64), b);

//
// left and right shift on primitive types
//
pragma "inline" def <<(a: int(32), b: integral) return __primitive("<<", a, b);
pragma "inline" def <<(a: int(64), b: integral) return __primitive("<<", a, b);
pragma "inline" def <<(a: uint(32), b: integral) return __primitive("<<", a, b);
pragma "inline" def <<(a: uint(64), b: integral) return __primitive("<<", a, b);

pragma "inline" def >>(a: int(32), b: integral) return __primitive(">>", a, b);
pragma "inline" def >>(a: int(64), b: integral) return __primitive(">>", a, b);
pragma "inline" def >>(a: uint(32), b: integral) return __primitive(">>", a, b);
pragma "inline" def >>(a: uint(64), b: integral) return __primitive(">>", a, b);

pragma "inline" def <<(param a: int(32), param b: integral) param return __primitive("<<", a, b);
pragma "inline" def <<(param a: int(64), param b: integral) param return __primitive("<<", a, b);
pragma "inline" def <<(param a: uint(32), param b: integral) param return __primitive("<<", a, b);
pragma "inline" def <<(param a: uint(64), param b: integral) param return __primitive("<<", a, b);

pragma "inline" def >>(param a: int(32), param b: integral) param return __primitive(">>", a, b);
pragma "inline" def >>(param a: int(64), param b: integral) param return __primitive(">>", a, b);
pragma "inline" def >>(param a: uint(32), param b: integral) param return __primitive(">>", a, b);
pragma "inline" def >>(param a: uint(64), param b: integral) param return __primitive(">>", a, b);

//
// These functions are to handle symbols at statement level such as
// the second statement here: var a: sync int = 1; a;
//
pragma "inline" def _statementLevelSymbol(a) { return a; }
pragma "inline" def _statementLevelSymbol(param a) param { return a; }
pragma "inline" def _statementLevelSymbol(type a) type { return a; }

//
// _cond_test function supports statement bool conversions and sync
//   variables in conditional statements; and checks for errors
// _cond_invalid function checks a conditional expression for
//   incorrectness; it is used to give better error messages for
//   promotion of && and ||
//
pragma "inline" def _cond_test(x: object) return x != nil;
pragma "inline" def _cond_test(x: bool) return x;
pragma "inline" def _cond_test(x: integral) return x != 0;

pragma "inline" def _cond_test(param x: bool) param return x;
pragma "inline" def _cond_test(param x: integral) param return x != 0;

pragma "inline" def _cond_test(x) {
  compilerError("type '", typeToString(x.type), "' used in if or while condition");
}

pragma "inline" def _cond_test(x: _iteratorRecord) {
  compilerError("iterator or promoted expression used in if or while condition");
}

def _cond_invalid(x: object) param return false;
def _cond_invalid(x: bool) param return false;
def _cond_invalid(x: integral) param return false;
def _cond_invalid(x) param return true;



//
// complex component methods re and im
//
//  bug?  in setters, parameterize real argument over complex bit width
//
pragma "inline" pragma "ref this"
def chpl_anycomplex.re var return __primitive("complex_get_real", this);
pragma "inline" pragma "ref this"
def chpl_anycomplex.im var return __primitive("complex_get_imag", this);

//
// helper functions
//
pragma "inline" def _i2r(a: imag(?w)) return __primitive("cast", real(w), a);
pragma "inline" def _r2i(a: real(?w)) return __primitive("cast", imag(w), a);

//
// primitive string functions and methods
//
pragma "inline" def ascii(a: string) return __primitive("ascii", a);
pragma "inline" def string.length return __primitive("string_length", this);
pragma "inline" def string.substring(i: int) return __primitive("string_index", this, i);
pragma "inline" def _string_contains(a: string, b: string) return __primitive("string_contains", a, b);

pragma "inline" def ascii(param a: string) param return __primitive("ascii", a);
pragma "inline" def param string.length param return __primitive("string_length", this);
pragma "inline" def _string_contains(param a: string, param b: string) param return __primitive("string_contains", a, b);

//
// identity functions (for reductions)
//
pragma "inline" def _prod_id(type t) return __primitive("_prod_id", t);
pragma "inline" def _land_id(type t) return __primitive("_land_id", t);
pragma "inline" def _lor_id(type t) return __primitive("_lor_id", t);
pragma "inline" def _lxor_id(type t) return __primitive("_lxor_id", t);
pragma "inline" def _band_id(type t) return __primitive("_band_id", t);
pragma "inline" def _bor_id(type t) return __primitive("_bor_id", t);
pragma "inline" def _bxor_id(type t) return __primitive("_bxor_id", t);

//
// min and max
//
pragma "inline" def min(x, y) return if x < y then x else y;
pragma "inline" def max(x, y) return if x > y then x else y;
pragma "inline" def min(x, y, z...?k) return min(min(x, y), (...z));
pragma "inline" def max(x, y, z...?k) return max(max(x, y), (...z));

//
// More primitive funs
//
pragma "inline" def exit(status: int) {
  __primitive("chpl_exit_any", status);
}

def init_elts(x, s, type t) {
  for i in 1..s {
    pragma "no auto destroy" var y: t;  // TODO: why is this in the loop?
    __primitive("array_set_first", x, i-1, y);
  }
}

// dynamic data block class
pragma "data class"
class _ddata {
  type eltType;
  def ~_ddata() {
    __primitive("array_free", this);
  }
  pragma "inline" def init(size: integral) {
    __primitive("array_alloc", this, eltType, size);
    init_elts(this, size, eltType);
  }
  pragma "inline" def this(i: integral) var {
    return __primitive("array_get", this, i);
  }
}

//
// internal reference type
//
pragma "ref"
pragma "no default functions"
pragma "no object"
class _ref {
  var _val;
}

def chpl__readXX(x: sync) return x.readXX();
def chpl__readXX(x: single) return x.readXX();
def chpl__readXX(x) return x;

// Returns whether an object of type t occupies a 64-bit word on Cray's MTA/XMT
// (The definition of this function should be target dependent.  This would avoid
// the need to write C macros in the runtime that essentially duplicate
// the functionality of the read/write methods of the _syncvar and _singlevar classes
// for targets that don't have particularly fast ways of achieving this functionality
// for simple base types.)
def isSimpleSyncBaseType (type t) param {
  if CHPL_TASKS == "mta" then
    if t == int(64) || t == uint(64) || t == int(32) || t == uint(32)
        || t == int(16) || t == uint(16) || t == int(8) || t == uint(8)
        || t == real(32) || t == real(64) || t == imag(32) || t == imag(64) then
        return true;
    else return false;
  else return false;
}

pragma "sync"
pragma "no default functions"
pragma "no object"
class _syncvar {
  type base_type;
  var  value: base_type;       // actual data - may need to be declared specially on some targets!
  pragma "omit from constructor" var sync_aux: _sync_aux_t; // data structure for locking, signaling, etc.
  // Ideally, the definition of this class should be target and base_type dependent,
  // since not all targets need to have a sync_aux field if base_type is sufficiently simple.

  def ~_syncvar() { __primitive("destroy_sync_aux", this); }

  def initialize() {
    __primitive("init_sync_aux", this);
    if (isSimpleSyncBaseType(this.base_type)) {
      // The sync_aux field might not be used on some targets!
      __primitive("sync_reset", this);
    }
  }
}

// The operations are:
//  readFE - wait for full, leave empty
//  readFF - wait for full, leave full
//  readXX - ignore F/E, leave F/E unchanged
//  = (i.e., write_wait_empty_leave_full)
//  writeEF  - wait for empty, leave full (same as =)
//  writeFF  - wait for full, leave full
//  writeXF  - ignore F/E, leave full
//  reset - ignore F/E, write zero, leave empty
//  isFull - query whether it is full

// This is the default read on sync vars. Wait for full, set and signal empty.
def _syncvar.readFE(): base_type {
  var ret: base_type;
  on this {
    var localRet: base_type;
    if isSimpleSyncBaseType(base_type) then
      localRet = __primitive("read_FE", localRet, this);
    else {
      __primitive("sync_wait_full_and_lock", this);
      localRet = value;
      __primitive("sync_mark_and_signal_empty", this);
    }
    ret = localRet;
  }
  return ret;
}

// Wait for full, set and signal full.
def _syncvar.readFF() {
  var ret: base_type;
  on this {
    var localRet: base_type;
    if isSimpleSyncBaseType(base_type) then
      localRet = __primitive("read_FF", localRet, this);
    else {
      __primitive("sync_wait_full_and_lock", this);
      localRet = value;
      __primitive("sync_mark_and_signal_full", this); // in case others are waiting
    }
    ret = localRet;
  }
  return ret;
}

// Ignore F/E.  Read value.  No state change or signals.
def _syncvar.readXX() {
  var ret: base_type;
  on this {
    var localRet: base_type;
    if isSimpleSyncBaseType(base_type) then
      localRet = __primitive("read_XX", localRet, this);
    else {
      __primitive("sync_lock", this);
      localRet = value;
      __primitive("sync_unlock", this);
    }
    ret = localRet;
  }
  return ret;
}

// This is the default write on sync vars. Wait for empty, set and signal full.
def _syncvar.writeEF(val:base_type) {
  on this {
    if isSimpleSyncBaseType(base_type) then
      __primitive("write_EF", this, val);
    else {
      __primitive("sync_wait_empty_and_lock", this);
      value = val;
      __primitive("sync_mark_and_signal_full", this);
    }
  }
}

def =(sv: sync, val:sv.base_type) {
  sv.writeEF(val);
}

// Wait for full, set and signal full.
def _syncvar.writeFF(val:base_type) {
  on this {
    if isSimpleSyncBaseType(base_type) then
      __primitive("write_FF", this, val);
    else {
      __primitive("sync_wait_full_and_lock", this);
      value = val;
      __primitive("sync_mark_and_signal_full", this);
    }
  }
}

// Ignore F/E, set and signal full.
def _syncvar.writeXF(val:base_type) {
  on this {
    if isSimpleSyncBaseType(base_type) then
      __primitive("write_XF", this, val);
    else {
      __primitive("sync_lock", this);
      value = val;
      __primitive("sync_mark_and_signal_full", this);
    }
  }
}

// Ignore F/E, set to zero or default value and signal empty.
def _syncvar.reset() {
  on this {
    if isSimpleSyncBaseType(base_type) then
      // Reset this's value to zero.
      __primitive("sync_reset", this);
    else {
      const default_value: base_type;
      __primitive("sync_lock", this);
      value = default_value;
      __primitive("sync_mark_and_signal_empty", this);
    }
  }
}

def _syncvar.isFull {
  var b: bool;
  on this {
    b = __primitive("sync_is_full", this, isSimpleSyncBaseType(base_type));
  }
  return b;
}


// single variable support
pragma "sync"
pragma "single"
pragma "no default functions"
pragma "no object"
class _singlevar {
  type base_type;
  var  value: base_type;     // actual data - may need to be declared specially on some targets!
  pragma "omit from constructor" var single_aux: _single_aux_t; // data structure for locking, signaling, etc.
  // Ideally, the definition of this class should be target and base_type dependent,
  // since not all targets need to have a single_aux field if base_type is sufficiently simple.

  def ~_singlevar() { __primitive("destroy_single_aux", this); }

  def initialize() {
    __primitive("init_single_aux", this);
    if (isSimpleSyncBaseType(this.base_type)) {
      // The single_aux field might not be used on some targets!
      __primitive("single_reset", this);  // No locking or unlocking done here!
    }
  }
}

// Wait for full. Set and signal full.
def _singlevar.readFF() {
  var ret: base_type;
  on this {
    var localRet: base_type;
    if isSimpleSyncBaseType(base_type) then
      localRet = __primitive("single_read_FF", localRet, this);
    else if this.isFull then
      localRet = value;
    else {
      __primitive("single_wait_full", this);
      localRet = value;
      __primitive("single_mark_and_signal_full", this); // in case others are waiting
    }
    ret = localRet;
  }
  return ret;
}


// Ignore F/E.  Read value.  No state change or signals.
def _singlevar.readXX() {
  var ret: base_type;
  on this {
    var localRet: base_type;
    if isSimpleSyncBaseType(base_type) then
      localRet = __primitive("single_read_XX", localRet, this);
    else if this.isFull then
      localRet = value;
    else {
      __primitive("single_lock", this);
      localRet = value;
      __primitive("single_unlock", this);
    }
    ret = localRet;
  }
  return ret;
}


// Can only write once.  Otherwise, it is an error.
def _singlevar.writeEF(val:base_type) {
  on this {
    if isSimpleSyncBaseType(base_type) then
      __primitive("single_write_EF", this, val);
    else {
      __primitive("single_lock", this);
      if this.isFull then
        halt("single var already defined");
      value = val;
      __primitive("single_mark_and_signal_full", this);
    }
  }
}

def =(sv: single, value:sv.base_type) {
  sv.writeEF(value);
}

def _singlevar.isFull {
  var b: bool;
  on this {
    b = __primitive("single_is_full", this, isSimpleSyncBaseType(base_type));
  }
  return b;
}

//
// data structures for naive implementation of end
//

pragma "no default functions"
class _EndCount {
  var i: sync int(64) = 0,
      b: sync bool = true,
      taskList: _task_list = _nullTaskList;
}

pragma "dont disable remote value forwarding"
def _endCountAlloc() return new _EndCount();

pragma "dont disable remote value forwarding"
def _endCountFree(e: _EndCount) {
  delete e;
}

pragma "dont disable remote value forwarding"
def _upEndCount(e: _EndCount) {
  on e {
    _upEndCountInternal(e);
    pragma "dont disable remote value forwarding" pragma "inline"
    def _upEndCountInternal(e: _EndCount) {
      //
      // By hiding this code in its own function, remote value
      // forwarding can be applied to the reference 'e' in order to
      // pass the class to the on-statement rather than a reference to
      // it.  Since the class is local, we avoid remote accesses in
      // this case.  Remote value forwarding is disabled otherwise
      // because of the sync variable accesses within this function.
      //
      var i = e.i;
      if i == 0 then
        e.b.reset();
      e.i = i + 1;
    }
  }
}

pragma "dont disable remote value forwarding"
def _downEndCount(e: _EndCount) {
  on e {
    _downEndCountInternal(e);
    // This function seems unnecessary; sse comment in _upEndCountInternal.
    pragma "dont disable remote value forwarding" pragma "inline"
    def _downEndCountInternal(e: _EndCount) {
      var i = e.i;
      if i == 1 then
        e.b = true;
      e.i = i - 1;
    }
  }
}

pragma "dont disable remote value forwarding"
def _waitEndCount(e: _EndCount) {
  __primitive("execute tasks in list", e.taskList);
  // First wait for the signal to be set.
  e.b.readFE();
  // Then wait for e.i to be updated because we may free the end count
  // class next; note that e.b needs to be signaled when e.i is in an
  // empty state in cases where the upEndCount and downEndCount calls
  // alternate.
  e.i;
  __primitive("free task list", e.taskList);
}

def _upEndCount() {
  var e = __primitive("get end count");
  _upEndCount(e);
}

def _downEndCount() {
  var e = __primitive("get end count");
  _downEndCount(e);
}

def _waitEndCount() {
  var e = __primitive("get end count");
  _waitEndCount(e);
}

//
// casts
//
def chpl__isType(type t) param return true;
def chpl__isType(e) param return false;

def _isPrimitiveType(type t) param return
  (t == bool) | (t == bool(8)) | (t == bool(16)) | (t == bool(32)) | (t == bool(64)) |
  (t == int(8)) | (t == int(16)) | (t == int(32)) | (t == int(64)) |
  (t == uint(8)) | (t == uint(16)) | (t == uint(32)) | (t == uint(64)) |
  (t == real(32)) | (t == real(64)) |
  (t == string);

def _isSimpleScalarType(type t) param return
  _isBooleanType(t) | _isIntegralType(t) | _isFloatType(t);

def _isBooleanType(type t) param return
  (t == bool) | (t == bool(8)) | (t == bool(16)) | (t == bool(32)) | (t == bool(64));

def _isIntegralType(type t) param return
  _isSignedType(t) || _isUnsignedType(t);

def _isSignedType(type t) param return
  (t == int(8)) || (t == int(16)) || (t == int(32)) || (t == int(64));


def _isUnsignedType(type t) param return
  (t == uint(8)) || (t == uint(16)) || (t == uint(32)) || (t == uint(64));

def _isEnumeratedType(type t) param {
  def isEnum(type t: enumerated) param return true;
  def isEnum(type t) param return false;
  return isEnum(t);
}

def _isComplexType(type t) param return
  (t == complex(64)) | (t == complex(128));

def _isFloatType(type t) param return
  (t == real(32)) | (t == real(64)) |
  (t == imag(32)) | (t == imag(64));

def _isRealType(type t) param return
  (t == real(32)) | (t == real(64));

def _isImagType(type t) param return
  (t == imag(32)) | (t == imag(64));

def chpl__idxTypeToStrType(type t) type {
  if (t == uint(8)) {
    return int(8);
  } else if (t == uint(16)) {
    return int(16);
  } else if (t == uint(32)) {
    return int(32);
  } else if (t == uint(64)) {
    return int(64);
  } else if (t == int(8) || t == int(16) || t == int(32) || t == int(64)) {
    return t;
  } else {
    compilerError("range idxType is non-integral: ", typeToString(t));
  }
}

pragma "command line setting"
def _command_line_cast(param s: string, type t, x) return _cast(t, x);

pragma "inline" def _cast(type t, x: bool) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: bool(8)) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: bool(16)) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: bool(32)) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: bool(64)) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: int(?w)) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: uint(?w)) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: chpl_taskID_t) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: real(?w)) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: string) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: enumerated) where _isPrimitiveType(t) && t != string
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x) where t:object && x:t
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x) where t:object && x:_nilType
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x) where x:object && t:x && (x.type != t)
  return __primitive("dynamic_cast", t, x);

pragma "inline" def _cast(type t, x:_nilType) where t == _nilType
  return nil;

//
// casts to complex
//
pragma "inline" def _cast(type t, x: bool) where _isComplexType(t)
  return (x, 0):t;

pragma "inline" def _cast(type t, x: int(?w)) where _isComplexType(t)
  return (x, 0):t;

pragma "inline" def _cast(type t, x: uint(?w)) where _isComplexType(t)
  return (x, 0):t;

pragma "inline" def _cast(type t, x: real(?w)) where _isComplexType(t)
  return (x, 0):t;

pragma "inline" def _cast(type t, x: imag(?w)) where _isComplexType(t)
  return (0, _i2r(x)):t;

pragma "inline" def _cast(type t, x: complex(?w)) where _isComplexType(t)
  return (x.re, x.im):t;

pragma "inline" def _cast(type t, x: string) where _isComplexType(t)
  return __primitive("cast", t, x);

//
// casts to imag
//
pragma "inline" def _cast(type t, x: bool) where _isImagType(t)
  return if x then 1i:t else 0i:t;

pragma "inline" def _cast(type t, x: int(?w)) where _isImagType(t)
  return 0i:t;

pragma "inline" def _cast(type t, x: uint(?w)) where _isImagType(t)
  return 0i:t;

pragma "inline" def _cast(type t, x: real(?w)) where _isImagType(t)
  return 0i:t;

pragma "inline" def _cast(type t, x: imag(?w)) where _isImagType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: complex(?w)) where _isImagType(t)
  return let xim = x.im in __primitive("cast", t, xim);

pragma "inline" def _cast(type t, x: string) where _isImagType(t)
  return __primitive("cast", t, x);

//
// casts from complex
//
pragma "inline" def _cast(type t, x: complex(?w)) where t == string {
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

pragma "inline" def _cast(type t, x: complex(?w)) where _isRealType(t) || _isIntegralType(t) {
  var y: t;
  y = x.re:t;
  return y;
}

//
// casts from imag
//
pragma "inline" def _cast(type t, x: imag(?w)) where t == string
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: imag(?w)) where _isRealType(t) || _isIntegralType(t)
  return 0:t;

pragma "inline" def _cast(type t, x: imag(?w)) where _isBooleanType(t)
  return if x != 0i then true else false;


//
// Default swap operator for most types
//
pragma "inline" def _chpl_swap(inout x, inout y) {
  const t = y;
  y = x;
  x = t;
}

pragma "inline" def chpl__typeAliasInit(type t) type return t;
pragma "inline" def chpl__typeAliasInit(v) {
  compilerError("illegal assignment of value to type");
}

pragma "dont disable remote value forwarding"
pragma "inline" def _createFieldDefault(type t, init) {
  pragma "no auto destroy" var x: t;
  x = init;
  return x;
}

pragma "dont disable remote value forwarding"
pragma "inline" def _createFieldDefault(type t, param init) {
  pragma "no auto destroy" var x: t;
  x = init;
  return x;
}

pragma "dont disable remote value forwarding"
pragma "inline" def _createFieldDefault(type t, init: _nilType) {
  pragma "no auto destroy" var x: t;
  return x;
}

pragma "dont disable remote value forwarding"
pragma "inline" def _createFieldDefault(type t, init: sync) {
  return init;
}

pragma "dont disable remote value forwarding"
pragma "inline" def _createFieldDefault(type t, init: single) {
  return init;
}

pragma "inline" def chpl__initCopy(a) {
  if a.type == string then
    return __primitive("string_copy", a);
  else
    return a;
}

pragma "inline" def chpl__initCopy(type t) {
  compilerError("illegal assignment of type to value");
}

pragma "inline" pragma "ref"
def chpl__initCopy(r: _ref) return chpl__initCopy(__primitive("get ref", r));

pragma "inline" def chpl__initCopy(sv: sync) {
  return sv.readFE();
}

pragma "inline" def chpl__initCopy(sv: single) {
  return sv.readFF();
}

pragma "inline" def chpl__initCopy(x: _tuple) { 
  // body inserted during generic instantiation
}

pragma "dont disable remote value forwarding"
pragma "removable auto copy" def chpl__autoCopy(x: _distribution) {
  if x._value then
    on x._value do x._value._distCnt$ += 1;
  return x;
}

pragma "dont disable remote value forwarding"
pragma "removable auto copy" def chpl__autoCopy(x: domain) {
  on x._value do x._value._domCnt$ += 1;
  return x;
}

pragma "dont disable remote value forwarding"
pragma "removable auto copy" def chpl__autoCopy(x: []) {
  on x._value do x._value._arrCnt$ += 1;
  return x;
}

pragma "dont disable remote value forwarding"
pragma "inline"
def chpl__autoCopy(x: sync) return x;

pragma "dont disable remote value forwarding"
pragma "inline"
def chpl__autoCopy(x: single) return x;

pragma "inline"
def chpl__autoCopy(x: _tuple) {
  // body inserted during generic instantiation
}

pragma "inline" def chpl__autoCopy(ir: _iteratorRecord) {
  // body modified during call destructors pass
  return ir;
}

pragma "inline" def chpl__autoCopy(x) return chpl__initCopy(x);

pragma "inline" pragma "ref" def chpl__autoCopy(r: _ref) var return r;

pragma "inline" def chpl__autoCopy(type t) type return t;

pragma "inline" def chpl__maybeAutoDestroyed(x) param
  return !(_isPrimitiveType(x.type) ||
           _isImagType(x.type) ||
           _isComplexType(x.type));
pragma "inline" def chpl__maybeAutoDestroyed(x: enumerated) param return false;
pragma "inline" def chpl__maybeAutoDestroyed(x: object) param return false;

pragma "inline" def chpl__autoDestroy(x: object) { }
pragma "inline" def chpl__autoDestroy(type t)  { }
pragma "inline" def chpl__autoDestroy(x: ?t) {
  __primitive("call destructor", x);
}
pragma "inline" def chpl__autoDestroy(ir: _iteratorRecord) {
  // body inserted during call destructors pass
}
pragma "dont disable remote value forwarding"
pragma "removable auto destroy" def chpl__autoDestroy(x: _distribution) {
  __primitive("call destructor", x);
}
pragma "dont disable remote value forwarding"
pragma "removable auto destroy" def chpl__autoDestroy(x: domain) {
  __primitive("call destructor", x);
}
pragma "dont disable remote value forwarding"
pragma "removable auto destroy" def chpl__autoDestroy(x: []) {
  __primitive("call destructor", x);
}
pragma "inline" def chpl__autoDestroy(x: _syncvar) {
  delete x;
}
pragma "inline" def chpl__autoDestroy(x: _singlevar) {
  delete x;
}

// Type function for representing function types
// We use the fully expanded versions because the automatically expanded ones use temporaries in places that cause us trouble
/*
def func(type t...?n) type {
  return __primitive("create fn type", (...t));
}
*/
def func() type {
  return __primitive("create fn type", void);
}
def func(type t1) type { 
  return __primitive("create fn type", t1);
}
def func(type t1, type t2) type { 
  return __primitive("create fn type", t1, t2);
}
def func(type t1, type t2, type t3) type { 
  return __primitive("create fn type", t1, t2, t3);
}
def func(type t1, type t2, type t3, type t4) type { 
  return __primitive("create fn type", t1, t2, t3, t4);
}
def func(type t1, type t2, type t3, type t4, type t5) type { 
  return __primitive("create fn type", t1, t2, t3, t4, t5);
}
def func(type t1, type t2, type t3, type t4, type t5, type t6) type { 
  return __primitive("create fn type", t1, t2, t3, t4, t5, t6);
}
def func(type t1, type t2, type t3, type t4, type t5, type t6, type t7) type { 
  return __primitive("create fn type", t1, t2, t3, t4, t5, t6, t7);
}
def func(type t1, type t2, type t3, type t4, type t5, type t6, type t7, type t8) type { 
  return __primitive("create fn type", t1, t2, t3, t4, t5, t6, t7, t8);
}
def func(type t1, type t2, type t3, type t4, type t5, type t6, type t7, type t8, type t9) type { 
  return __primitive("create fn type", t1, t2, t3, t4, t5, t6, t7, t8, t9);
}
def func(type t1, type t2, type t3, type t4, type t5, type t6, type t7, type t8, type t9, type t10) type { 
  return __primitive("create fn type", t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
}
def func(type t1, type t2, type t3, type t4, type t5, type t6, type t7, type t8, type t9, type t10, type t11) type { 
  return __primitive("create fn type", t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11);
}
def func(type t1, type t2, type t3, type t4, type t5, type t6, type t7, type t8, type t9, type t10, type t11, type t12) type { 
  return __primitive("create fn type", t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12);
}
def func(type t1, type t2, type t3, type t4, type t5, type t6, type t7, type t8, type t9, type t10, type t11, type t12, type t13) type { 
  return __primitive("create fn type", t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13);
}
def func(type t1, type t2, type t3, type t4, type t5, type t6, type t7, type t8, type t9, type t10, type t11, type t12, type t13, type t14) type { 
  return __primitive("create fn type", t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14);
}
def func(type t1, type t2, type t3, type t4, type t5, type t6, type t7, type t8, type t9, type t10, type t11, type t12, type t13, type t14, type t15) type { 
  return __primitive("create fn type", t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);
}

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
def chpldev_refToString(inout ref) {

  //
  // print out the address of class references as well
  //
  def chpldev_classToString(x: object)
    return " (class = " + __primitive("ref to string", x) + ")";
  def chpldev_classToString(x) return "";

  return __primitive("ref to string", ref) + chpldev_classToString(ref);
}


// Everything below this comment was generated by the program
// $CHPL_HOME/util/gen_int_uint64_operators.chpl.  If something below this
// comment needs to change, please update that file and regenerate these operators


// non-param/non-param
pragma "inline" def +(a: uint(64), b: int(64)) { _throwOpError("+"); }
pragma "inline" def +(a: int(64), b: uint(64)) { _throwOpError("+"); }

// param/param
pragma "inline" def +(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("+"); else return a + b:uint(64);
}
pragma "inline" def +(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("+"); else return a:uint(64) + b;
}

// non-param/param and param/non-param
pragma "inline" def +(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("+"); else return a + b:uint(64);
}
pragma "inline" def +(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("+"); else return a:uint(64) + b;
}


// non-param/non-param
pragma "inline" def -(a: uint(64), b: int(64)) { _throwOpError("-"); }
pragma "inline" def -(a: int(64), b: uint(64)) { _throwOpError("-"); }

// param/param
pragma "inline" def -(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("-"); else return a - b:uint(64);
}
pragma "inline" def -(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("-"); else return a:uint(64) - b;
}

// non-param/param and param/non-param
pragma "inline" def -(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("-"); else return a - b:uint(64);
}
pragma "inline" def -(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("-"); else return a:uint(64) - b;
}


// non-param/non-param
pragma "inline" def *(a: uint(64), b: int(64)) { _throwOpError("*"); }
pragma "inline" def *(a: int(64), b: uint(64)) { _throwOpError("*"); }

// param/param
pragma "inline" def *(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("*"); else return a * b:uint(64);
}
pragma "inline" def *(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("*"); else return a:uint(64) * b;
}

// non-param/param and param/non-param
pragma "inline" def *(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("*"); else return a * b:uint(64);
}
pragma "inline" def *(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("*"); else return a:uint(64) * b;
}


// non-param/non-param
pragma "inline" def /(a: uint(64), b: int(64)) { _throwOpError("/"); }
pragma "inline" def /(a: int(64), b: uint(64)) { _throwOpError("/"); }

// param/param
pragma "inline" def /(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("/"); else return a / b:uint(64);
}
pragma "inline" def /(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("/"); else return a:uint(64) / b;
}

// non-param/param and param/non-param
pragma "inline" def /(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("/"); else return a / b:uint(64);
}
pragma "inline" def /(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("/"); else return a:uint(64) / b;
}


// non-param/non-param
pragma "inline" def **(a: uint(64), b: int(64)) { _throwOpError("**"); }
pragma "inline" def **(a: int(64), b: uint(64)) { _throwOpError("**"); }

// param/param
pragma "inline" def **(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("**"); else return a ** b:uint(64);
}
pragma "inline" def **(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("**"); else return a:uint(64) ** b;
}

// non-param/param and param/non-param
pragma "inline" def **(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("**"); else return a ** b:uint(64);
}
pragma "inline" def **(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("**"); else return a:uint(64) ** b;
}


// non-param/non-param
pragma "inline" def %(a: uint(64), b: int(64)) { _throwOpError("%"); }
pragma "inline" def %(a: int(64), b: uint(64)) { _throwOpError("%"); }

// param/param
pragma "inline" def %(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("%"); else return a % b:uint(64);
}
pragma "inline" def %(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("%"); else return a:uint(64) % b;
}

// non-param/param and param/non-param
pragma "inline" def %(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("%"); else return a % b:uint(64);
}
pragma "inline" def %(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("%"); else return a:uint(64) % b;
}


// non-param/non-param
pragma "inline" def ==(a: uint(64), b: int(64)) { _throwOpError("=="); }
pragma "inline" def ==(a: int(64), b: uint(64)) { _throwOpError("=="); }

// param/param
pragma "inline" def ==(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("=="); else return a == b:uint(64);
}
pragma "inline" def ==(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("=="); else return a:uint(64) == b;
}

// non-param/param and param/non-param
pragma "inline" def ==(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("=="); else return a == b:uint(64);
}
pragma "inline" def ==(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("=="); else return a:uint(64) == b;
}


// non-param/non-param
pragma "inline" def !=(a: uint(64), b: int(64)) { _throwOpError("!="); }
pragma "inline" def !=(a: int(64), b: uint(64)) { _throwOpError("!="); }

// param/param
pragma "inline" def !=(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("!="); else return a != b:uint(64);
}
pragma "inline" def !=(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("!="); else return a:uint(64) != b;
}

// non-param/param and param/non-param
pragma "inline" def !=(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("!="); else return a != b:uint(64);
}
pragma "inline" def !=(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("!="); else return a:uint(64) != b;
}


// non-param/non-param
pragma "inline" def >(a: uint(64), b: int(64)) { _throwOpError(">"); }
pragma "inline" def >(a: int(64), b: uint(64)) { _throwOpError(">"); }

// param/param
pragma "inline" def >(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError(">"); else return a > b:uint(64);
}
pragma "inline" def >(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError(">"); else return a:uint(64) > b;
}

// non-param/param and param/non-param
pragma "inline" def >(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError(">"); else return a > b:uint(64);
}
pragma "inline" def >(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError(">"); else return if a == 0 then false else a:uint(64) > b;
}


// non-param/non-param
pragma "inline" def <(a: uint(64), b: int(64)) { _throwOpError("<"); }
pragma "inline" def <(a: int(64), b: uint(64)) { _throwOpError("<"); }

// param/param
pragma "inline" def <(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("<"); else return a < b:uint(64);
}
pragma "inline" def <(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("<"); else return a:uint(64) < b;
}

// non-param/param and param/non-param
pragma "inline" def <(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("<"); else return if b == 0 then false else a < b:uint(64);
}
pragma "inline" def <(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("<"); else return a:uint(64) < b;
}


// non-param/non-param
pragma "inline" def >=(a: uint(64), b: int(64)) { _throwOpError(">="); }
pragma "inline" def >=(a: int(64), b: uint(64)) { _throwOpError(">="); }

// param/param
pragma "inline" def >=(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError(">="); else return a >= b:uint(64);
}
pragma "inline" def >=(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError(">="); else return a:uint(64) >= b;
}

// non-param/param and param/non-param
pragma "inline" def >=(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError(">="); else return if b == 0 then true else a >= b:uint(64);
}
pragma "inline" def >=(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError(">="); else return a:uint(64) >= b;
}


// non-param/non-param
pragma "inline" def <=(a: uint(64), b: int(64)) { _throwOpError("<="); }
pragma "inline" def <=(a: int(64), b: uint(64)) { _throwOpError("<="); }

// param/param
pragma "inline" def <=(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("<="); else return a <= b:uint(64);
}
pragma "inline" def <=(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("<="); else return a:uint(64) <= b;
}

// non-param/param and param/non-param
pragma "inline" def <=(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("<="); else return a <= b:uint(64);
}
pragma "inline" def <=(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("<="); else return if a == 0 then true else a:uint(64) <= b;
}



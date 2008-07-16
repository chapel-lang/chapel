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

config param CHPL_THREADS: string = "unset";
if (CHPL_THREADS == "unset") {
  compilerWarning("CHPL_THREADS not set");
}

config param CHPL_COMM: string = "unset";
if (CHPL_COMM == "unset") {
  compilerWarning("CHPL_COMM not set");
}


// the number of locales on which to run the program
config const numLocales: int = __primitive("_chpl_comm_default_num_locales");

// the maximum number of threads that can be live at any given time
config const maxThreads: int = __primitive("chpl_maxThreads");
const maxThreadsLimit: int = __primitive("chpl_maxThreadsLimit");
if maxThreadsLimit != 0 {
  if (maxThreads > maxThreadsLimit) then
    __primitive("chpl_warning",
                "specified value of " + maxThreads
                  + " for maxThreads is too high; limit is " + maxThreadsLimit);
  else if (maxThreads == 0) then
    __primitive("chpl_warning",
                "maxThreads is unbounded; however, the limit is " + maxThreadsLimit);
}

def _throwOpError(param op: string) {
    compilerError("illegal use of '", op, "' on operands of type uint(64) and signed integer");
}

//
// assignment on primitive types
//
pragma "inline" def =(a: bool, b: bool) return b;
pragma "inline" def =(a: int(?w), b: int(w)) return b;
pragma "inline" def =(a: uint(?w), b: uint(w)) return b;
pragma "inline" def =(a: real(?w), b: real(w)) return b;
pragma "inline" def =(a: imag(?w), b: imag(w)) return b;
pragma "inline" def =(a: complex(?w), b: complex(w)) return b;
pragma "inline" def =(a: string, b: string) return __primitive("string_copy", b);
pragma "inline" def =(a, b) return b;

//
// equality comparison on primitive types
//
pragma "inline" def ==(a: bool, b: bool) return __primitive("==", a, b);
pragma "inline" def ==(a: int(32), b: int(32)) return __primitive("==", a, b);
pragma "inline" def ==(a: int(64), b: int(64)) return __primitive("==", a, b);
pragma "inline" def ==(a: uint(32), b: uint(32)) return __primitive("==", a, b);
pragma "inline" def ==(a: uint(64), b: uint(64)) return __primitive("==", a, b);
pragma "inline" def ==(a: real(?w), b: real(w)) return __primitive("==", a, b);
pragma "inline" def ==(a: string, b: string) return (__primitive("_string_compare", a, b) == 0);
pragma "inline" def ==(a: object, b: object) return __primitive("ptr_eq", a, b);

pragma "inline" def !=(a: bool, b: bool) return __primitive("!=", a, b);
pragma "inline" def !=(a: int(32), b: int(32)) return __primitive("!=", a, b);
pragma "inline" def !=(a: int(64), b: int(64)) return __primitive("!=", a, b);
pragma "inline" def !=(a: uint(32), b: uint(32)) return __primitive("!=", a, b);
pragma "inline" def !=(a: uint(64), b: uint(64)) return __primitive("!=", a, b);
pragma "inline" def !=(a: real(?w), b: real(w)) return __primitive("!=", a, b);
pragma "inline" def !=(a: string, b: string) return (__primitive("_string_compare", a, b) != 0);
pragma "inline" def !=(a: object, b: object) return __primitive("ptr_neq", a, b);

pragma "inline" def ==(param a: bool, param b: bool) param return __primitive("==", a, b);
pragma "inline" def ==(param a: int(32), param b: int(32)) param return __primitive("==", a, b);
pragma "inline" def ==(param a: int(64), param b: int(64)) param return __primitive("==", a, b);
pragma "inline" def ==(param a: uint(32), param b: uint(32)) param return __primitive("==", a, b);
pragma "inline" def ==(param a: uint(64), param b: uint(64)) param return __primitive("==", a, b);
pragma "inline" def ==(param a: enumerated, param b: enumerated) param return __primitive("==", a, b);
pragma "inline" def ==(param a: string, param b: string) param return __primitive("_string_compare", a, b) == 0;

pragma "inline" def !=(param a: bool, param b: bool) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: int(32), param b: int(32)) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: int(64), param b: int(64)) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: uint(32), param b: uint(32)) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: uint(64), param b: uint(64)) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: enumerated, param b: enumerated) param return __primitive("!=", a, b);
pragma "inline" def !=(param a: string, param b: string) param return __primitive("_string_compare", a, b) != 0;

//
// ordered comparison on primitive types
//
pragma "inline" def <=(a: int(32), b: int(32)) return __primitive("<=", a, b);
pragma "inline" def <=(a: int(64), b: int(64)) return __primitive("<=", a, b);
pragma "inline" def <=(a: uint(32), b: uint(32)) return __primitive("<=", a, b);
pragma "inline" def <=(a: uint(64), b: uint(64)) return __primitive("<=", a, b);
pragma "inline" def <=(a: real(?w), b: real(w)) return __primitive("<=", a, b);
pragma "inline" def <=(a: string, b: string) return (__primitive("_string_compare", a, b) <= 0);

pragma "inline" def >=(a: int(32), b: int(32)) return __primitive(">=", a, b);
pragma "inline" def >=(a: int(64), b: int(64)) return __primitive(">=", a, b);
pragma "inline" def >=(a: uint(32), b: uint(32)) return __primitive(">=", a, b);
pragma "inline" def >=(a: uint(64), b: uint(64)) return __primitive(">=", a, b);
pragma "inline" def >=(a: real(?w), b: real(w)) return __primitive(">=", a, b);
pragma "inline" def >=(a: string, b: string) return (__primitive("_string_compare", a, b) >= 0);

pragma "inline" def <(a: int(32), b: int(32)) return __primitive("<", a, b);
pragma "inline" def <(a: int(64), b: int(64)) return __primitive("<", a, b);
pragma "inline" def <(a: uint(32), b: uint(32)) return __primitive("<", a, b);
pragma "inline" def <(a: uint(64), b: uint(64)) return __primitive("<", a, b);
pragma "inline" def <(a: real(?w), b: real(w)) return __primitive("<", a, b);
pragma "inline" def <(a: string, b: string) return (__primitive("_string_compare", a, b) < 0);

pragma "inline" def >(a: int(32), b: int(32)) return __primitive(">", a, b);
pragma "inline" def >(a: int(64), b: int(64)) return __primitive(">", a, b);
pragma "inline" def >(a: uint(32), b: uint(32)) return __primitive(">", a, b);
pragma "inline" def >(a: uint(64), b: uint(64)) return __primitive(">", a, b);
pragma "inline" def >(a: real(?w), b: real(w)) return __primitive(">", a, b);
pragma "inline" def >(a: string, b: string) return (__primitive("_string_compare", a, b) > 0);

pragma "inline" def <=(param a: int(32), param b: int(32)) param return __primitive("<=", a, b);
pragma "inline" def <=(param a: int(64), param b: int(64)) param return __primitive("<=", a, b);
pragma "inline" def <=(param a: uint(32), param b: uint(32)) param return __primitive("<=", a, b);
pragma "inline" def <=(param a: uint(64), param b: uint(64)) param return __primitive("<=", a, b);
pragma "inline" def <=(param a: enumerated, param b: enumerated) param return __primitive("<=", a, b);
pragma "inline" def <=(param a: string, param b: string) param return __primitive("_string_compare", a, b) <= 0;

pragma "inline" def >=(param a: int(32), param b: int(32)) param return __primitive(">=", a, b);
pragma "inline" def >=(param a: int(64), param b: int(64)) param return __primitive(">=", a, b);
pragma "inline" def >=(param a: uint(32), param b: uint(32)) param return __primitive(">=", a, b);
pragma "inline" def >=(param a: uint(64), param b: uint(64)) param return __primitive(">=", a, b);
pragma "inline" def >=(param a: enumerated, param b: enumerated) param return __primitive(">=", a, b);
pragma "inline" def >=(param a: string, param b: string) param return __primitive("_string_compare", a, b) >= 0;

pragma "inline" def <(param a: int(32), param b: int(32)) param return __primitive("<", a, b);
pragma "inline" def <(param a: int(64), param b: int(64)) param return __primitive("<", a, b);
pragma "inline" def <(param a: uint(32), param b: uint(32)) param return __primitive("<", a, b);
pragma "inline" def <(param a: uint(64), param b: uint(64)) param return __primitive("<", a, b);
pragma "inline" def <(param a: enumerated, param b: enumerated) param return __primitive("<", a, b);
pragma "inline" def <(param a: string, param b: string) param return __primitive("_string_compare", a, b) < 0;

pragma "inline" def >(param a: int(32), param b: int(32)) param return __primitive(">", a, b);
pragma "inline" def >(param a: int(64), param b: int(64)) param return __primitive(">", a, b);
pragma "inline" def >(param a: uint(32), param b: uint(32)) param return __primitive(">", a, b);
pragma "inline" def >(param a: uint(64), param b: uint(64)) param return __primitive(">", a, b);
pragma "inline" def >(param a: enumerated, param b: enumerated) param return __primitive(">", a, b);
pragma "inline" def >(param a: string, param b: string) param return (__primitive("_string_compare", a, b) > 0);

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
pragma "inline" def -(a: uint(64)) { compilerError("illegal use of '-' on operand of type ", a.type); }
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
    compilerError("illegal use of '-' on operand of type ", a.type);
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
pragma "inline" def **(a: int(32), b: int(32)) return __primitive("**", a, b);
pragma "inline" def **(a: int(64), b: int(64)) return __primitive("**", a, b);
pragma "inline" def **(a: uint(32), b: uint(32)) return __primitive("**", a, b);
pragma "inline" def **(a: uint(64), b: uint(64)) return __primitive("**", a, b);
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
// for delete
//
def object.chpl_destroy() { }

//
// These functions are to handle symbols at statement level such as
// the second statement here: var a: sync int = 1; a;
//
pragma "inline" def _statementLevelSymbol(a) { return a; }
pragma "inline" def _statementLevelSymbol(param a) param { return a; }
pragma "inline" def _statementLevelSymbol(type a) type { return a; }

//
// _init on primitive types and classes
//
pragma "inline" def _init(x: bool) return false;
pragma "inline" def _init(x: int(?w)) return 0:int(w);
pragma "inline" def _init(x: uint(?w)) return 0:uint(w);
pragma "inline" def _init(x: real(?w)) return 0.0:real(w);
pragma "inline" def _init(x: imag(?w)) return _r2i(0.0:real(w));
pragma "inline" def _init(x: complex(?w)) return (0.0:real(w/2), 0.0:real(w/2)):complex;
pragma "inline" def _init(x: string) return "";
pragma "inline" def _init(x) return nil:x.type;

//
// _copy on primitive types and classes
//
pragma "inline" def _copy(a) return a;

pragma "inline" def _copy(type t) {
  compilerError("illegal assignment of type to value");
}

pragma "inline" def _pass(a) return _copy(a);

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
  compilerError("type '", x.type, "' used in if or while condition");
}

pragma "inline" def _cond_test(x: _iteratorClass) {
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
def complex.re var return __primitive("complex_get_real", this);
pragma "inline" pragma "ref this"
def complex.im var return __primitive("complex_get_imag", this);

//
// helper functions
//
pragma "inline" def _i2r(a: imag(?w)) return __primitive("cast", real(w), a);
pragma "inline" def _r2i(a: real(?w)) return __primitive("cast", imag(w), a);

//
// primitive string functions and methods
//
pragma "inline" def ascii(a: string) return __primitive("ascii", a);
pragma "inline" def length(a: string) return __primitive("string_length", a);
pragma "inline" def string.substring(i: int) return __primitive("string_index", this, i);
pragma "inline" def _string_contains(a: string, b: string) return __primitive("string_contains", a, b);

pragma "inline" def ascii(param a: string) param return __primitive("ascii", a);
pragma "inline" def length(param a: string) param return __primitive("string_length", a);
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
  __primitive("_chpl_exit_any", status);
}

def init_elts(x, s, type t) {
  for i in 1..s {
    var y: t;
    __primitive("array_set_first", x, i-1, y);
  }
}

// dynamic data block class
pragma "data class"
class _ddata {
  type eltType;
  var size: int;
  pragma "inline" def init() {
    __primitive("array_alloc", this, eltType, size);
    init_elts(this, size, eltType);
  }
  pragma "inline" def init(d: eltType) {
    __primitive("array_alloc", this, eltType, size);
    [i in 0..size-1] __primitive("array_set_first", this, i, d);
  }
  pragma "inline" def this(i: int) var {
    return __primitive("array_get", this, i);
  }
}

//
// heap class for allocating locals on the heap in support of begin-
// and on-statements
//
pragma "heap"
class _heap {
  var _val;
}

pragma "heap" def _heapAlloc(x) return new _heap(x);
pragma "heap" def _heapAccess(x) var return x._val;

pragma "heap" def _heapAllocGlobal(x) return new _heap(x);

pragma "heap" def _heapAllocConstGlobal(x) where _isSimpleScalarType(x.type) return x;
pragma "heap" def _heapAllocConstGlobal(x) where !_isSimpleScalarType(x.type) return new _heap(x);
pragma "heap" def _heapAccessConstGlobal(x) where _isSimpleScalarType(x.type) return x;
pragma "heap" def _heapAccessConstGlobal(x) where !_isSimpleScalarType(x.type) return x._val;

//
// internal reference type
//
pragma "ref"
pragma "no default functions"
pragma "no object"
class _ref {
  var _val;
}

pragma "inline" pragma "ref"
def _init(r: _ref) return _init(__primitive("get ref", r));
pragma "inline" pragma "ref"
def _copy(r: _ref) return _copy(__primitive("get ref", r));
pragma "inline" pragma "ref"
def _pass(r: _ref) var return r;

// Returns whether an object of type t occupies a 64-bit word on Cray's MTA/XMT
// (The definition of this function should be target dependent.  This would avoid
// the need to write C macros in the runtime that essentially duplicate
// the functionality of the read/write methods of the _syncvar and _singlevar classes
// for targets that don't have particularly fast ways of achieving this functionality
// for simple base types.)
def isSimpleSyncBaseType (type t) param {
  if CHPL_THREADS == "mta" then
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

  def initialize() {
    __primitive("init_sync_aux", this);
    if (isSimpleSyncBaseType(this.base_type)) {
      // The sync_aux field might not be used on some targets!
      __primitive("sync_reset", this);
    }
  }
}

def _copy(sv: sync) {
  return sv.readFE();
}

def _pass(sv: sync) {
  return sv;
}

def _init(sv: sync) {
  return _syncvar(sv.value.type); 
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
  return sv;
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

  def initialize() {
    __primitive("init_single_aux", this);
    if (isSimpleSyncBaseType(this.base_type)) {
      // The single_aux field might not be used on some targets!
      __primitive("single_reset", this);  // No locking or unlocking done here!
    }
  }
}

def _copy(sv: single) {
  return sv.readFF();
}

def _pass(sv: single) {
  return sv;
}

def _init(sv: single) {
  return _singlevar(sv.value.type); 
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
  return sv;
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

def _endCountAlloc() return new _EndCount();

def _upEndCount(e: _EndCount) {
  var i = e.i;
  if i == 0 then
    e.b.reset();
  e.i = i + 1;
}

def _downEndCount(e: _EndCount) {
  var i = e.i;
  if i == 1 then
    e.b = true;
  e.i = i - 1;
}

def _waitEndCount(e: _EndCount) {
  __primitive("execute tasks in list", e.taskList);
  e.b.readFE();
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
def _isPrimitiveType(type t) param return
  (t == bool) |
  (t == int(8)) | (t == int(16)) | (t == int(32)) | (t == int(64)) |
  (t == uint(8)) | (t == uint(16)) | (t == uint(32)) | (t == uint(64)) |
  (t == real(32)) | (t == real(64)) |
  (t == string);

def _isSimpleScalarType(type t) param return
  (t == bool) | _isIntegralType(t) | _isFloatType(t);

def _isIntegralType(type t) param return
  (t == int(8)) | (t == int(16)) | (t == int(32)) | (t == int(64)) |
  (t == uint(8)) | (t == uint(16)) | (t == uint(32)) | (t == uint(64));

def _isComplexType(type t) param return
  (t == complex(64)) | (t == complex(128));

def _isFloatType(type t) param return
  (t == real(32)) | (t == real(64)) |
  (t == imag(32)) | (t == imag(64));

def _isRealType(type t) param return
  (t == real(32)) | (t == real(64));

def _isImagType(type t) param return
  (t == imag(32)) | (t == imag(64));

pragma "command line setting"
def _command_line_cast(param s: string, type t, x) return _cast(t, x);

pragma "inline" def _cast(type t, x: bool) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: int(?w)) where _isPrimitiveType(t)
  return __primitive("cast", t, x);

pragma "inline" def _cast(type t, x: uint(?w)) where _isPrimitiveType(t)
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

pragma "inline" def _cast(type t, x: imag(?w)) where t == bool
  return if x != 0i then true else false;



pragma "inline" def _startTrackingMem() { __primitive("startTrackingMem"); }

//
// Default swap operator for most types
//
pragma "inline" def _chpl_swap(inout x, inout y) {
  const t = y;
  y = x;
  x = t;
}

pragma "inline" def _createFieldDefault(type t, init) {
  var x: t;
  x = init;
  return x;
}

pragma "inline" def _createFieldDefault(type t, param init) {
  var x: t;
  x = init;
  return x;
}

pragma "inline" def _createFieldDefault(type t, init: _nilType) {
  var x: t;
  return x;
}

pragma "inline" def _createFieldDefault(type t, init: sync) {
  return init;
}

pragma "inline" def _createFieldDefault(type t, init: single) {
  return init;
}

// Everything below this comment was generated by the program
// $CHPL_HOME/util/gen_int_uint64_operators.chpl.  If something below this
// comment needs to change, please update that file and regenerate these operators

// non-param/non-param
pragma "inline" def +(a: uint(64), b: int(64)) { _throwOpError("+"); }
pragma "inline" def +(a: int(64), b: uint(64)) { _throwOpError("+"); }

// param/param
pragma "inline" def +(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("+"); else return __primitive("+", a, b: uint(64));
}
pragma "inline" def +(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("+"); else return __primitive("+", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def +(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("+"); else return __primitive("+", a, b:uint(64));
}
pragma "inline" def +(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("+"); else return __primitive("+", a:uint(64), b);
}


// non-param/non-param
pragma "inline" def -(a: uint(64), b: int(64)) { _throwOpError("-"); }
pragma "inline" def -(a: int(64), b: uint(64)) { _throwOpError("-"); }

// param/param
pragma "inline" def -(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("-"); else return __primitive("-", a, b: uint(64));
}
pragma "inline" def -(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("-"); else return __primitive("-", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def -(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("-"); else return __primitive("-", a, b:uint(64));
}
pragma "inline" def -(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("-"); else return __primitive("-", a:uint(64), b);
}


// non-param/non-param
pragma "inline" def *(a: uint(64), b: int(64)) { _throwOpError("*"); }
pragma "inline" def *(a: int(64), b: uint(64)) { _throwOpError("*"); }

// param/param
pragma "inline" def *(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("*"); else return __primitive("*", a, b: uint(64));
}
pragma "inline" def *(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("*"); else return __primitive("*", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def *(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("*"); else return __primitive("*", a, b:uint(64));
}
pragma "inline" def *(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("*"); else return __primitive("*", a:uint(64), b);
}


// non-param/non-param
pragma "inline" def /(a: uint(64), b: int(64)) { _throwOpError("/"); }
pragma "inline" def /(a: int(64), b: uint(64)) { _throwOpError("/"); }

// param/param
pragma "inline" def /(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("/"); else return __primitive("/", a, b: uint(64));
}
pragma "inline" def /(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("/"); else return __primitive("/", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def /(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("/"); else return __primitive("/", a, b:uint(64));
}
pragma "inline" def /(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("/"); else return __primitive("/", a:uint(64), b);
}


// non-param/non-param
pragma "inline" def **(a: uint(64), b: int(64)) { _throwOpError("**"); }
pragma "inline" def **(a: int(64), b: uint(64)) { _throwOpError("**"); }

// param/param
pragma "inline" def **(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("**"); else return __primitive("**", a, b: uint(64));
}
pragma "inline" def **(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("**"); else return __primitive("**", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def **(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("**"); else return __primitive("**", a, b:uint(64));
}
pragma "inline" def **(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("**"); else return __primitive("**", a:uint(64), b);
}


// non-param/non-param
pragma "inline" def %(a: uint(64), b: int(64)) { _throwOpError("%"); }
pragma "inline" def %(a: int(64), b: uint(64)) { _throwOpError("%"); }

// param/param
pragma "inline" def %(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("%"); else return __primitive("%", a, b: uint(64));
}
pragma "inline" def %(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("%"); else return __primitive("%", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def %(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("%"); else return __primitive("%", a, b:uint(64));
}
pragma "inline" def %(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("%"); else return __primitive("%", a:uint(64), b);
}


// non-param/non-param
pragma "inline" def ==(a: uint(64), b: int(64)) { _throwOpError("=="); }
pragma "inline" def ==(a: int(64), b: uint(64)) { _throwOpError("=="); }

// param/param
pragma "inline" def ==(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("=="); else return __primitive("==", a, b: uint(64));
}
pragma "inline" def ==(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("=="); else return __primitive("==", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def ==(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("=="); else return __primitive("==", a, b:uint(64));
}
pragma "inline" def ==(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("=="); else return __primitive("==", a:uint(64), b);
}


// non-param/non-param
pragma "inline" def !=(a: uint(64), b: int(64)) { _throwOpError("!="); }
pragma "inline" def !=(a: int(64), b: uint(64)) { _throwOpError("!="); }

// param/param
pragma "inline" def !=(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("!="); else return __primitive("!=", a, b: uint(64));
}
pragma "inline" def !=(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("!="); else return __primitive("!=", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def !=(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("!="); else return __primitive("!=", a, b:uint(64));
}
pragma "inline" def !=(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("!="); else return __primitive("!=", a:uint(64), b);
}


// non-param/non-param
pragma "inline" def >(a: uint(64), b: int(64)) { _throwOpError(">"); }
pragma "inline" def >(a: int(64), b: uint(64)) { _throwOpError(">"); }

// param/param
pragma "inline" def >(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError(">"); else return __primitive(">", a, b: uint(64));
}
pragma "inline" def >(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError(">"); else return __primitive(">", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def >(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError(">"); else return __primitive(">", a, b:uint(64));
}
pragma "inline" def >(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError(">"); else if a == 0 then return false; else return __primitive(">", a:uint(64), b);
}


// non-param/non-param
pragma "inline" def <(a: uint(64), b: int(64)) { _throwOpError("<"); }
pragma "inline" def <(a: int(64), b: uint(64)) { _throwOpError("<"); }

// param/param
pragma "inline" def <(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("<"); else return __primitive("<", a, b: uint(64));
}
pragma "inline" def <(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("<"); else return __primitive("<", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def <(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("<"); else if b == 0 then return false; else return __primitive("<", a, b:uint(64));
}
pragma "inline" def <(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("<"); else return __primitive("<", a:uint(64), b);
}


// non-param/non-param
pragma "inline" def >=(a: uint(64), b: int(64)) { _throwOpError(">="); }
pragma "inline" def >=(a: int(64), b: uint(64)) { _throwOpError(">="); }

// param/param
pragma "inline" def >=(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError(">="); else return __primitive(">=", a, b: uint(64));
}
pragma "inline" def >=(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError(">="); else return __primitive(">=", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def >=(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError(">="); else if b == 0 then return true; else return __primitive(">=", a, b:uint(64));
}
pragma "inline" def >=(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError(">="); else return __primitive(">=", a:uint(64), b);
}


// non-param/non-param
pragma "inline" def <=(a: uint(64), b: int(64)) { _throwOpError("<="); }
pragma "inline" def <=(a: int(64), b: uint(64)) { _throwOpError("<="); }

// param/param
pragma "inline" def <=(param a: uint(64), param b: int(64)) param {
  if b < 0 then _throwOpError("<="); else return __primitive("<=", a, b: uint(64));
}
pragma "inline" def <=(param a: int(64), param b: uint(64)) param {
  if a < 0 then _throwOpError("<="); else return __primitive("<=", a:uint(64), b);
}

// non-param/param and param/non-param
pragma "inline" def <=(a: uint(64), param b: int(64)) {
  if b < 0 then _throwOpError("<="); else return __primitive("<=", a, b:uint(64));
}
pragma "inline" def <=(param a: int(64), b: uint(64)) {
  if a < 0 then _throwOpError("<="); else if a == 0 then return true; else return __primitive("<=", a:uint(64), b);
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
  return __primitive("ref to string", ref);
}

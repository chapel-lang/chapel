//
// special type functions
//

pragma "inline" def _init(x : float(?w)) return 0:float(w);
pragma "inline" def _init(x: complex(?w)) return (0.0:float(w/2), 0.0:float(w/2)):complex;

//
// complex component methods real and imag
//
//  bug? note: in setter, float argument should be parameterized over
//             complex bit width
//

def complex.real return __primitive( "complex_get_real", this);
def complex.imag return __primitive( "complex_get_imag", this);
def complex.=real(f:float) { __primitive( "complex_set_real", this, f); }
def complex.=imag(f:float) { __primitive( "complex_set_imag", this, f); }

//
// assignment
//

pragma "inline" def =(a: float(?w), b: float(w)) return b;
pragma "inline" def =(a: complex(?w), b: complex(w)) return b;

//
// relational operators
//
//  question: should there be any over imaginary?  over complex?
//

pragma "inline" def ==(a: float(?w), b: float(w)) return __primitive("==", a, b);
pragma "inline" def !=(a: float(?w), b: float(w)) return __primitive("!=", a, b);
pragma "inline" def <=(a: float(?w), b: float(w)) return __primitive("<=", a, b);
pragma "inline" def >=(a: float(?w), b: float(w)) return __primitive(">=", a, b);
pragma "inline" def <(a: float(?w), b: float(w)) return __primitive("<", a, b);
pragma "inline" def >(a: float(?w), b: float(w)) return __primitive(">", a, b);

//
// unary + and -
//

pragma "inline" def +(a: float(?w)) return __primitive("u+", a);
pragma "inline" def -(a: float(?w)) return __primitive("u-", a);
pragma "inline" def +(a: complex(?w)) return a;
pragma "inline" def -(a: complex(?w)) return (-a.real, -a.imag):complex;

//
// binary + and -
//

pragma "inline" def +(a: float(?w), b: float(w)) return __primitive("+", a, b);
pragma "inline" def -(a: float(?w), b: float(w)) return __primitive("-", a, b);
pragma "inline" def +(a: complex(?w), b: complex(w))
  return (a.real+b.real, a.imag+b.imag):complex;
pragma "inline" def -(a: complex(?w), b: complex(w))
  return (a.real-b.real, a.imag-b.imag):complex;

//
// * and /
//

pragma "inline" def *(a: float(?w), b: float(w)) return __primitive("*", a, b);
pragma "inline" def /(a: float(?w), b: float(w)) return __primitive("/", a, b);

pragma "inline" def *(a: complex(?w), b: complex(w))
  return (a.real*b.real-a.imag*b.imag, a.imag*b.real+a.real*b.imag):complex;
pragma "inline" def /(a: complex(?w), b: complex(w))
  return let d = b.real*b.real+b.imag*b.imag in
    ((a.real*b.real+a.imag*b.imag)/d, (a.imag*b.real-a.real*b.imag)/d):complex;

//
// **
//

pragma "inline" def **(a: float(?w), b: float(w)) return __primitive("**", a, b);

//
// miscellaneous functions
//
pragma "inline" def conjg(a: complex(?w))
  return (a.real, -a.imag):complex;

//
// string conversions
//
pragma "inline" def _tostring(x: complex, format: string)
  return __primitive("to_string", format, x);
pragma "inline" def _tostring(x : float, format : string)
  return __primitive("to_string", format, x);

//
// special type functions
//

pragma "inline" def _init(x : float(?w)) return 0.0:float(w);
pragma "inline" def _init(x : imaginary(?w)) return 0.0:imaginary(w);
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
pragma "inline" def =(a: imaginary(?w), b: imaginary(w)) return b;
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
pragma "inline" def +(a: imaginary(?w)) return __primitive("u+", a):imaginary(w);
pragma "inline" def -(a: imaginary(?w)) return __primitive("u-", a):imaginary(w);
pragma "inline" def +(a: complex(?w)) return a;
pragma "inline" def -(a: complex(?w)) return (-a.real, -a.imag):complex;

//
// binary + and -
//

pragma "inline" def _i2f(a: imaginary(?w)) return a:float(w);

pragma "inline" def +(a: float(?w), b: float(w)) return __primitive("+", a, b);
pragma "inline" def +(a: float(?w), b: imaginary(w)) return (a, _i2f(b)):complex;
pragma "inline" def +(a: imaginary(?w), b: float(w)) return (b, _i2f(a)):complex;
pragma "inline" def +(a: float(?w), b: complex(w*2)) return (a+b.real, b.imag):complex;
pragma "inline" def +(a: complex(?w), b: float(w/2)) return (a.real+b, a.imag):complex;
pragma "inline" def +(a: imaginary(?w), b: imaginary(w)) return __primitive("+", a, b);
pragma "inline" def +(a: imaginary(?w), b: complex(w*2)) return (b.real, _i2f(a)+b.imag):complex;
pragma "inline" def +(a: complex(?w), b: imaginary(w/2)) return (a.real, a.imag+_i2f(b)):complex;
pragma "inline" def +(a: complex(?w), b: complex(w)) return (a.real+b.real, a.imag+b.imag):complex;

pragma "inline" def -(a: float(?w), b: float(w)) return __primitive("-", a, b);
pragma "inline" def -(a: float(?w), b: imaginary(w)) return (a, -_i2f(b)):complex;
pragma "inline" def -(a: imaginary(?w), b: float(w)) return (-b, _i2f(a)):complex;
pragma "inline" def -(a: float(?w), b: complex(w*2)) return (a-b.real, -b.imag):complex;
pragma "inline" def -(a: complex(?w), b: float(w/2)) return (a.real-b, a.imag):complex;
pragma "inline" def -(a: imaginary(?w), b: imaginary(w)) return __primitive("-", a, b);
pragma "inline" def -(a: imaginary(?w), b: complex(w*2)) return (-b.real, _i2f(a)-b.imag):complex;
pragma "inline" def -(a: complex(?w), b: imaginary(w/2)) return (a.real, a.imag-_i2f(b)):complex;
pragma "inline" def -(a: complex(?w), b: complex(w)) return (a.real-b.real, a.imag-b.imag):complex;

//
// * and /
//


pragma "inline" def *(a: float(?w), b: float(w)) return __primitive("*", a, b);

pragma "inline" def *(a: float(?w), b: imaginary(w)) return (a*_i2f(b)):imaginary(w);
pragma "inline" def *(a: imaginary(?w), b: float(w)) return (_i2f(a)*b):imaginary(w);

pragma "inline" def *(a: float(?w), b: complex(w*2)) return (a*b.real, a*b.imag):complex;
pragma "inline" def *(a: complex(?w), b: float(w/2)) return (a.real*b, a.imag*b):complex;

pragma "inline" def *(a: imaginary(?w), b: imaginary(w)) return _i2f(__primitive("*", a, b));

pragma "inline" def *(a: imaginary(?w), b: complex(w*2)) return (-_i2f(a)*b.imag, _i2f(a)*b.real):complex;
pragma "inline" def *(a: complex(?w), b: imaginary(w/2)) return (-a.imag*_i2f(b), a.real*_i2f(b)):complex;

pragma "inline" def *(a: complex(?w), b: complex(w)) return (a.real*b.real-a.imag*b.imag, a.imag*b.real+a.real*b.imag):complex;

pragma "inline" def /(a: float(?w), b: float(w)) return __primitive("/", a, b);

pragma "inline" def /(a: float(?w), b: imaginary(w)) return (-a/_i2f(b)):imaginary(w);
pragma "inline" def /(a: imaginary(?w), b: float(w)) return (_i2f(a)/b):imaginary(w);

pragma "inline" def /(a: float(?w), b: complex(w*2))
  return let d = b.real*b.real+b.imag*b.imag in
    (a*b.real/d, -a*b.imag/d):complex;

pragma "inline" def /(a: complex(?w), b: float(w/2))
  return (a.real/b, a.imag/b):complex;

pragma "inline" def /(a: imaginary(?w), b: imaginary(w)) return _i2f(__primitive("/", a, b));

pragma "inline" def /(a: imaginary(?w), b: complex(w*2))
  return let d = b.real*b.real+b.imag*b.imag in
    (_i2f(a)*b.imag/d, _i2f(a)*b.real/d):complex;

pragma "inline" def /(a: complex(?w), b: imaginary(w/2))
  return let d = _i2f(b)*_i2f(b) in
    (a.imag/_i2f(b), -a.real/_i2f(b)):complex;

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
pragma "inline" def _tostring(x : imaginary, format : string)
  return __primitive("to_string", format, x);

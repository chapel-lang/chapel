//
// special type functions
//

pragma "inline" def _init(x : real(?w)) return 0.0:real(w);
pragma "inline" def _init(x : imag(?w)) return 0.0:imag(w);
pragma "inline" def _init(x: complex(?w)) return (0.0:real(w/2), 0.0:real(w/2)):complex;

//
// complex component methods re and im
//
//  bug? note: in setter, real argument should be parameterized over
//             complex bit width
//

def complex.re return __primitive( "complex_get_real", this);
def complex.im return __primitive( "complex_get_imag", this);
def complex.=re(f:real) { __primitive( "complex_set_real", this, f); }
def complex.=im(f:real) { __primitive( "complex_set_imag", this, f); }

//
// assignment
//

pragma "inline" def =(a: real(?w), b: real(w)) return b;
pragma "inline" def =(a: imag(?w), b: imag(w)) return b;
pragma "inline" def =(a: complex(?w), b: complex(w)) return b;


//
// relational operators
//
//  question: should there be any over imag?  over complex?
//

pragma "inline" def ==(a: real(?w), b: real(w)) return __primitive("==", a, b);
pragma "inline" def !=(a: real(?w), b: real(w)) return __primitive("!=", a, b);
pragma "inline" def <=(a: real(?w), b: real(w)) return __primitive("<=", a, b);
pragma "inline" def >=(a: real(?w), b: real(w)) return __primitive(">=", a, b);
pragma "inline" def <(a: real(?w), b: real(w)) return __primitive("<", a, b);
pragma "inline" def >(a: real(?w), b: real(w)) return __primitive(">", a, b);

//
// unary + and -
//

pragma "inline" def +(a: real(?w)) return __primitive("u+", a);
pragma "inline" def -(a: real(?w)) return __primitive("u-", a);
pragma "inline" def +(a: imag(?w)) return __primitive("u+", a):imag(w);
pragma "inline" def -(a: imag(?w)) return __primitive("u-", a):imag(w);
pragma "inline" def +(a: complex(?w)) return a;
pragma "inline" def -(a: complex(?w)) return (-a.re, -a.im):complex;

//
// binary + and -
//

pragma "inline" def _i2f(a: imag(?w)) return a:real(w);

pragma "inline" def +(a: real(?w), b: real(w)) return __primitive("+", a, b);
pragma "inline" def +(a: real(?w), b: imag(w)) return (a, _i2f(b)):complex;
pragma "inline" def +(a: imag(?w), b: real(w)) return (b, _i2f(a)):complex;
pragma "inline" def +(a: real(?w), b: complex(w*2)) return (a+b.re, b.im):complex;
pragma "inline" def +(a: complex(?w), b: real(w/2)) return (a.re+b, a.im):complex;
pragma "inline" def +(a: imag(?w), b: imag(w)) return __primitive("+", a, b);
pragma "inline" def +(a: imag(?w), b: complex(w*2)) return (b.re, _i2f(a)+b.im):complex;
pragma "inline" def +(a: complex(?w), b: imag(w/2)) return (a.re, a.im+_i2f(b)):complex;
pragma "inline" def +(a: complex(?w), b: complex(w)) return (a.re+b.re, a.im+b.im):complex;

pragma "inline" def -(a: real(?w), b: real(w)) return __primitive("-", a, b);
pragma "inline" def -(a: real(?w), b: imag(w)) return (a, -_i2f(b)):complex;
pragma "inline" def -(a: imag(?w), b: real(w)) return (-b, _i2f(a)):complex;
pragma "inline" def -(a: real(?w), b: complex(w*2)) return (a-b.re, -b.im):complex;
pragma "inline" def -(a: complex(?w), b: real(w/2)) return (a.re-b, a.im):complex;
pragma "inline" def -(a: imag(?w), b: imag(w)) return __primitive("-", a, b);
pragma "inline" def -(a: imag(?w), b: complex(w*2)) return (-b.re, _i2f(a)-b.im):complex;
pragma "inline" def -(a: complex(?w), b: imag(w/2)) return (a.re, a.im-_i2f(b)):complex;
pragma "inline" def -(a: complex(?w), b: complex(w)) return (a.re-b.re, a.im-b.im):complex;

//
// * and /
//


pragma "inline" def *(a: real(?w), b: real(w)) return __primitive("*", a, b);

pragma "inline" def *(a: real(?w), b: imag(w)) return (a*_i2f(b)):imag(w);
pragma "inline" def *(a: imag(?w), b: real(w)) return (_i2f(a)*b):imag(w);

pragma "inline" def *(a: real(?w), b: complex(w*2)) return (a*b.re, a*b.im):complex;
pragma "inline" def *(a: complex(?w), b: real(w/2)) return (a.re*b, a.im*b):complex;

pragma "inline" def *(a: imag(?w), b: imag(w)) return _i2f(__primitive("*", a, b));

pragma "inline" def *(a: imag(?w), b: complex(w*2)) return (-_i2f(a)*b.im, _i2f(a)*b.re):complex;
pragma "inline" def *(a: complex(?w), b: imag(w/2)) return (-a.im*_i2f(b), a.re*_i2f(b)):complex;

pragma "inline" def *(a: complex(?w), b: complex(w)) return (a.re*b.re-a.im*b.im, a.im*b.re+a.re*b.im):complex;

pragma "inline" def /(a: real(?w), b: real(w)) return __primitive("/", a, b);

pragma "inline" def /(a: real(?w), b: imag(w)) return (-a/_i2f(b)):imag(w);
pragma "inline" def /(a: imag(?w), b: real(w)) return (_i2f(a)/b):imag(w);

pragma "inline" def /(a: real(?w), b: complex(w*2))
  return let d = b.re*b.re+b.im*b.im in
    (a*b.re/d, -a*b.im/d):complex;

pragma "inline" def /(a: complex(?w), b: real(w/2))
  return (a.re/b, a.im/b):complex;

pragma "inline" def /(a: imag(?w), b: imag(w)) return _i2f(__primitive("/", a, b));

pragma "inline" def /(a: imag(?w), b: complex(w*2))
  return let d = b.re*b.re+b.im*b.im in
    (_i2f(a)*b.im/d, _i2f(a)*b.re/d):complex;

pragma "inline" def /(a: complex(?w), b: imag(w/2))
  return let d = _i2f(b)*_i2f(b) in
    (a.im/_i2f(b), -a.re/_i2f(b)):complex;

pragma "inline" def /(a: complex(?w), b: complex(w))
  return let d = b.re*b.re+b.im*b.im in
    ((a.re*b.re+a.im*b.im)/d, (a.im*b.re-a.re*b.im)/d):complex;

//
// **
//

pragma "inline" def **(a: real(?w), b: real(w)) return __primitive("**", a, b);

//
// miscellaneous functions
//
pragma "inline" def conjg(a: complex(?w))
  return (a.re, -a.im):complex;

//
// string conversions
//
pragma "inline" def _tostring(x: complex, format: string)
  return __primitive("to_string", format, x);
pragma "inline" def _tostring(x : real, format : string)
  return __primitive("to_string", format, x);
pragma "inline" def _tostring(x : imag, format : string)
  return __primitive("to_string", format, x);

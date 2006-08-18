pragma "inline" def _init(x : complex) return 0.0i;

// complex(32) bound functions
def _complex32.real: float(32) return __primitive( "complex_get_real", this);
def _complex32.imag: float(32) return __primitive( "complex_get_imag", this);
def _complex32.=real (f:float) { __primitive( "complex_set_real", this, f); }
def _complex32.=imag (f:float) { __primitive( "complex_set_imag", this, f); }

// complex(64) bound functions
def complex.real: float return __primitive( "complex_get_real", this);
def complex.imag: float return __primitive( "complex_get_imag", this);
def complex.=real (f:float) { __primitive( "complex_set_real", this, f); }
def complex.=imag (f:float) { __primitive( "complex_set_imag", this, f); }

def =(a:complex(?w), b:complex(w)) return b;

def +(a:complex(?w), b:complex(w)) {
  var sum:complex(w);
  sum.real = a.real + b.real;
  sum.imag = a.imag + b.imag;
  return sum;
}

def -(a:complex(?w), b:complex(w)) {
  var diff:complex(w);
  diff.real = a.real - b.real;
  diff.imag = a.imag - b.imag;
  return diff;
}

def *(a:complex(?w), b:complex(w)) {
  var prod:complex(w);
  prod.real = a.real*b.real - a.imag*b.imag;
  prod.imag = a.imag*b.real + a.real*b.imag;
  return prod;
}

def /(a:complex(?w), b:complex(w)) {
  var div:complex(w);
  var d:float(w) = b.real*b.real + b.imag*b.imag;
  div.real = (a.real*b.real + a.imag*b.imag)/d; 
  div.imag = (a.imag*b.real - a.real*b.imag)/d;
  return div;
}

def conjg(a:complex(?w)) {
  var c:complex(w);
  c.real = a.real;
  c.imag = -a.imag;
  return c;
}

pragma "no codegen"
pragma "rename _chpl_tostring_complex64"
def _tostring(x : complex, format : string) : string {
  return __primitive("to_string", format, x);
}

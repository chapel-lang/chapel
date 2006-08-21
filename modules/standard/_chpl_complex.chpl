pragma "inline" def _init(x: complex(?w)) return 0.0i:x.type;

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

def =(a: complex(?w), b: complex(w)) return b;

pragma "inline" def _complex(real, imag) {
  var x: complex;
  x.real = real;
  x.imag = imag;
  return x;
}

pragma "inline" def +(a: complex(?w), b: complex(w))
  return _complex(a.real+b.real, a.imag+b.imag);

pragma "inline" def -(a: complex(?w), b: complex(w))
  return _complex(a.real-b.real, a.imag-b.imag);

pragma "inline" def *(a: complex(?w), b: complex(w))
  return _complex(a.real*b.real-a.imag*b.imag, a.imag*b.real+a.real*b.imag);

pragma "inline" def /(a: complex(?w), b: complex(w))
  return let d = b.real*b.real+b.imag*b.imag in
    _complex((a.real*b.real+a.imag*b.imag)/d, (a.imag*b.real-a.real*b.imag)/d);

pragma "inline" def conjg(a: complex(?w))
  return _complex(a.real, -a.imag);

pragma "no codegen"
pragma "rename _chpl_tostring_complex64"
def _tostring(x: complex, format: string) : string {
  return __primitive("to_string", format, x);
}

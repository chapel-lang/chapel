pragma "inline" def _init(x : complex) return 0.0i;

// complex(32)
def _complex32.real: float(32) {
  return __primitive( "complex_get_real", this);
}

def _complex32.imag: float(32) {
  return __primitive( "complex_get_imag", this);
}

def _complex32.=real (f:float) {
  __primitive( "complex_set_real", this, f);
}

def _complex32.=imag (f:float) {
  __primitive( "complex_set_imag", this, f);
}

def =(a:complex(32), b:complex) {  // for now
  var ret:complex(32);
  ret.imag = b.imag;
  ret.real = b.real;
  return ret;
}

def +(a:complex(32), b:complex(32)) {
  var sum:complex(32);
  sum.real = a.real + b.real;
  sum.imag = a.imag + b.imag;
  return sum;
}

def -(a:complex(32), b:complex(32)) {
  var diff:complex(32);
  diff.real = a.real - b.real;
  diff.imag = a.imag - b.imag;
  return diff;
}

def *(a:complex(32), b:complex(32)) {
  var prod:complex(32);
  prod.real = a.real*b.real - a.imag*b.imag;
  prod.imag = a.imag*b.real + a.real*b.imag;
  return prod;
}

def /(a:complex(32), b:complex(32)) {
  var div:complex(32);
  var d:float(32) = b.real*b.real + b.imag*b.imag;
  div.real = (a.real*b.real + a.imag*b.imag)/d; 
  div.imag = (a.imag*b.real - a.real*b.imag)/d;
  return div;
}

def conjg(a:complex(32)) {
  var c:complex(32);
  c.real = a.real;
  c.imag = -a.imag;
  return c;
}


// complex(64)
def complex.real: float {
  return __primitive( "complex_get_real", this);
}

def complex.imag: float {
  return __primitive( "complex_get_imag", this);
}

def complex.=real (f:float) {
  __primitive( "complex_set_real", this, f);
}

def complex.=imag (f:float) {
  __primitive( "complex_set_imag", this, f);
}

def =(a:complex(64), b:complex(64)) {
  var ret:complex(64);
  ret.imag = b.imag;
  ret.real = b.real;
  return ret;
}

def +(a:complex(64), b:complex(64)) {
  var sum:complex(64);
  sum.real = a.real + b.real;
  sum.imag = a.imag + b.imag;
  return sum;
}

def -(a:complex(64), b:complex(64)) {
  var diff:complex(64);
  diff.real = a.real - b.real;
  diff.imag = a.imag - b.imag;
  return diff;
}

def *(a:complex(64), b:complex(64)) {
  var prod:complex(64);
  prod.real = a.real*b.real - a.imag*b.imag;
  prod.imag = a.imag*b.real + a.real*b.imag;
  return prod;
}

def /(a:complex(64), b:complex(64)) {
  var div:complex(64);
  var d:float(64) = b.real*b.real + b.imag*b.imag;
  div.real = (a.real*b.real + a.imag*b.imag)/d; 
  div.imag = (a.imag*b.real - a.real*b.imag)/d;
  return div;
}

def conjg(a:complex(64)) {
  var c:complex(64);
  c.real = a.real;
  c.imag = -a.imag;
  return c;
}


pragma "no codegen"
pragma "rename _chpl_tostring_complex64"
def _tostring(x : complex, format : string) : string {
  return __primitive("to_string", format, x);
}

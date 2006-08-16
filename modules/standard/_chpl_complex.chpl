def complex.real: float {
  return __primitive( "_chpl_complex_real", this);
}

def complex.imag: float {
  return __primitive( "_chpl_complex_imag", this);
}

def complex.=real (f:float) {
  __primitive( "_chpl_complex_set_real", this, f);
}

def complex.=imag (f:float) {
  __primitive( "_chpl_complex_set_imag", this, f);
}

def =(a:complex, b:complex) {
  var ret:complex;
  ret.imag = b.imag;
  ret.real = b.real;
  return ret;
}

def +(f:float, c:complex) {
  var ret:complex;
  ret.imag = c.imag;
  ret.real = c.real + f;
  return ret;
}

def +(c:complex, f:float) {
  var ret:complex;
  ret.imag = c.imag;
  ret.real = c.real + f;
  return ret;
}

def +(a:complex, b:complex) {
  var sum:complex;
  sum.real = a.real + b.real;
  sum.imag = a.imag + b.imag;
  return sum;
}

def -(a:complex, b:complex) {
  var diff:complex;
  diff.real = a.real - b.real;
  diff.imag = a.imag - b.imag;
  return diff;
}

def -(f:float, c:complex) {
  var diff:complex;
  diff.real = f - c.real;
  diff.imag = -c.imag;
  return diff;
}

def -(c:complex, f:float) {
  var diff:complex;
  diff.real = c.real - f;
  diff.imag = c.imag;
  return diff;
}

def *(a:complex, b:complex) {
  var prod:complex;
  prod.real = a.real*b.real - a.imag*b.imag;
  prod.imag = a.imag*b.real + a.real*b.imag;
  return prod;
}

def /(a:complex, b:complex) {
  var div:complex;
  var d:float = b.real*b.real + b.imag*b.imag;
  div.real = (a.real*b.real + a.imag*b.imag)/d; 
  div.imag = (a.imag*b.real - a.real*b.imag)/d;
  return div;
}

def conjg(a:complex) {
  var c:complex;
  c.real = a.real;
  c.imag = -a.imag;
  return c;
}


pragma "no codegen"
pragma "rename _chpl_tostring_complex64"
def _tostring(x : complex, format : string) : string {
  return __primitive("to_string", format, x);
}

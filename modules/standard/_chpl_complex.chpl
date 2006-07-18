fun complex.real: float {
  return __primitive( "_chpl_complex_real", this);
}

fun complex.imag: float {
  return __primitive( "_chpl_complex_imag", this);
}

fun complex.=real (f:float) {
  __primitive( "_chpl_complex_set_real", this, f);
}

fun complex.=imag (f:float) {
  __primitive( "_chpl_complex_set_imag", this, f);
}

fun =(a:complex, b:complex) {
  var ret:complex;
  ret.imag = b.imag;
  ret.real = b.real;
  return ret;
}

fun +(f:float, c:complex) {
  var ret:complex;
  ret.imag = c.imag;
  ret.real = c.real + f;
  return ret;
}

fun +(c:complex, f:float) {
  var ret:complex;
  ret.imag = c.imag;
  ret.real = c.real + f;
  return ret;
}

fun +(a:complex, b:complex) {
  var sum:complex;
  sum.real = a.real + b.real;
  sum.imag = a.imag + b.imag;
  return sum;
}

fun -(a:complex, b:complex) {
  var diff:complex;
  diff.real = a.real - b.real;
  diff.imag = a.imag - b.imag;
  return diff;
}

fun -(f:float, c:complex) {
  var diff:complex;
  diff.real = f - c.real;
  diff.imag = -c.imag;
  return diff;
}

fun -(c:complex, f:float) {
  var diff:complex;
  diff.real = c.real - f;
  diff.imag = c.imag;
  return diff;
}

fun *(a:complex, b:complex) {
  var prod:complex;
  prod.real = a.real*b.real - a.imag*b.imag;
  prod.imag = a.imag*b.real + a.real*b.imag;
  return prod;
}

fun /(a:complex, b:complex) {
  var div:complex;
  var d:float = b.real*b.real + b.imag*b.imag;
  div.real = (a.real*b.real + a.imag*b.imag)/d; 
  div.imag = (a.imag*b.real - a.real*b.imag)/d;
  return div;
}

fun conjg(a:complex) {
  var c:complex;
  c.real = a.real;
  c.imag = -a.imag;
  return c;
}



pragma "rename _chpl_fwrite_complex"
fun fwrite(f : file = stdout, x : complex) {
  fwrite(f, x.real);
  if (x.imag < 0) {
    fwrite(f, " - ");
    x.imag = -x.imag;
  } else {
    fwrite(f, " + ");
  }
  fwrite(f, x.imag, "i");
}

/*
pragma "rename _chpl_fread_complex"
fun fread(f : file = stdin, x : complex) {
  var realPart: float;
  var imagPart: float;
  var imagI: string;
  var matchingCharWasRead: int;
  var isNeg: bool;
  var ignoreWhiteSpace = true;

  fread(f, realPart);

  matchingCharWasRead = _readLitChar(f.fp, "+", ignoreWhiteSpace);
  if (matchingCharWasRead != 1) {
    matchingCharWasRead = _readLitChar(f.fp, "-", ignoreWhiteSpace);
    if (matchingCharWasRead != 1) {
      halt("***Error: Incorrect format for complex numbers***");
    }
    isNeg = true;
  }

  fread(f, imagPart);

  ignoreWhiteSpace = false;
  matchingCharWasRead = _readLitChar(f.fp, "i", ignoreWhiteSpace);
  if (matchingCharWasRead != 1) {
    halt("***Error: Incorrect format for complex numbers***");
  }

  x.real = realPart;
  if (isNeg) {
    x.imag = -imagPart;
  } else {
    x.imag = imagPart;
  }
}
*/


pragma "no codegen"
pragma "rename _chpl_tostring_complex"
fun _tostring(x : complex, format : string) : string {
  return __primitive("to_string", format, x);
}

pragma "rename _chpl_complex" record complex {
  var real : float;
  var imag : float;
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

pragma "no codegen"
pragma "rename _chpl_tostring_complex"
fun _tostring(x : complex, format : string) : string {
  return __primitive("to_string", format, x);
}


fun +(x : complex, y : complex)
  return complex(x.real + y.real, x.imag + y.imag);

fun +(x : float, y : complex)
  return complex(x + y.real, y.imag);

fun -(x : complex, y : complex)
  return complex(x.real - y.real, x.imag - y.imag);

fun *(x : complex, y : complex)
  return complex(x.real*y.real - x.imag*y.imag,
                 x.imag*y.real + x.real*y.imag);

fun /(x : complex, y : complex)
  return let d = y.real*y.real + y.imag*y.imag in
    complex((x.real*y.real + x.imag*y.imag)/d,
            (x.imag*y.real - x.real*y.imag)/d);

fun conjg(x : complex)
  return complex(x.real, -x.imag);

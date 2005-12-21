pragma "rename _chpl_complex" class complex : value {
  var real : float;
  var imag : float;
}


pragma "rename _chpl_fwrite_complex"
function fwrite(f : file = stdout, x : complex) {
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
function fread(f : file = stdin, x : complex) {
  var realPart: float;
  var imagPart: float;
  var imagI: string;
  var matchingCharWasRead: integer;
  var isNeg: boolean;
  var ignoreWhiteSpace = 1;

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

  ignoreWhiteSpace = 0;
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
function _tostring(x : complex, format : string) : string {
  return _complex_tostring_hack(x, format);
}


function +(x : complex, y : complex)
  return complex(x.real + y.real, x.imag + y.imag);

function +(x : float, y : complex)
  return complex(x + y.real, y.imag);

function -(x : complex, y : complex)
  return complex(x.real - y.real, x.imag - y.imag);

function *(x : complex, y : complex)
  return complex(x.real*y.real - x.imag*y.imag,
                 x.imag*y.real + x.real*y.imag);

function /(x : complex, y : complex)
  return let d = y.real*y.real + y.imag*y.imag in
    complex((x.real*y.real + x.imag*y.imag)/d,
            (x.imag*y.real - x.real*y.imag)/d);

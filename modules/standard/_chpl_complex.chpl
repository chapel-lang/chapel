pragma "rename _chpl_complex" class complex : value {
  var real : float;
  var imag : float;
}

pragma "rename _chpl_write_complex"
function write(x : complex) {
  write(x.real, " + ", x.imag, "i");
}

pragma "no codegen"
pragma "rename _chpl_read_complex"
function read(inout x : complex) {
  _complex_read_hack(x);
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

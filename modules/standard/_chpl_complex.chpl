pragma "rename _chpl_complex" record complex {
  var _im : float;
  var _re : float;
  function imag var : float { return _im; }
  function real var : float { return _re; }
}

pragma "rename _chpl_write_complex"
function write(x : complex) {
  write(x._re, " + ", x._im, "i");
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
  return complex(x._im + y._im, x._re + y._re);

function +(x : float, y : complex)
  return complex(y._im, x + y._re);

function -(x : complex, y : complex)
  return complex(x._im - y._im, x._re - y._re);

function *(x : complex, y : complex)
  return complex(x._im*y._re + x._re*y._im,
                 x._re*y._re - x._im*y._im);

function /(x : complex, y : complex)
  return let d = y._re*y._re + y._im*y._im in
    complex((x._im*y._re - x._re*y._im)/d,
            (x._re*y._re + x._im*y._im)/d);


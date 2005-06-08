pragma "rename _chpl_complex" record complex {
  var _re : float;
  var _im : float;
  function real : float { return _re; }
  function imag : float { return _im; }
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

pragma "omit for noanalysis" function +(x : complex, y : complex)
  return complex(x._re + y._re, x._im + y._im);

pragma "omit for noanalysis" function -(x : complex, y : complex)
  return complex(x._re - y._re, x._im - y._im);

pragma "omit for noanalysis" function *(x : complex, y : complex)
  return complex(x._re*y._re - x._im*y._im,
                 x._im*y._re + x._re*y._im);

pragma "omit for noanalysis" function /(x : complex, y : complex)
  return let d = y._re*y._re + y._im*y._im in
    complex((x._re*y._re + x._im*y._im)/d,
            (x._im*y._re - x._re*y._im)/d);


pragma "rename _chpl_complex" record complex {
  var _chpl_re : float;
  var _chpl_im : float;
  function real : float { return _chpl_re; }
  function imag : float { return _chpl_im; }
}

pragma "rename _chpl_write_complex"
function write(x : complex) {
  write(x._chpl_re, " + ", x._chpl_im, "i");
}

pragma "no codegen"
pragma "rename _chpl_read_complex"
function read(inout x : complex) {
  _chpl_complex_read_hack(x);
}

pragma "no codegen"
pragma "rename _chpl_tostring_complex"
function _chpl_tostring(x : complex, format : string) : string {
  return _chpl_complex_tostring_hack(x, format);
}

pragma "omit for noanalysis" function +(x : complex, y : complex)
  return complex(x._chpl_re + y._chpl_re, x._chpl_im + y._chpl_im);

pragma "omit for noanalysis" function -(x : complex, y : complex)
  return complex(x._chpl_re - y._chpl_re, x._chpl_im - y._chpl_im);

pragma "omit for noanalysis" function *(x : complex, y : complex)
  return complex(x._chpl_re*y._chpl_re - x._chpl_im*y._chpl_im,
                 x._chpl_im*y._chpl_re + x._chpl_re*y._chpl_im);

pragma "omit for noanalysis" function /(x : complex, y : complex)
  return let d = y._chpl_re*y._chpl_re + y._chpl_im*y._chpl_im in
    complex((x._chpl_re*y._chpl_re + x._chpl_im*y._chpl_im)/d,
            (x._chpl_im*y._chpl_re - x._chpl_re*y._chpl_im)/d);


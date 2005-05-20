pragma "rename _chpl_complex" record complex {
  var re : float;
  var im : float;
}

pragma "rename _chpl_write_complex"
function write(x : complex) {
  write(x.re, " + ", x.im, "i");
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
  return complex(x.re + y.re, x.im + y.im);

pragma "omit for noanalysis" function -(x : complex, y : complex)
  return complex(x.re - y.re, x.im - y.im);

pragma "omit for noanalysis" function *(x : complex, y : complex)
  return complex(x.re*y.re - x.im*y.im, x.im*y.re + x.re*y.im);

pragma "omit for noanalysis" function /(x : complex, y : complex)
  return let d = y.re*y.re + y.im*y.im in
    complex((x.re*y.re + x.im*y.im)/d, (x.im*y.re - x.re*y.im)/d);

-- Get rid of below when coercion wrapper is working

pragma "omit for noanalysis" function +(x : complex, y : float)
  return complex(x.re + y, x.im);

pragma "omit for noanalysis" function +(x : float, y : complex)
  return complex(x + y.re, y.im);

pragma "omit for noanalysis" function -(x : complex, y : float)
  return complex(x.re - y, x.im);

pragma "omit for noanalysis" function -(x : float, y : complex)
  return complex(x - y.re, -y.im);

pragma "omit for noanalysis" function *(x : complex, y : float)
  return complex(x.re*y, x.im*y);

pragma "omit for noanalysis" function *(x : float, y : complex)
  return complex(x*y.re, x*y.im);

pragma "omit for noanalysis" function /(x : complex, y : float)
  return complex(x.re/y, x.im/y);

pragma "omit for noanalysis" function /(x : float, y : complex)
  return let d = y.re*y.re + y.im*y.im in
    complex(x*y.re/d, -x*y.im/d);

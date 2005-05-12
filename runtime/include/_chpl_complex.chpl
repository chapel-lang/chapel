pragma "rename _chpl_complex" record complex {
  var re : float;
  var im : float;
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

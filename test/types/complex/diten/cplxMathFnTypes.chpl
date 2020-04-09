proc testTypes(x: complex(?w)) {
  const res1 = abs(x);
  assert(res1.type == real(w/2));

  const res2 = carg(x);
  assert(res2.type == real(w/2));

  const res3 = conjg(x);
  assert(res3.type == complex(w));

  const res4 = cproj(x);
  assert(res4.type == real(w/2));

  const res5 = exp(x);
  assert(res5.type == complex(w));

  const res6 = log(x);
  assert(res6.type == complex(w));

  const res7 = sqrt(x);
  assert(res7.type == complex(w));

  const res8 = sin(x);
  assert(res8.type == complex(w));

  const res9 = cos(x);
  assert(res9.type == complex(w));

  const res10 = tan(x);
  assert(res10.type == complex(w));

  const res11 = asin(x);
  assert(res11.type == complex(w));

  const res12 = acos(x);
  assert(res12.type == complex(w));

  const res13 = atan(x);
  assert(res13.type == complex(w));

  const res14 = sinh(x);
  assert(res14.type == complex(w));

  const res15 = cosh(x);
  assert(res15.type == complex(w));

  const res16 = tanh(x);
  assert(res16.type == complex(w));

  const res17 = asinh(x);
  assert(res17.type == complex(w));

  const res18 = acosh(x);
  assert(res18.type == complex(w));

  const res19 = atanh(x);
  assert(res19.type == complex(w));
}

var c64 = 1.0:real(32) + 2.0i:imag(32); 
var c128 = 1.0: real(64) + 2.0i: imag(64);

testTypes(c64);
testTypes(c128);

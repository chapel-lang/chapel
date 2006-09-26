
// BLC -- a good example of where one would like to declare multiple
// arguments of the same type/intent:
def butterfly(wk1: complex, wk2: complex, wk3: complex, 
              inout a, inout b, inout c, inout d) {
  var x0 = a + b;
  var x1 = a - b;
  var x2 = c + d;
  var x3 = c - d;

  a = x0 + x2;
  x0 -= x2;
  c = wk2 * x0;
  x0 = complex(x1.real - x3.imag, x1.imag + x3.real);
  b = wk1 * x0;
  x0 = complex(x1.real + x3.imag, x1.imag - x3.real);
  d = wk3 * x0;
}

// called like:
//
// for j in 0..bw do
//   butterfly(1.0, 1.0, 1.0, a(j), a(j+bw), a(j+2*bw), a(j+3*bw));


// OR:

def butterfly(wk1: complex, wk2: complex, wk3: complex, 
              abcd: [1..4]) {
  var x0 = abcd(1) + abcd(2);
  var x1 = abcd(1) - abcd(2);
  var x2 = abcd(3) + abcd(4);
  var x3 = abcd(3) - abcd(4);

  abcd(1) = x0 + x2;
  x0 -= x2;
  abcd(3) = wk2 * x0;
  x0 = complex(x1.real - x3.imag, x1.imag + x3.real);
  abcd(2) = wk1 * x0;
  x0 = complex(x1.real + x3.imag, x1.imag - x3.real);
  abcd(4) = wk3 * x0;
}


// called like:
//  for j in 0..bw do
//    butterfly(1.0, 1.0, 1.0, x[j..j+3*bw by bw])

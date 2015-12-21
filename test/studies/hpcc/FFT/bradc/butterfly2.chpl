proc butterfly(wk1: complex, wk2: complex, wk3: complex, 
              abcd: [?D] complex) {
  const a = D.low,
        b = a + D.stride,
        c = b + D.stride,
        d = c + D.stride;
  var x0 = abcd(a) + abcd(b);
  var x1 = abcd(a) - abcd(b);
  var x2 = abcd(c) + abcd(d);
  var x3 = abcd(c) - abcd(d);

//  writeln("abcd is: ", abcd);

  abcd(a) = x0 + x2;
  x0 -= x2;
  abcd(c) = wk2 * x0;
  x0 = (x1.re - x3.im, x1.im + x3.re):complex;
  abcd(b) = wk1 * x0;
  x0 = (x1.re + x3.im, x1.im - x3.re):complex;
  abcd(d) = wk3 * x0;

//  writeln("abcd is: ", abcd);
}


config var logN = 3;
const N = 0x1 << logN;

const D = {0..N-1};

var A: [D] complex;

[i in D] A(i) = (2*i, 2*i+1):complex;

for i in D by 4 {
  butterfly((-1.0, -2.0):complex, (-3.0, -4.0):complex, (-5.0, -6.0):complex,
            A[i..i+3]);
}

writeln("A is: ", A);

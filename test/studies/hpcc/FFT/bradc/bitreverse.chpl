config var logN = 5;
const N = 1 << logN;

var DW = [0..N/4-1];

var A: [DW] complex;

[i in DW] A(i) = _complex(i, i);

writeln("A is: ", A);

var V = bitReverse(A);

writeln("V is: ", V);


def bitReverse(W: [?WD] complex) {
  const n = WD(1).length;
  const reverse = lg(n);
  var V: [WD] complex;
  for i in WD {
    var ndx = bitReverse(i:uint(64), numBits=reverse);
    V(ndx:int) = W(i); // BLC: unfortunate cast
  }
  return V;
}

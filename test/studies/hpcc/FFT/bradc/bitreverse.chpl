use BitOps;

config var logN = 5;
const N = 1 << logN;

var DW = {0..N/4-1};

var A: [DW] complex;

[i in DW] A(i) = (i, i):complex;

writeln("A is: ", A);

var V = bitReverse(A);

writeln("V is: ", V);


proc bitReverse(W: [?WD] complex) {
  const n = WD.dim(1).size;
  const reverse = log2(n);
  var V: [WD] complex;
  for i in WD {
    var ndx = bitReverse(i:uint(64), numBits=reverse);
    V(ndx:int) = W(i); // BLC: unfortunate cast
  }
  return V;
}


// reverses numBits low-order bits of val
proc bitReverse(val: ?valType, numBits = 64) {
  param mask: uint(64) = 0x0102040810204080;
  const valReverse64 = bitMatMultOr(mask, bitMatMultOr(val:uint(64), mask));
  const valReverse = rotl(valReverse64, numBits);
  return valReverse: valType;
}

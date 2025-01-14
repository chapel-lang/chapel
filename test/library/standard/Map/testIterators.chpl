use Sort;
use Map;

record FacInt {
  var n: uint = 0;
  var nFac: uint = 1;

  proc init() {
    n = 0;
    nFac = 1;
  }

  proc init(n: uint) {
    this.n = n;
    init this;
    this.nFac = factorial(n);
  }

  proc const factorial(n: uint) {
    var bi: uint = 1;
    if n > 15 then
      halt("I only do small numbers");

    for i in 2..n do bi *= i;
    return bi;
  }
}

operator FacInt.<(a: FacInt, b: FacInt) {
  return a.n < b.n;
}

operator FacInt.>(a: FacInt, b: FacInt) {
  return a.n > b.n;
}

var fac = new map(int, FacInt);

for i in 1..15 {
  fac[i] = new FacInt(i: uint);
}

var A: [1..fac.size] (int, FacInt);
for item in zip(fac.keys(), fac.values()) {
  A[item(0)] = item;
}
writeln("A: ", sorted(A));

var B: [1..fac.size] FacInt;
for (val, i) in zip(fac.values(), 1..) {
  B[i] = val;
}
writeln("B: ", sorted(B));

var C: [1..fac.size] (int, FacInt);
for key in fac.keys() {
  C[key] = (key, fac[key]);
}
writeln("C: ", sorted(C));

var D: [1..fac.size] (int, FacInt);
forall item in zip(fac.keys(), fac.values()) {
  D[item(0)] = item;
}
writeln("D: ", sorted(D));

var E: [1..fac.size] (FacInt, int);
forall item in zip(fac.values(), fac.keys()) {
  E[item(1)] = item;
}
writeln("E: ", sorted(E));

var F: [1..fac.size] (int, FacInt);
forall key in fac.keys() {
  F[key] = (key, fac[key]);
}
writeln("F: ", sorted(F));

var G: [1..fac.size] FacInt;
var Hidx: atomic int = 1;
forall val in fac.values() {
  var i = Hidx.fetchAdd(1);
  G[i] = val;
}
writeln("G: ", sorted(G));

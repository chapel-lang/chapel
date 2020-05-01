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
    this.complete();
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

proc <(a: FacInt, b: FacInt) {
  return a.n < b.n;
}

proc >(a: FacInt, b: FacInt) {
  return a.n > b.n;
}

var fac = new map(int, FacInt);

for i in 1..15 {
  fac[i] = new FacInt(i: uint);
}

var A: [1..fac.size] (int, FacInt);
for key in fac {
  A[key] = (key, fac[key]);
}
writeln(A.sorted());

var B: [1..fac.size] (int, FacInt);
for item in fac.items() {
  B[item(0)] = item;
}
writeln(B.sorted());

var C: [1..fac.size] FacInt;
for (val, i) in zip(fac.values(), 1..) {
  C[i] = val;
}
writeln(C.sorted());

var D: [1..fac.size] (int, FacInt);
for key in fac.keys() {
  D[key] = (key, fac[key]);
}
writeln(D.sorted());

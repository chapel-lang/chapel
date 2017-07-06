proc badReturnRefLocal() ref {
  var i = 1;
  return i;
}

proc badReturnRefConstant() ref {
  return 1;
}

proc badReturnSliceLocal() {
  var A: [1..100] int;
  return A[1..5];
}

proc badReturnSliceByRef(A: []) ref {
  return A[A.domain.low..A.domain.high/2];
}

proc badReturnAliasLocal() {
  var A: [1..100] int;
  ref B = A;
  return B;
}

proc badReturnAliasSliceLocal() {
  var A: [1..100] int;
  ref B = A[1..5];
  return B;
}

proc badReturnAliasAliasLocal() {
  var A: [1..100] int;
  ref B = A;
  ref C = B;
  return C;
}

proc badReturnSliceSliceLocal() {
  var A: [1..100] int;
  ref B = A[1..5];
  return B[1..2];
}

var a = badReturnRefLocal();
writeln(a);
var b = badReturnRefConstant();
writeln(b);
var c = badReturnSliceLocal();
writeln(c);
var d = badReturnAliasLocal();
writeln(d);
var e = badReturnAliasSliceLocal();
writeln(e);
var f = badReturnAliasAliasLocal();
writeln(f);
var g = badReturnSliceSliceLocal();
writeln(g);

var Data : [1..10] int;
badReturnSliceByRef(Data);

var i = 1;
var A: [1..100] int;

class C {
  var field: [1..10] int;
}

var globalC = new owned C();

proc returnRefGlobal() ref {
  return i;
}

proc returnSliceGlobal() {
  return A[1..5];
}

proc returnAliasGlobal() {
  ref B = A;
  return B;
}

proc returnAliasSliceGlobal() {
  ref B = A[1..5];
  return B;
}

proc returnAliasField() {
  ref B = globalC.field;
  return B;
}

proc returnAliasSliceField() {
  ref B = globalC.field[1..5];
  return B;
}

var a = returnRefGlobal();
writeln(a);
var b = returnSliceGlobal();
writeln(b);
var c = returnAliasGlobal();
writeln(c);
var d = returnAliasSliceGlobal();
writeln(d);
var e = returnAliasField();
writeln(e);
var f = returnAliasSliceField();
writeln(f);

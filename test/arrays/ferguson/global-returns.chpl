var i = 1;
var A: [1..100] int;

proc returnRefGlobal() ref {
  return i;
}

proc returnSliceGlobal() {
  return A[1..5];
}

proc returnAliasGlobal() {
  var B => A;
  return B;
}

proc returnAliasSliceGlobal() {
  var B => A[1..5];
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

var A:[1..4] int;
for i in 1..4 do
  A[i] = i;
var B:[1..4] int;
for i in 1..4 do
  B[i] = 10*i;

proc add(a: int, b: int) {
  writeln("In add");
  return a + b;
}

proc f() {
  writeln("In f");
  return add(A, B);
}

proc g(x) {
  writeln("In g");
  A[2] = 1;

  var C = x;
  writeln(C[2]); // Does this print 21 or 22?
}

g(f());

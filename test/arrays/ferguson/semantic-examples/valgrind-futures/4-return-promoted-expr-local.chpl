var A:[1..4] int;
for i in 1..4 do
  A[i] = i;

proc add(a: int, b: int) {
  writeln("In add");
  return a + b;
}

proc f() {
  writeln("In f");
  var B:[1..4] int;
  for i in 1..4 do
    B[i] = 10*i;
  return add(A, B);
}

proc g(x) {
  writeln("In g");
  A[2] = 1;

  var C = x;
  writeln(C[2]); // does it print out 22 or 21?
}

g(f());

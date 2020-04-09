use samples;
// one defined in samples as new R(1)
printcopies=false;

var A:[1..4] R;
for i in 1..4 do
  A[i] = new R(i);

proc add(a: R, b: R) {
  writeln("In add");
  return new R(a.x + b.x);
}

proc f() {
  writeln("In f");
  var B:[1..4] R;
  for i in 1..4 do
    B[i] = new R(10*i);
  return add(A, B);
}

proc g(x) {
  writeln("In g");
  A[2] = one;
  
  var C = x;
  writeln(C[2].x); // does it output 22 or 21?
}

g(f());

use Time;

const n = 5;

var a: [1..n] int;
var B: [1..n] sync bool;

for i in 1..n do begin with (ref a) {
  a[i] = foo(i);
  B[i].writeEF(true);
}

for i in 1..n {
  B[i].readFE();
  assert(a[i] != 0);

  writeln(a[i]);
}

proc foo(i: int) do return i**2;

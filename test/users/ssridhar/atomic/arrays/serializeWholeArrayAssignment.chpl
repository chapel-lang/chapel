config var n: int = 8;

var D: domain(1) = [1..n];

var A: [D] int;
var B: [D] int;

proc main() {
  for i in D do A(i) = i;

  coforall i in 1..1000 do
    atomic serial true do B = A;

  writeln("A = ", A);
  writeln("B = ", B);
}
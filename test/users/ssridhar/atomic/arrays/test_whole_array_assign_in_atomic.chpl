config var n: int = 8;

var D: domain(1) = [1..n];

var A: [D] int;
var B: [D] int;

[i in D] A(i) = i;

coforall i in 1..1000 do
  atomic B = A;

writeln("B = ", B);

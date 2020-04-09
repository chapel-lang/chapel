var A: [1..3] real;
var B: [1..3] complex;
var C: [1..3] int;
var D: [1..3] string;

for t in zip(A, B, C, D) do
  writeln(t);

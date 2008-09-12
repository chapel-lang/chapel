
config const n = 16;

const radix = 4;

var A: [i in 1..n] int = i;
var B: [1..radix] int;

writeln("A is: ", A);

for span in 1..n/radix {
  B = A[1.. by span #radix];
  writeln("B is: ", B);
}

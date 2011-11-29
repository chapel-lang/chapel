var A: [1..3] real = [i in 1..3] i;

writeln("A is: ", A);

for i in 1..4 do
  writeln("A[",i,"..] is: ", A[i..]);

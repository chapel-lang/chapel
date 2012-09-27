var A, B, C, D: [1..3] real;
A = 1..3;
B = 4..6;
C = 7..9;
D = 10..12;

for t in zip(A, B, C, D) do
  writeln(t);

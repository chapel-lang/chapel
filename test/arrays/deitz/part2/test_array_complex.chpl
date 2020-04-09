var A: [1..5] complex;

writeln(A);

for i in 1..5 do
  A(i) = 1.0i * (i + 0.0i);

for i in 1..5 do
  A(i) = A(i) - (1.0 + 2.0i);

writeln(A);

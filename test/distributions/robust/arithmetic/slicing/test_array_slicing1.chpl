use driver_arrays;

for e in A1D do e = next();
for e in A2D do e = next();
for e in A3D do e = next();

writeln(A1D);
A1D(5..7) = 1;
writeln(A1D);

writeln(A2D);
A2D(1..n2, 2) = 2;
A2D(2, 1..n2) = 3;
writeln(A2D);

writeln(A3D);
for e in A3D(2..n3-1, 2..n3-1, 2..n3-1) do
  e = next();
writeln(A3D);
A3D(Space3.expand(-1)) = 0;
writeln(A3D);

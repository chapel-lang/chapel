use driver_arrays;

for e in A1D do e = next();
for e in A2D do e = next();
for e in A3D do e = next();
for e in A4D do e = next();

forall e in A1D(Space1.expand(-1)) do e = 0;
forall e in A2D(Space2.expand(-1)) do e = 0;
forall e in A3D(Space3.expand(-1)) do e = 0;
forall e in A4D(Space4.expand(-1)) do e = 0;

writeln(A1D);
writeln(A2D);
writeln(A3D);
writeln(A4D);

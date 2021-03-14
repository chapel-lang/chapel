use driver_arrays;

for e in A1D do e = next();
for e in A2D do e = next();
for e in A3D do e = next();
for e in A4D do e = next();

writeln(+ reduce A1D);
writeln(+ reduce A2D);
writeln(+ reduce A3D);
writeln(+ reduce A4D);

writeln(+ reduce [i in Dom1D] A1D(i));
writeln(+ reduce [i in Dom2D] A2D(i));
writeln(+ reduce [i in Dom3D] A3D(i));
writeln(+ reduce [i in Dom4D] A4D(i));

writeln(+ reduce [i in Space1] A1D(i));
writeln(+ reduce [i in Space2] A2D(i));
writeln(+ reduce [i in Space3] A3D(i));
writeln(+ reduce [i in Space4] A4D(i));

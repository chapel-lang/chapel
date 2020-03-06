use driver_arrays;

writeln(Dom1D.size);
writeln(Dom2D.size);
writeln(Dom3D.size);
writeln(Dom4D.size);
writeln(Dom2D32.size);

writeln(A1D.numElements);
writeln(A2D.numElements);
writeln(A3D.numElements);
writeln(A4D.numElements);
writeln(A2D32.numElements);

var D: domain(3) dmapped Dist3D = {1..7, 1..11, 1..13};
writeln(D.size);

use driver_arrays;

writeln(Dom1D.numIndices);
writeln(Dom2D.numIndices);
writeln(Dom3D.numIndices);
writeln(Dom4D.numIndices);
writeln(Dom2D32.numIndices);

writeln(A1D.numElements);
writeln(A2D.numElements);
writeln(A3D.numElements);
writeln(A4D.numElements);
writeln(A2D32.numElements);

var D: domain(3) dmapped Dist3D = {1..7, 1..11, 1..13};
writeln(D.numIndices);

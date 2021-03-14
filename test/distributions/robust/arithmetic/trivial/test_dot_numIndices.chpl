use driver_arrays;

writeln(Dom1D.size);
writeln(Dom2D.size);
writeln(Dom3D.size);
writeln(Dom4D.size);
writeln(Dom2D32.size);

writeln(A1D.size);
writeln(A2D.size);
writeln(A3D.size);
writeln(A4D.size);
writeln(A2D32.size);

var D: domain(3) dmapped Dist3D = {1..7, 1..11, 1..13};
writeln(D.size);

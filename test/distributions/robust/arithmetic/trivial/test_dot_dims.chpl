use driver_arrays;

writeln(Dom1D.dims());
writeln(Dom2D.dims());
writeln(Dom3D.dims());
writeln(Dom4D.dims());
writeln(Dom2D32.dims());

writeln(A1D.dims());
writeln(A2D.dims());
writeln(A3D.dims());
writeln(A4D.dims());
writeln(A2D32.dims());

writeln(Dom1D.getIndices());
writeln(Dom2D.getIndices());
writeln(Dom3D.getIndices());
writeln(Dom4D.getIndices());
writeln(Dom2D32.getIndices());

writeln(A1D.indices);
writeln(A2D.indices);
writeln(A3D.indices);
writeln(A4D.indices);
writeln(A2D32.indices);

var D: domain(2) dmapped Dist2D = {200..300, 400..500};
var A: [D] real;

writeln();
writeln(A.domain.dims());
writeln(A.domain.getIndices());
writeln(A.dims());
writeln(A.indices);

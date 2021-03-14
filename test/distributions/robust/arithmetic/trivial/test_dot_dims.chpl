use driver_domains;

writeln(Dom1D.dims());
writeln(Dom2D.dims());
writeln(Dom3D.dims());
writeln(Dom4D.dims());
writeln(Dom2D32.dims());

var D: domain(2) dmapped Dist2D = {200..300, 400..500};
var A: [D] real;

writeln(A.domain.dims());

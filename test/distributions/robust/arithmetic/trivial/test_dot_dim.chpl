use driver_domains;

config const zero = 0, one = 1, two = 2, three = 3;

// param dimensions
writeln(Dom1D.dim(0));
writeln(Dom2D.dim(0));
writeln(Dom2D.dim(1));
writeln(Dom3D.dim(0));
writeln(Dom3D.dim(1));
writeln(Dom3D.dim(2));
writeln(Dom4D.dim(0));
writeln(Dom4D.dim(1));
writeln(Dom4D.dim(2));
writeln(Dom4D.dim(3));
writeln(Dom2D32.dim(0));
writeln(Dom2D32.dim(1));
writeln({Dom1D.dim(0), Dom2D.dim(1), Dom3D.dim(2), Dom4D.dim(3)});

writeln();

// non-param dimensions
writeln(Dom1D.dim(zero));
writeln(Dom2D.dim(zero));
writeln(Dom2D.dim(one));
writeln(Dom3D.dim(zero));
writeln(Dom3D.dim(one));
writeln(Dom3D.dim(two));
writeln(Dom4D.dim(zero));
writeln(Dom4D.dim(one));
writeln(Dom4D.dim(two));
writeln(Dom4D.dim(three));
writeln(Dom2D32.dim(zero));
writeln(Dom2D32.dim(one));
writeln({Dom1D.dim(zero), Dom2D.dim(one), Dom3D.dim(two), Dom4D.dim(three)});

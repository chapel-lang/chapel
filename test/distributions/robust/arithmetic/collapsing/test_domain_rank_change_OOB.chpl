use driver;

var D = [1..5, 1..5, 1..5] distributed Dist3D;

writeln(D[1..5, 1..5, 0]);
writeln(D[1..5, 0, 1..5]);
writeln(D[0, 1..5, 1..5]);
writeln();

writeln(D[1..5,0,0]);
writeln(D[0, 1..5, 0]);
writeln(D[0, 0, 1..5]);

use BlockDist;

var A: [[1..4, 1..4] distributed Block(2,bbox=[1..4,1..4])] real;

writeln("Distribution Representation");
writeln();
A.domain.dist.displayRepresentation();

writeln();
writeln("Domain");
writeln();
writeln(A.domain);
writeln();
writeln("Domain Representation");
writeln();
A.domain.displayRepresentation();

writeln();
writeln("Array");
writeln();
writeln(A);
writeln();
writeln("Array Representation");
writeln();
A.displayRepresentation();

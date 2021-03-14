use BlockDist;

var A: [{1..4, 1..4} dmapped Block(boundingBox={1..4,1..4},
                                       dataParTasksPerLocale=2,
                                       dataParIgnoreRunningTasks=true)] real;

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

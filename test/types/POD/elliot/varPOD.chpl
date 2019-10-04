// Check that POD can be used correctly in variable type declarations

var podI: POD = 1;
writeln(podI);

record R { var i: int; }
var podR: POD = new R(1);
writeln(podR);

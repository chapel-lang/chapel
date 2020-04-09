// Check that POD can be used correctly in variable type declarations

var podI: chpl_anyPOD = 1;
writeln(podI);

record R { var i: int; }
var podR: chpl_anyPOD = new R(1);
writeln(podR);

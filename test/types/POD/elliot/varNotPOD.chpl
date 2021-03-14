// Check that a non-POD type can't be stored in a var with POD type

record R { var A: [1..1]int; }
var podR: chpl_anyPOD = new R();
writeln(podR);

// Equality tests for tuples

var A = (1,2,3);
var B = (1,2,3);
var C = (1,2,4);
var D = (1,2,3,4);

writeln(A == B);
writeln(A != B);

writeln(A == C);
writeln(A != C);

writeln(A == D);
writeln(A != D);

writeln(D == A);
writeln(D != A);

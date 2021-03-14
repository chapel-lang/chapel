// This code should produce an error for each 'ref' declaration.

// Original motivation.
var A: [1..2] int;
ref D1 = A.localSubdomain();
writeln(D1);

// A simplification of the above.
proc dom() return {3..4};
ref D2 = dom();
writeln(D2);

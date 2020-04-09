var D: domain(string) = ("a", "b", "c", "d");

var A:[D] int;

A["a"] = 1;
A["b"] = 2;
A["c"] = 3;
A["d"] = 4;

writeln(+ reduce A);

// The below results in a compiler error.
var B = A[D];
writeln(+ reduce B);


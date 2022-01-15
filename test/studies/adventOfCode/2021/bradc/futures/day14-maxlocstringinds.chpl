const D: domain(string) = {"Hi", "Bye"};
var A: [D] real;

A["Hi"] = 100;

var (a,d) = maxloc reduce zip(A, D);

writeln("Got: ", (a,d));

use Functions;

var f: proc(i: int): int = proc(i: int) { return i+1; };

writeln(f(10));

// TODO: Formal name discarding.
var g = proc(i:int) { return i+2; };
f = g;

writeln(f(10));


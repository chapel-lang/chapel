use Functions;

proc testme(i : int = 4) { return i; }

writeln(testme());

var f : proc(i: int): int = testme;

writeln(f(5));

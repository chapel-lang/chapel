use Functions;

proc testme(i : int = 4) { return i; }

writeln(testme());

var f : func(int,int) = testme;

writeln(f(5));
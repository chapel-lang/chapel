var A: [1..4] int;

writeln(A);

proc f() ref do return A;

f() = 1;

writeln(A);

proc g() do return A;

g() = 2;

writeln(A);

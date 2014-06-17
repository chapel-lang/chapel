var A: [1..4] int;

writeln(A);

proc f() var return A;

f() = 1;

writeln(A);

proc g() return A;

g() = 2;

writeln(A);

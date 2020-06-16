var x = ((1, 1), 1);
x(0)(0) = 2;
writeln(x);

proc foo() ref return x(0)(0);
foo() = 3;
writeln(x);


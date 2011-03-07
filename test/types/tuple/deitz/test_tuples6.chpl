var x = ((1, 1), 1);
x(1)(1) = 2;
writeln(x);

proc foo() var return x(1)(1);
foo() = 3;
writeln(x);


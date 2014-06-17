extern proc foo(x: real): real;
var A: [1..10] real = [i in 1..10] i/10.0;
writeln(foo(A));


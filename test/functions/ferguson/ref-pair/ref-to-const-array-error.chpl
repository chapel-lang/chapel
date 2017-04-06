proc foo(const X: [] real) {
   ref Y = X;
   Y[1] = 42;
}

var A: [1..10] real;
foo(A);
writeln(A);

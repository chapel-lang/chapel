proc f(const x) {
  x(1)[1] = 1;  // this should be an error
}

proc g(x) {
  x(1)[1] = 1;
}



var A:[1..2] int;
f((A, A));
writeln(A);

const B:[1..2] int;
g((B, B)); // This should be an error.
writeln(B);


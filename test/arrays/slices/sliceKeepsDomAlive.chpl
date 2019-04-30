var A: [1..10, 1..10] real = [(i,j) in {1..10, 1..10}] i + j / 10.0;

proc foo() {
  const D = {1..3, 1..3};
  return A[D];
}

ref B = foo();
writeln(B);

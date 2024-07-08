var A, B, C: [0..<10] real = 1.0;

proc foo(x: real, y: real, z: real) {
  return x**y + 10*z;
}

C = foo(A, 2, B);
writeln("after C = foo(A, 2, B); C = ", C);

forall (c, a, b) in zip(C, A, B) do
  c = foo(a, 2, b);

C = A**2 + 10*B;

writeln("after C = A**2 + 10*B; C = ", C);

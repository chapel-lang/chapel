class C { var x: int; }

proc main() {
  var X: [1..2] owned C?;
  X[1] = new owned C(1);
  X[2] = new owned C(2);

  const Y = X;

  const Z = Y;

  writeln(X);
  writeln(Y);
  writeln(Z);
}

config const n = 10;

config const alpha = 10;

on here.getChild(1) {
  var A: [1..n] int;
  var B: [1..n] int;
  var C: [1..n] int;

  // Each of the following should cause a kernel launch
  A = 1;                            writeArr(A);
  B = 2;                            writeArr(B);
  C = 3;                            writeArr(C);
  A = B + alpha * C;                writeArr(A);
  A = A + A + A + B + B + C;        writeArr(A);
  A = B * C + A;                    writeArr(A);
  A = foo(A);                       writeArr(A);

}

proc foo(a: int) {
  return a+1;
}

proc writeArr(A) {
  // normally we can just do writeln(A), but we don't have a good way of having
  // verbose GPU execution. So, if we do that the output is just too messy with
  // verbose. If we had verbosity for only launches, things would have been much
  // easier.
  write("Array: ");
  for a in A do
    write(a, " ");
  writeln();
}

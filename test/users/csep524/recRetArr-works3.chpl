var A: [1..3] real = [i in 1..3] i;

foo(A);

proc fooHelp(x, y) {
  foo(x, y);
}

proc foo(A: [], depth: int = 3) {
  var B = A;

  writeln("In foo(), A is: ", A);
  if (depth) then
    fooHelp(A, depth-1);
  return B;
}

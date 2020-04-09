var A: [1..3] real = [i in 1..3] i;

foo(A, 3);

proc foo(A: [], depth: int = 3) {
  var B = A;

  writeln("In foo(), A is: ", A);
  if (depth) then
    foo(A, depth-1);
  return B;
}

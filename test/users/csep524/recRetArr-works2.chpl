var A: [1..3] real = [i in 1..3] i;

foo(A);

proc foo(A: [], depth: int = 3) {


  writeln("In foo(), A is: ", A);
  if (depth) then
    foo(A, depth-1);

}

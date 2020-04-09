/*
 * querying the type of an argument
 */
proc sumOfSquares(x: ?t, y: t): t {
  return x**2 + y**2;
}

writeln("sumOfSquares(1, 2) = ",     sumOfSquares(1, 2));
writeln("sumOfSquares(1.1, 2.2) = ", sumOfSquares(1.0, 2.0));
writeln();


/*
 * querying the width of an arguemnt
 */
proc printShifts(x: int(?width)) {
  writeln("the value ", x, "...");
  for i in 1..(width - 1) do
    writeln("  ...shifted ", i, " times: ", x >> i);
  writeln();
}

printShifts(0x1001:int(16));
printShifts(0x80:int(8));


/*
 * querying the domain of an array argument 
 */
proc printIndices(A: [?D]) {
  for i in D {
    writeln("i is: ", i, ", A(i) is: ", A(i));
  }
}

var A: [1..10] real = [i in 1..10] i;
printIndices(A);

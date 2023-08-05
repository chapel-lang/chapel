proc makeInputArray(n: int) {
  var A:[1..n] int = 1..n;
  return A;
}

proc mutateAndSum(ref A) {
  for x in A {
    x *= 2;
  }
  return + reduce A;
}

var A = makeInputArray(1000);
A[1..10] = 4; // this should not be an error

var sum = mutateAndSum(makeInputArray(1000)); // this should be an error
writeln(sum);

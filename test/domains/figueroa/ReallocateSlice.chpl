// find out what happens when a slice is passed in to another function and
// that function changes the domain of the array from which the slice was taken

const N = 5;
var D = {1..N};
var A: [D] int = 7;
var sum = -3;

proc foo (X) {
  var sum = 0;
  D = {0..1};
  for i in X.domain do sum += X[i]*i;
  return sum;
}

proc bar () {
  sum += foo(A[A.domain.expand(-1)]);
}

bar();

writeln("sum is ", sum);
writeln("A is ", A);

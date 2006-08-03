fun crossSum(is: int...?k) {
  var sum: int;
  for param j in 1..k do
    sum += is(j);
  return sum;
}

config const n = 5;

var A : [1..n, 1..n] float = crossSum[n*(0..n-1), 0..n-1];

writeln(A);

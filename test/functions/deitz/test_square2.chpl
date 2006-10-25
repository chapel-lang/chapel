def sum(is: int...?k) {
  var result: int;
  for param j in 1..k do
    result += is(j);
  return result;
}

config const n = 5;

var A : [1..n, 1..n] real = sum[n*(0..n-1), 0..n-1];

writeln(A);

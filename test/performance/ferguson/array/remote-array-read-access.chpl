use CommUtil;

config const n = 100000;
var A: [1..n] int;

for a in A {
  a = 1;
}

start();

on Locales[1] {
  var sum = 0;
  for i in 1..n {
    sum += A[i];
  }
  assert(sum == n);
}

stop();

assert(A[1] == 1);
assert(A[n] == 1);

report(maxPuts=0, maxOns=1);

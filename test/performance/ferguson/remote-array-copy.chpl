use CommUtil;


config const n = 100000;
var A: [1..n] int;
var B: [1..n] int;

for i in 1..n {
  A[i] = i;
}

start();

on Locales[1] {
  for i in 1..n {
    B[i] = A[i];
  }
}

stop();

assert(B[1] == 1);
assert(B[n] == n);

report(maxOns=1);

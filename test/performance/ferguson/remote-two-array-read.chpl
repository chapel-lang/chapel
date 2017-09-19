use CommUtil;


config const n = 100000;
var A: [1..n] int;
var B: [1..n] int;

for a in A {
  a = 1;
}
for b in B {
  b = -1;
}

var sumToPrint = 0;

start();

on Locales[1] {
  var sum = 0;
  for (a,b) in zip(A,B) {
    sum += a;
    sum += b;
  }
  sumToPrint = sum;
}

stop();

writeln(sumToPrint);
writeln(A[1], " ", B[1]);
writeln(A[n], " ", B[n]);

report(maxPuts=1, maxOns=1);

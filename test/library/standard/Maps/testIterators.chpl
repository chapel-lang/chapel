use Maps, BigInteger;
use utilFunctions;

var fac = new map(int, bigint);

for i in 1..100 {
  fac[i] = factorial(i);
}

var A: [1..fac.size] (int, bigint);
for key in fac {
  A[key] = (key, fac[key]);
}
writeln(A);

var B: [1..fac.size] (int, bigint);
for item in fac.items() {
  B[item(1)] = item;
}
writeln(B);

var C: [1..fac.size] bigint;
for (val, i) in zip(fac.values(), 1..) {
  C[i] = val;
}
writeln(C.sorted());

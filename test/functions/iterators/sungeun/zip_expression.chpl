var A, B, C: [1..5] int;

var z0 = zip(6..10, 11..15);
var z1 = zip(1..5, z0);
var z2 = zip(C, z1);
var z3 = zip(B, z2);

for (a, (b, (c, (i, (j, k))))) in zip(A, z3) {
  a = i;
  b = j;
  c = k;
}

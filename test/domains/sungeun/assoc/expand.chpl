config param parSafe = true;
config const n = 7;

proc my_expand(D, off) {
  return D.expand(off);
}

proc dit(D, lo, hi) {
  writeln("D: ", D);
  for i in lo..hi do
    writeln(my_expand(D, i));
}

var D1: domain(int, parSafe=parSafe);
D1 += n+2;
D1 += n+1;
D1 += n;
D1 += n-1;
D1 += n-2;

dit(D1, -n/2, n/2);

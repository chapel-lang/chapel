config const n = 7;

proc my_interior(D, off) {
  return D.interior(off);
}

proc dit(D, lo, hi) {
  writeln("D: ", D);
  for i in lo..hi do
    writeln(my_interior(D, i));
}

const D = {1..n*n};

var D1: sparse subdomain(D);
D1 += n+2;
D1 += n+1;
D1 += n;
D1 += n-1;
D1 += n-2;

dit(D1, -n/2, n/2);

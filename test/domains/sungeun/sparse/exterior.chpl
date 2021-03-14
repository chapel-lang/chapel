config const n = 7;

proc my_exterior(D, off) {
  return D.exterior(off);
}

proc dit(D, lo, hi) {
  writeln("D: ", D);
  for i in lo..hi do
    writeln(my_exterior(D, i));
}

const D = {1..n*n};

var D1: sparse subdomain(D);
D1 += n+2;
D1 += n+1;
D1 += n;
D1 += n-1;
D1 += n-2;

dit(D1, -n/2, n/2);

config param parSafe = true;
config const n = 7;

//
// NOTE: We should eventually make this legal
//
proc my_translate(D, off) {
  return D.translate(off);
}

proc dit(D, lo, hi) {
  writeln("D: ", D);
  for i in lo..hi do
    writeln(my_translate(D, i));
}

var D1: domain(int, parSafe=parSafe);
D1 += n+2;
D1 += n+1;
D1 += n;
D1 += n-1;
D1 += n-2;

dit(D1, -n/2, n/2);

use Random, Random.PCGRandomLib;

config const seed = 42;

// Generate a table of increasing incs.
config param width=4;

config const n = 12;
config const max:uint(32) = 2**31 + 1;


writeln("Table showing how bounded rand is implemented");

var rng0:pcg_setseq_64_xsh_rr_32_rng;
var inc0 = pcg_getvalid_inc(1);

rng0.srandom(seed:uint(64), inc0);

var rngs:width*pcg_setseq_64_xsh_rr_32_rng;
var incs:width*uint(64);

for j in 0..#width {
  incs(j) = pcg_getvalid_inc((101+4*j):uint(64));
  rngs(j).srandom(seed:uint(64), incs(j));
}

for i in 0..#n {
  writef("% 12u ", rng0.random(inc0));
  for j in 0..#width {
    var r = rngs(j).random(incs(j));
    writef("% 12u ", r);
  }
  writeln();
}

var rs = new randomStream(seed=seed, eltType=uint(32));

var got:[0..#n] uint(32);

writeln("Numbers first time through");
for i in 0..#n {
  var num = rs.next(0, max);
  writeln(num);
  assert(num <= max);
  got[i] = num;
}

writeln("Numbers with skipTo each time");
for i in 0..#n {
  rs.skipTo(i);
  var num = rs.next(0, max);
  writeln(num);
  assert(num <= max);
  assert(got[i] == num);
}

var rs2 = new randomStream(seed=seed, eltType=uint(64));

var max2:uint = (2**32 + max):uint;

var got2:[0..2*n] uint;

writeln("Numbers first time through - 64");
for i in 0..#n {
  var num = rs2.next(0, max2);
  writeln(num);
  assert(num <= max2);
  got2[i] = num;
}
got2[n] = rs2.next();
got2[n+1] = rs2.next(0, 2**32);
got2[n+2] = rs2.next();

writeln("Numbers with skipTo each time - 64");
for i in 0..#n {
  rs2.skipTo(i);
  var num = rs2.next(0, max2);
  writeln(num);
  assert(num <= max2);
  assert(got2[i] == num);
}
rs2.skipTo(n);
var num = rs2.next();
assert(got2[n] == num);

rs2.skipTo(n + 1);
num = rs2.next(0, 2**32);
assert(got2[n+1] == num);

rs2.skipTo(n + 2);
num = rs2.next();
assert(got2[n+2] == num);

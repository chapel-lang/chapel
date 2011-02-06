use driver;

config const debug = false;
config const offset = 7;
config const n = 1<<16;

const m: int = 13;
var keys: [1..m] int;
for i in 1..m do
  keys[i] = 2*i+offset;

for i in 1..n do
  foo(i);

proc foo(i: int) {
  const modi = i % m;
  var D: domain(intType) dmapped DistIntType;

  for i in 1..modi do
    D += keys[i];
  if debug then writeln(D);
  const sum = + reduce [i in D] i;

  const expectedSum = + reduce [j in 1..modi] 2*j+offset;

  if sum != expectedSum then
    halt("FAIL -- ", sum, " != ", expectedSum);
}

writeln("SUCCESS");

use driver, driver_arrays;

config const debug = false;
config const n = 1<<13;

const m: int = 13;
var keys: [1..m] string;
keys[1] = "k1";
keys[2] = "k2";
keys[3] = "k3";
keys[4] = "k4";
keys[5] = "k5";
keys[6] = "k6";
keys[7] = "k7";
keys[8] = "k8";
keys[9] = "k9";
keys[10] = "k10";
keys[11] = "k11";
keys[12] = "k12";
keys[13] = "k13";

for i in 1..n do
  foo(i);

proc foo(i: int) {
  var D: domain(stringType) dmapped DistStringType;
  var AA: [D] int;

  for i in 1..m {
    D += keys[i];
    AA(keys[i]) = i;
  }

  if debug then writeln(D);
  if debug then writeln(AA);
  const sum = + reduce AA;

  const expectedSum = + reduce [i in 1..m] i;
  if sum != expectedSum then
    halt("FAIL -- ", sum, " != ", expectedSum);
}

writeln("SUCCESS");

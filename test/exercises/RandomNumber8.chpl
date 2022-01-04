const randNumDom = {1..3};
var seed: [randNumDom] sync int = 1;
const multipliers: [randNumDom] int = (16807, 397204094, 950706376),
      modulus = 2147483647;

proc RandomNumber (x_n, multiplier) {
  // The following calculation must be done in at least 46-bit arithmetic!
  return (x_n.readFE() * multiplier % modulus) : int(32);
}

proc RealRandomNumber (i) {
  var n = RandomNumber(seed[i], multipliers[i]);
  seed[i].writeEF(n);
  return (n-1) / (modulus-2) : real;
}

config const numberOfIterations = 10000;

proc test {
  coforall i in 1..numberOfIterations do
    seed[1] = RandomNumber(seed[1], multipliers[1]);

  writeln ("After 10000 iterations, RandomNumber should return 1043618065");
  writeln ("After ", numberOfIterations, " iterations, RandomNumber returns ",
           seed[1].readFF()); // this reads the value of seed, but does not mark it empty
  writeln ("RealRandomNumber returns ", RealRandomNumber(1));
}

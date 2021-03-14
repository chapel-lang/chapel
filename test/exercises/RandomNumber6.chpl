use Time;

config var seed = 1;
const multiplier = 16807,
      modulus = 2147483647;

proc RandomNumber (x_n) {
  sleep(1);
  // The following calculation must be done in at least 46-bit arithmetic!
  return x_n:int(64) * multiplier % modulus;
}

proc RealRandomNumber () {
  // Cast RandomNumber's return value to whatever seed's type is
  seed = RandomNumber(seed) : seed.type;
  return (seed-1) / (modulus-2) : real;
}

config const numberOfIterations = 10000;

proc test {
  coforall i in 1..numberOfIterations with (ref seed) do
    // Cast RandomNumber's return value to whatever seed's type is
    seed = RandomNumber(seed) : seed.type;

  writeln ("After 10000 iterations, RandomNumber should return 1043618065");
  writeln ("After ", numberOfIterations, " iterations, RandomNumber returns ", seed);
  writeln ("RealRandomNumber returns ", RealRandomNumber());
}

test;

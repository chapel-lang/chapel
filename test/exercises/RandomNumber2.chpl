proc RandomNumber (x_n) {
  const multiplier = 16807,
        modulus = 2147483647;
  // The following calculation must be done in at least 46-bit arithmetic!
  return x_n:int(64) * multiplier % modulus;
}

var seed = 1;
config const numberOfIterations = 10000;

for i in 1..numberOfIterations do
  // Cast RandomNumber's return value to whatever seed's type is
  seed = RandomNumber(seed) : seed.type;

writeln ("After 10000 iterations, RandomNumber should return 1043618065");
writeln ("After ", numberOfIterations, " iterations, RandomNumber returns ", seed);

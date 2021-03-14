proc RandomNumber (x_n) {
  const multiplier = 16807,
        modulus = 2147483647;
  // The following calculation must be done in at least 46-bit arithmetic!
  return x_n:int(64) * multiplier % modulus;
}

writeln ("RandomNumber returns ", RandomNumber(1));

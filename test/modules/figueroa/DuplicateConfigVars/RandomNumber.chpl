class Random {
  // Avoid negative or very large seeds.
  var seed: uint = 1;
  const multiplier: int(64) = 16807,
        modulus: int(64) = 2147483647;
  def RandomNumber () {
    // The following calculation must be done in at least 46-bit arithmetic!
    seed = (seed * multiplier % modulus) : uint;
    return (seed-1) / (modulus-2) : real;
  }
  def RandomNumber (n) {
    var R: [1..n] real;
    // The calls to RandomNumber() cannot be done in parallel,
    // but this shows one way of conveniently filling up an entire array.
    [r in R] r = RandomNumber();
    return R;
  }
}

config const seed: uint = 13;
config const arraySize = 10;

def main {
  var r = Random (seed);
  var rArray: [1..arraySize] real;
  rArray = r.RandomNumber(arraySize);
  writeln (rArray);
}

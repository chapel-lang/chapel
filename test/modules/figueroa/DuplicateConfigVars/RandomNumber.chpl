class Random {
  // Avoid negative or very large seeds.
  var seed: uint(32) = 1;
  proc RandomNumber () {
    const multiplier: int(64) = 16807,
          modulus: int(64) = 2147483647;
    // The following calculation must be done in at least 46-bit arithmetic!
    seed = (seed * multiplier % modulus) : uint(32);
    return (seed-1) / (modulus-2) : real;
  }
  proc RandomNumber (n) {
    var rs = for 1..n do RandomNumber();
    return rs;
  }
}

config const seed: uint(32) = 13,
             arraySize = 10;

proc main {
  var r = new owned Random (seed);
  var rArray: [1..arraySize] real;

  rArray = r.RandomNumber(arraySize);

  writeln (rArray);
}

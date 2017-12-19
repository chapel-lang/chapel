class Random {
  // Avoid negative or very large seeds.
  var seed: uint(32) = 1;
  proc RandomNumber () {
    /*const multiplier: int(64) = 16807,
          modulus: int(64) = 2147483647;
    seed = (seed * multiplier % modulus) : uint(32);
    return (seed-1) / (modulus-2) : real;*/
    var ret = seed;  // Using a a bogus RNG to make checking .good easier
    seed = seed + 1;
    return ret;
  }
  proc RandomNumber (n) {
    return for 1..n do RandomNumber();
  }
}

config const seed: uint(32) = 13,
             arraySize = 10;

proc main {
  var r = new Random (seed);
  var rArray: [1..arraySize] real;
  rArray = r.RandomNumber(arraySize);
  writeln (rArray);
  delete r;
}

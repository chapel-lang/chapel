proc main() {
  use Random;

  var Vector1 = {1..20};
  var Vec1 : [Vector1] real;

  var rng = createRandomStream(real, 314159265, algorithm=RNG.NPB);

  rng.fillRandom(Vec1);
  writeln(Vec1);

  fillRandom(Vec1,314159265, algorithm=RNG.NPB);
  writeln(Vec1);
  fillRandom(Vec1,314159265, algorithm=RNG.NPB);
  writeln(Vec1);
}


module A2 {
  use Random;

  config param useNPB = true;
  config param rtype  = if useNPB then RNG.NPB else RNG.PCG;

  proc main() {
    var rnd  = createRandomStream(real, seed=314159265, algorithm=rtype);

    writeln(rnd);
  }
}

module A2 {
  use Random;

  config param useNPB = true;
  config param rtype  = if useNPB then RNG.NPB else RNG.PCG;

  proc main() {
    var rnd  = makeRandomStream(seed=314159265, algorithm=rtype);

    writeln(rnd);

    delete rnd;
  }
}

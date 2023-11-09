module A2 {
  use Random;

  proc main() {
    var rnd  = new randomStream(real, seed=314159265);

    writeln(rnd);
  }
}

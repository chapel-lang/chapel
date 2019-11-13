module A2 {
  use Random;

  proc main() {
    var rnd  = RandomStream.create(real, seed=314159265);

    writeln(rnd);
  }
}

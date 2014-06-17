module A2 {
  use Random;
  proc main() {
    var rnd : RandomStream = new RandomStream(seed=314159265);
    writeln(rnd);
  }
}

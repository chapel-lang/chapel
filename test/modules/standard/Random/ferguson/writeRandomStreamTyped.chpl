module A2 {
  use Random;
  proc main() {
    var rnd1 : NPBRandomStream = new NPBRandomStream(seed=314159265);
    writeln(rnd1);
    var rnd2 : PCGRandomStream  = new PCGRandomStream(seed=314159265);
    writeln(rnd2);
  }
}

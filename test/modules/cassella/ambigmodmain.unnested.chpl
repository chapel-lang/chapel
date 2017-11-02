module M {
  proc main() { writeln("M"); }
}

module X {
  module M {
    proc main() { writeln("X.M"); }
  }
}

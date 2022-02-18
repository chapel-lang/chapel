module M {
  var X: int;
  proc f() { writeln("M.f"); }
}

module N {
  var X: real;
  proc f() { writeln("N.f"); }

  proc main() {
    use M;
    writeln(X); // prints out 0 so refers to M.X
    f(); // ambiguity error
  }
}

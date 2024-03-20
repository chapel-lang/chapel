module M {
  var x: int = 42;
}

module N {
  var x: real;
  proc main() {
    import M.x;
    writeln(x);
  }
}


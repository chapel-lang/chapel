module red {
  var x : int = 8;

  proc foo(i : int) {
    writeln(i);
  }
}

module blue {
  var y : int = 4;

  use red;

  proc main() {
    foo(y);
    foo(x);
    foo(16);
  }
}

module red {
  var x : int = 8;
}

module blue {
  var y : int = 4;

  use red;

  proc main() {
    writeln("y is ", y);
    writeln("x is ", x);
  }
}

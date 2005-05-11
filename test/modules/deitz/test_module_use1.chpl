module red {
  var x : integer = 8;
}

module blue {
  var y : integer = 4;

  use red;

  function main() {
    writeln("y is ", y);
    writeln("x is ", x);
  }
}

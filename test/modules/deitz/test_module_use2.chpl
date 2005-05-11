module red {
  var x : integer = 8;

  function foo(i : integer) {
    writeln(i);
  }
}

module blue {
  var y : integer = 4;

  use red;

  function main() {
    foo(y);
    foo(x);
    foo(16);
  }
}

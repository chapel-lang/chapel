module red {
  var x : int = 8;

  function foo(i : int) {
    writeln(i);
  }
}

module blue {
  var y : int = 4;

  use red;

  function main() {
    foo(y);
    foo(x);
    foo(16);
  }
}

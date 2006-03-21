module red {
  var x : int = 8;

  fun foo(i : int) {
    writeln(i);
  }
}

module blue {
  var y : int = 4;

  use red;

  fun main() {
    foo(y);
    foo(x);
    foo(16);
  }
}

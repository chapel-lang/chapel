module red {
  var x : int = 8;

  module green {
    fun bar() {
      writeln(x);
    }
  }

  fun foo(i : int) {
    writeln(i);
    use green;
    bar();
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

module red {
  var x : int = 8;

  def foo(i : int) {
    writeln(i);
  }
}

module blue {
  var y : int = 4;

  use red;

  def main() {
    foo(y);
    foo(x);
    foo(16);
  }
}

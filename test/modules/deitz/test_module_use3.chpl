module red {
  var x : int = 8;

  module green {
    def bar() {
      writeln(x);
    }
  }

  def foo(i : int) {
    writeln(i);
    use green;
    bar();
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

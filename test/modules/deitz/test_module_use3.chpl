module red {
  var x : int = 8;

  module green {
    proc bar() {
      writeln(x);
    }
  }

  proc foo(i : int) {
    writeln(i);
    use this.green;
    bar();
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

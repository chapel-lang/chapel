module red {
  var x : int = 8;

  module green {
    proc bar() {
      import red.x;
      writeln(x);
    }
  }

  proc foo(i : int) {
    writeln(i);
    use green;
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

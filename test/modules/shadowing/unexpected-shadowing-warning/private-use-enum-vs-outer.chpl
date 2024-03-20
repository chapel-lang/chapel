module N {
  enum myEnum { x = 1 }

  var x: real;
  proc main() {
    private use myEnum;
    writeln(x);
  }
}


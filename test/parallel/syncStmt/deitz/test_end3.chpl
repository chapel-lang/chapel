proc foo() {
  var s: sync int;
  sync {
    begin {
      s.readFE();
      writeln("2. hello, world");
    }
    begin {
      writeln("1. hello, world");
      s = 1;
    }
  }
}

foo();

writeln("3. hello, world");

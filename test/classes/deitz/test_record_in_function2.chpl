def foo() {
  record bar {
    var x : int = 2;
    def f() {
      writeln(x);
    }
  }
  var b : bar;
  writeln(b);
  b.f();
}

foo();

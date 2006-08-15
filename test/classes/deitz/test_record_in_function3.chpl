def foo() {
  var y : int = 3;
  record bar {
    var x : int = 2;
    def f() {
      writeln(x);
      writeln(y);
    }
  }
  var b : bar;
  writeln(b);
  b.f();
}

foo();

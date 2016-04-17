proc foo() {
  record bar {
    var x : int = 2;
    proc f() {
      writeln(x);
    }
  }
  var b : bar;
  writeln(b);
  b.f();
}

foo();

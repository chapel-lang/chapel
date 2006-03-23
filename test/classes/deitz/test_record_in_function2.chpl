fun foo() {
  record bar {
    var x : int = 2;
    fun f() {
      writeln(x);
    }
  }
  var b : bar;
  writeln(b);
  b.f();
}

foo();

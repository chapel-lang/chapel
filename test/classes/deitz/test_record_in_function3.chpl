fun foo() {
  var y : int = 3;
  record bar {
    var x : int = 2;
    fun f() {
      writeln(x);
      writeln(y);
    }
  }
  var b : bar;
  writeln(b);
  b.f();
}

foo();

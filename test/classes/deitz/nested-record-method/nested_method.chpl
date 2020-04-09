class C {
  var x : int = 2;
  proc foo() {
    var y : int = x;
    writeln(y);
    proc bar() {
      writeln(x + y);
    }
    bar();
  }
}

var c : unmanaged C = new unmanaged C();

writeln(c);
c.foo();

delete c;

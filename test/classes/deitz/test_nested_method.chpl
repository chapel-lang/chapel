class C {
  var x : int = 2;
  def foo() {
    var y : int = x;
    writeln(y);
    def bar() {
      writeln(x + y);
    }
    bar();
  }
}

var c : C = new C();

writeln(c);
c.foo();

class D {
  var y = 2;
}

class C : D {
  var x = 1;
  def foo() {
    def bar(_x) {
      writeln("default bar");
    }
    def bar(_x : D) {
      writeln("bar of D");
    }
    bar(x);
    bar(this);
    x += 1;
  }
}

var d = new D();
writeln(d);

var c = new C();
c.foo();
writeln(c);

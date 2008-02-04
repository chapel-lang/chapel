class C {
  type t;
  var x = 1;
  var y : t;
  def foo() {
    def bar(_x) {
      writeln("default bar");
    }
    def bar(_x : string) {
      writeln("bar of string");
    }
    bar(x);
    bar("hello world");
  }
}

var c = new C(int);
c.foo();
writeln(c);

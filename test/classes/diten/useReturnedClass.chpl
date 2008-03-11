module M2 {
  class C {
    var a = 1, b = 2;
    def foo() {
      return a+b;
    }
  }
  def main {
    var c = bar();
    writeln(baz(c));
  }
  def bar() {
    return new C();
  }
  def baz(obj) {
    return obj.foo();
  }
}

module M1 {
  var a = 1;
  class C {
    var b = 2;
    def foo() {
      return a+b;
    }
  }
}

module M2 {
  def main {
    var c = bar();
    writeln(c.foo());
  }
  def bar() {
    use M1;
    return new C();
  }
}

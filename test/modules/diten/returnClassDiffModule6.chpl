module M1 {
  var a = 1;
  class C {
    var myvar = 2;
    def foo() {
      return a+myvar;
    }
  }
}

module M2 {
  def main {
    var c = bar();
    writeln(c.myvar);
  }
  def bar() {
    use M1;
    return new C();
  }
}

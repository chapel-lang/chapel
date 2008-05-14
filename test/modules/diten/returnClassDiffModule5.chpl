module M1 {
  var a = 1;
  class C {
    var b = 2;
    def foo() {
      return a+b;
    }
    def writeThis(f: Writer) {
      f.writeln("How does this get found?");
      f.write("{a = ", a, ", b = ", b, "}");
    }
  }
}

module M2 {
  def main {
    var c = bar();
    writeln(baz(c));
    writeln(c);
  }
  def bar() {
    use M1;
    return new C();
  }
  def baz(obj:object) {
    use M1;
    return (obj:C).foo();
  }
}

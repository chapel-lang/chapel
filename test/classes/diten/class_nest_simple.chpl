class myouter {
  var a = 3;

  class inner {
    var b = 2;
    def bar() {
      writeln("inner.b is: ", b);
    }
  }
  def foo() {
    var inside_inner = new inner();
    writeln("outer.a is: ", a);
    inside_inner.bar();
  }
}

def main() {
  var outside: myouter = new myouter();
  outside.foo();
}

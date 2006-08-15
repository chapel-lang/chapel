module mod1 {
  def foo() {
    writeln("in mod1.foo()");
  }
}


module mod2 {
  use mod1; 

  def bar() {
    writeln("in mod2.bar()");
    foo();
  }

  def main() {
    bar();
  }
}

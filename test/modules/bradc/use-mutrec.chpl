module mod1 {
  use mod2;

  def foo() {
    writeln("in mod1.foo()");
    baz();
  }
}


module mod2 {
  use mod1; 

  def bar() {
    writeln("in mod2.bar()");
    foo();
  }

  def baz() {
    writeln("in mod2.baz()");
  }

  def main() {
    bar();
  }
}

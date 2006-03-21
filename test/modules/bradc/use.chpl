module mod1 {
  fun foo() {
    writeln("in mod1.foo()");
  }
}


module mod2 {
  use mod1; 

  fun bar() {
    writeln("in mod2.bar()");
    foo();
  }

  fun main() {
    bar();
  }
}

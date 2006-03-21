module mod1 {
  use mod2;

  fun foo() {
    writeln("in mod1.foo()");
    baz();
  }
}


module mod2 {
  use mod1; 

  fun bar() {
    writeln("in mod2.bar()");
    foo();
  }

  fun baz() {
    writeln("in mod2.baz()");
  }

  fun main() {
    bar();
  }
}

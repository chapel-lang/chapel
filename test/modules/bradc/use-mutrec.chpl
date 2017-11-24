module mod1 {
  use mod2;

  proc foo() {
    writeln("in mod1.foo()");
    baz();
  }
}


module mod2 {
  use mod1; 

  proc bar() {
    writeln("in mod2.bar()");
    foo();
  }

  proc baz() {
    writeln("in mod2.baz()");
  }

  proc main() {
    bar();
  }
}

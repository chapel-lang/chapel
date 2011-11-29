module mod1 {
  proc foo() {
    writeln("in mod1.foo()");
  }
}


module mod2 {
  use mod1; 

  proc bar() {
    writeln("in mod2.bar()");
    foo();
  }

  proc main() {
    bar();
  }
}

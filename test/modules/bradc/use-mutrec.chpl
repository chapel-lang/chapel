module mod1 {
  use mod2;

  function foo() {
    writeln("in mod1.foo()");
    baz();
  }
}


module mod2 {
  use mod1; 

  function bar() {
    writeln("in mod2.bar()");
    foo();
  }

  function baz() {
    writeln("in mod2.baz()");
  }

  function main() {
    bar();
  }
}

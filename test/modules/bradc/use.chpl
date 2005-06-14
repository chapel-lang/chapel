module mod1 {
  function foo() {
    writeln("in mod1.foo()");
  }
}


module mod2 {
  use mod1; 

  function bar() {
    writeln("in mod2.bar()");
    foo();
  }

  function main() {
    bar();
  }
}

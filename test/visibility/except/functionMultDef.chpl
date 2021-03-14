module A {
  proc foo() {
    writeln("I shouldn't be found!");
  }

  proc foo(x: int) {
    writeln("Me neither!");
  }

  proc bar() {
    writeln("Finding me is perfectly valid");
  }
}

module M {
  use A except foo;

  proc main() {
    foo(4);
    foo();
  }
}

module Foo {
  proc bar() {
    writeln("In Foo.bar()");
  }

  proc bar(x: int) {
    writeln("In Foo.bar() with an argument of ", x);
  }

  proc baz() {
    writeln("In Foo.baz()");
  }
}

module M {

  proc main() {
    use Foo only bar as bleep;
    // Verifies the behavior when a function is listed as needing to be renamed
    bar();
    bleep();
    bleep(2);
    Foo.bar();
    Foo.bar(2);
  }

  proc bar() {
    writeln("In M.bar()");
  }
}

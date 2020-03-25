// Checks that we can't call functions using qualified access
module A {
  proc foo() {
    writeln("In A.foo()");
  }

  proc bar(x: int) {
    writeln(x);
  }

  proc other(y: bool): int {
    if (y) {
      return 3;
    } else {
      return 10;
    }
  }

  proc baz(): int {
    return 2;
  }
}
module User {
  import A.{bar, foo, baz};

  proc main() {
    A.foo();
  }
}

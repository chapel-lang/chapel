// Checks that we can call all functions listed in an unqualified manner
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
  import A.{foo, bar, baz};

  proc main() {
    foo();
    var x = baz();
    bar(x);
  }
}

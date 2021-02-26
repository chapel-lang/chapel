module Library {
  record Foo {
    var x: int;
  }

  private inline proc +(a: Foo, b: Foo) {
    writeln("In private +");
    return new Foo(a.x + b.x);
  }

  proc addEm(a: Foo, b: Foo) {
    return a + b;
  }
}

module User {
  use Library only Foo, addEm;

  proc main() {
    var first = new Foo(2);
    var second = new Foo(3);
    //var sum = first + second; // commented out because it fails
    var sumCall = addEm(first, second);
    //writeln(sum);
    writeln(sumCall);
  }
}

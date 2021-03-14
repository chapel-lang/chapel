module Foo {
  var x: int;
}

module Other {
  public use Foo as Bar;
}

module User {
  use Other;
  use Bar as Baz;

  proc main() {
    writeln(Baz.x); // Should work, Foo should be treated like a used Bar?
    writeln(x);
  }
}

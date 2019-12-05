module Foo {
  var x: int;
  var y: bool;
}

module User {
  use Foo as Baz only x;
  use Foo as Bar only y;

  proc main() {
    writeln(x);
    writeln(y);
    writeln(Baz.x);
    writeln(Baz.y);
    writeln(Bar.x);
    writeln(Bar.y);
  }
}

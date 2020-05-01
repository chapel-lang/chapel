module Foo {
  var x: int;
  var y: bool;
}

module User {
  use Foo only x;
  use Foo as Bar only y;

  proc main() {
    writeln(x);
    writeln(y);
    writeln(Foo.x);
    writeln(Foo.y);
    writeln(Bar.x);
    writeln(Bar.y);
  }
}

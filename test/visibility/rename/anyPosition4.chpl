module Foo {
  var bar = 42;

  var baz = 19.6;

  var nerf: int;

  var naming: complex;

  var isDifficult = "blah";
}

module M {
  use Foo only bar, baz, nerf, isDifficult as boop;

  proc main() {
    writeln(boop);

    writeln(bar);
    writeln(baz);
    writeln(nerf);
  }
}

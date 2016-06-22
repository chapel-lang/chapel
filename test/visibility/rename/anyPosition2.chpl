module Foo {
  var bar = 42;

  var baz = 19.6;

  var nerf: int;

  var naming: complex;

  var isDifficult = "blah";
}

module M {
  use Foo only bar, baz as boop, nerf, isDifficult;

  proc main() {
    writeln(boop);

    writeln(bar);
    writeln(nerf);
    writeln(isDifficult);
  }
}

module Foo {
  use Inner;
  var bar = 42;

  var baz = 19.6;
}

module Inner {
  var bar = false;

  proc a() {
    writeln("Taking up space");
  }
}

module M {
  use Foo only bar as boop;

  proc main() {
    writeln(bar); // Should fail, can't see either Foo.bar or Inner.bar
  }
}

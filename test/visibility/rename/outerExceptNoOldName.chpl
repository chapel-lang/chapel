module Foo {
  public use Inner only bar as boop;
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
  use Foo except bar;

  proc main() {
    writeln(boop);
    // Should work, Inner.bar is only visible as boop in this context.
  }
}

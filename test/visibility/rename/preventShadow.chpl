module Foo {
  var bar = false;
}

module M {
  var bar = true;

  proc main() {
    use Foo only bar as baz;

    writeln(baz);
    writeln(bar);
  }
}

module Foo {
  var baz = false;
}

module M {
  var bar = true;

  proc main() {
    use Foo only baz as bar;

    writeln(bar); // Should be false because Foo.baz is more in scope as bar.
  }
}

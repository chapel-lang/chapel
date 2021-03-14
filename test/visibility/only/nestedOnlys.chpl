// Verifies that multiple only layers drop the deeper ones if they don't
// match the topmost.
module A {
  use B only bar;

  var foo = 14.0 - 11.7i;
  type baz = complex;
}

module B {
  type bar = real;

  var gob: bar;
}

module M {
  use A only foo;

  proc main() {
    var a: bar;
    // Since we specified that we just want symbols named foo, bar shouldn't be
    // visible, even though A specifies it in its only list.
    writeln(a);
  }
}

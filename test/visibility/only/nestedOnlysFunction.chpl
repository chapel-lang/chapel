// Verifies that multiple only layers drop the deeper ones if they don't
// match the topmost.
module A {
  use B only bar;

  var foo = 14.0 - 11.7i;
  type baz = complex;
}

module B {
  proc bar() {
    writeln("catch me if you can!");
  }

  var gob: complex;
}

module M {
  use A only foo;

  proc main() {
    bar();
    // Since we specified that we just want symbols named foo, bar shouldn't be
    // visible, even though A specifies it in its only list.
  }
}

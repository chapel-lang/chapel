module m1 {
  deprecated "m1.asdf"
  var asdf = 1;

  deprecated "m1.foo"
  proc foo() {
    writeln("In foo");
  }
}
module m2 {
  use m1; // same issue if 'import m1'
  proc main {
    writeln(m1.asdf);
    writeln(asdf);
    m1.foo();
    foo();
  }
}

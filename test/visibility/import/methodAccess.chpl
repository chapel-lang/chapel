// Covers accessing primary methods, and secondary methods defined in the same
// module.
module A {
  class Foo {
    proc bar() {
      writeln("In Foo's method bar");
    }
  }

  proc Foo.baz() {
    writeln("In Foo's method baz");
  }
}

module B {
  import A;

  proc main() {
    var foo = new A.Foo();
    foo.bar();
    foo.baz();
  }
}

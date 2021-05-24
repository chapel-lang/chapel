class Foo {
  var v1 = 3;

  deprecated "bar is deprecated, use baz instead"
  proc bar() {
    baz();
  }

  proc baz() {
    writeln("In Foo.baz()");
  }
}

proc main() {
  var f = new Foo(5);
  f.bar();
}

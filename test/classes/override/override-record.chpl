record Parent {
  override proc foo() {
    writeln("Parent.foo");
  }
}

proc main() {
  var a = new Parent();
  a.foo();
}

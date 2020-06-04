module A {
  var foo = 11;
}
module User {
  use A as _ only foo as bar;

  proc main() {
    writeln(bar);
  }
}

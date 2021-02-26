module A {
  var x: int;
}
module B {
  var y = 17;
}
module User {
  use A as _, B as _;

  proc main() {
    writeln(x);
    writeln(y);
  }
}

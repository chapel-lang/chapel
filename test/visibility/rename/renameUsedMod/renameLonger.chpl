module Short {
  var x: int;
}
module User {
  use Short as Longer;

  proc main() {
    writeln(Longer.x);
    writeln(x);
  }
}

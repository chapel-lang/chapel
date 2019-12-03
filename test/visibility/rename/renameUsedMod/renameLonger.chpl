module Short {
  var x: int;
}
module User {
  use Short as Longer;

  proc main() {
    writeln(Longer.x); // It shouldn't matter that the new name is longer
    writeln(x);
  }
}

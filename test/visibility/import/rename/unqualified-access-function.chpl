module M {
  proc x() {
    writeln("In M.x()");
  }
}
module User {
  import M.x as y;

  proc main() {
    y();
  }
}

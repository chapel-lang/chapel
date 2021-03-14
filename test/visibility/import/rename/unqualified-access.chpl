module M {
  var x: int;
}
module User {
  import M.x as y;

  proc main() {
    writeln(y);
  }
}

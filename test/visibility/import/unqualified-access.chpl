module M {
  var x: int;
}
module U {
  import M.x;

  proc main() {
    writeln(x);
  }
}

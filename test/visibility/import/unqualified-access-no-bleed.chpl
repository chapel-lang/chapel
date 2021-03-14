module M {
  var x: int;
  var y: int;
}
module U {
  import M.x;

  proc main() {
    writeln(y); // y wasn't imported, shouldn't be able to see it
  }
}

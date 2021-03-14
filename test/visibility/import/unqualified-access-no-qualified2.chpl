module M {
  var x: int;
}
module U {
  import M.x;

  proc main() {
    // Import statements of a single symbol in a module should not enable
    // qualified access of that symbol (or any other)
    writeln(M.x);
  }
}

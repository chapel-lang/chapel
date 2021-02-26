module M {
  var x: int;
  var y: bool;
}
module User {
  import M.{x as foo};

  proc main() {
    writeln(y); // Ensures the renaming doesn't enable other symbols to be found
  }
}

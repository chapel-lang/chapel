module M {
  var a: int;
  var b: bool;
  var c = "blah";
}

module User {
  // ensure we can just list a single symbol in a multi-symbol import
  import M.{c};

  proc main() {
    writeln(c);
  }
}

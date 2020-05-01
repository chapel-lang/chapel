module Name {
  var x: int;
}
module User {
  use Name as Name;

  proc main() {
    // The rename was to the same name.  A bit unnecessary, but shouldn't cause
    // issues
    writeln(Name.x);
    writeln(x);
  }
}

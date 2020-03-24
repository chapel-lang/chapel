module M {
  var x: int;
  var y = false;
  proc z() {
    writeln("In M.z()");
  }
}
module User {
  import M.{x as a, y, z};

  proc main() {
    writeln(a);
    writeln(y);
    z();
  }
}

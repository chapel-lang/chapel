module M {
  var x: int;
  var y = false;
  proc z() {
    writeln("In M.z()");
  }
}
module User {
  import M.{x, y as b, z};

  proc main() {
    writeln(x);
    writeln(b);
    z();
  }
}

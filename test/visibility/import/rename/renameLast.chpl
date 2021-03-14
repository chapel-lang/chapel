module M {
  var x: int;
  var y = false;
  proc z() {
    writeln("In M.z()");
  }
}
module User {
  import M.{x, y, z as c};

  proc main() {
    writeln(x);
    writeln(y);
    c();
  }
}

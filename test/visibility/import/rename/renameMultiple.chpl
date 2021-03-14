module M {
  var x: int;
  var y = false;
  proc z() {
    writeln("In M.z()");
  }
}
module User {
  import M.{x as a, y as b, z as c};

  proc main() {
    writeln(a);
    writeln(b);
    c();
  }
}

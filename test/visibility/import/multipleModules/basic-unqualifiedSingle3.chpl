// Verifies that a large number of modules for unqualified access of a single
// symbol all work
module A {
  var x: int;
}
module B {
  var y: bool;
}
module C {
  proc z () {
    writeln("In C.z()");
  }
}
module D {
  var a = 3;
}
module E {
  var b = "blah blah";
}
module User {
  import A.x, B.y, C.z, D.a, E.b;

  proc main() {
    writeln(x);
    writeln(y);
    z();
    writeln(a);
    writeln(b);
  }
}

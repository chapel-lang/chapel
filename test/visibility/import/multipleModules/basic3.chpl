// Verifies that a large number of modules for qualified access all work
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
  import A, B, C, D, E;

  proc main() {
    writeln(A.x);
    writeln(B.y);
    C.z();
    writeln(D.a);
    writeln(E.b);
  }
}

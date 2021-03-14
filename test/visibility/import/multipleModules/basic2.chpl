// Verifies that 3 modules in the import statement all work for qualified access
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
module User {
  import A, B, C;

  proc main() {
    writeln(A.x);
    writeln(B.y);
    C.z();
  }
}

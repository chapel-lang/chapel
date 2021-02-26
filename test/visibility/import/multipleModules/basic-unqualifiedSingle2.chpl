// Verifies that 3 modules in the import statement all work for unqualified
// access of a single symbol per module
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
  import A.x, B.y, C.z;

  proc main() {
    writeln(x);
    writeln(y);
    z();
  }
}

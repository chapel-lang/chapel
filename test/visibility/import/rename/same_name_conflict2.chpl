module A {
  var x = 10;
}
module C {
  var x = 15;
}
module D {
  import C as A;  // implies A.x is 15
  import A; // imports the renamed A from the previous import
  proc main() {
    writeln(A.x);
  }
}

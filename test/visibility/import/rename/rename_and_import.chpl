module A {
  var x = 10;
}
module C {
  var x = 15;
}
module D {
  import C as A;  // implies A.x is 15
  import A.x; // imports from the renamed A in the previous import
  proc main() {
    writeln(x);
    writeln(A.x);
  }
}

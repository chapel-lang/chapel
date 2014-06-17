class C {
  var a, b, c: int;
  proc ~ // Declare a destructor for this class. Continued below.
  /* Multiple line comment
   */
  // Single line comment

  C() {
    writeln("In destructor");
  }
}

proc main {
  var c = new C(1,2,3);
  delete c;
}

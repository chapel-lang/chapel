class C {
  var a, b, c: int;
  def ~ // Declare a destructor for this class. Continued below.
  /* Multiple line comment
   */
  // Single line comment

  C() {
    writeln("In destructor");
  }
}

def main {
  var c = new C(1,2,3);
  delete c;
}

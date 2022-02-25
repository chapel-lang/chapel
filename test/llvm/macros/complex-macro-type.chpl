extern type ComplexType;

use CTypes;

proc main() {
  var a: ComplexType;
  writeln(c_ptrTo(a):c_void_ptr);
}

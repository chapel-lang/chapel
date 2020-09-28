extern type ComplexType;

use CPtr;

proc main() {
  var a: ComplexType;
  writeln(c_ptrTo(a):c_void_ptr);
}

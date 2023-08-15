class MyClass { var x: int; }
use CTypes;
proc main() {
  var c = new MyClass();
  var ptr = c_ptrTo(c.borrow());

  var c2 = ptr: unmanaged MyClass;
}

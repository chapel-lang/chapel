class MyClass { var x: int; }
use CPtr;
proc main() {
  var c = new MyClass();
  var ptr = c.borrow():c_void_ptr;

  var c1 = ptr: borrowed MyClass;
  var c2 = ptr: unmanaged MyClass;
}

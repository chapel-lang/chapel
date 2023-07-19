class MyClass { var x: int; }
use CTypes;
proc main() {
  var c = new MyClass();
  var ptr = c.borrow():c_ptr(void);

  var c2 = ptr: unmanaged MyClass;
}

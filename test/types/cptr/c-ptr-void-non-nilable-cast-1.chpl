use CTypes;
class MyClass { var x: int; }
proc main() {
  var c = new MyClass();
  var ptr = c.borrow():c_ptr(void);

  var c1 = ptr: borrowed MyClass;
}

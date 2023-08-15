use CTypes;
class MyClass { var x: int; }
proc main() {
  var c = new MyClass();
  var ptr = c_ptrTo(c.borrow());

  var c1 = ptr: borrowed MyClass;
}

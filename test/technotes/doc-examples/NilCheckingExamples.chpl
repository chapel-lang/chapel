//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
class MyClass {
  var x: int;
  proc method() {}
}

proc main() {
  var obj: owned MyClass?; // default initializes 'obj' to store 'nil'
  obj!.method();

  var x = new owned MyClass(1)?;
  var y = owned.release(x); // now 'x' stores 'nil'
  x!.method();
}
/* STOP_EXAMPLE_0 */

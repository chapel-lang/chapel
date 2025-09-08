use CTypes;

type MyHandle = c_ptr(void);
type MyCallback = c_fn_ptr;

record MyObject {
  var value: int;
}
proc myModifyCallback(ref handle: MyHandle): c_int {
  writeln("Chapel: Modifying object...");

  // --------------------------
  // THE ISSUE: Try to dereference the handle back to Chapel record
  ref obj = (handle: c_ptr(MyObject)).deref();
  // --------------------------
  obj.value = 100;

  writeln("Chapel: Object modified, new value = ", obj.value);

  return 0: c_int;
}

proc main() {

  var handle: MyHandle = nil;
  var r = c_ptrTo(myModifyCallback): MyCallback;
  writeln(r:c_ptr(void));
}

/*
 * Minimal reproducer for Chapel/C interop issue:
 * - C typedef of pointer to struct
 * - Chapel record with same name as C struct
 * - Callback that creates and dereferences the pointer
 */

use CTypes;
require "bug.h";

// C types from header
extern type MyHandle = c_ptr(void);
extern type MyCallback = c_fn_ptr;

// Chapel record
record MyObject {
  var value: int;
}

// Chapel callback function - THE PROBLEMATIC PATTERN
proc myCreateCallback(ref handle: MyHandle): c_int {
  writeln("Chapel: Creating object...");

  // Create Chapel record
  ref obj = new MyObject(value = 42);

  // Store as C handle
  handle = c_ptrTo(obj): MyHandle;

  writeln("Chapel: Object created, now trying to access it...");

  // Sanity Check
  ref objBack = (handle: c_ptr(MyObject)).deref();
  writeln("Chapel: Object value = ", objBack.value);

  return 0: c_int;
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

// External C functions
extern proc testCreateCallback(callback: MyCallback): c_int;
extern proc testModifyCallback(callback: MyCallback, handle: MyHandle): c_int;

proc main() {
  writeln("=== Minimal Chapel/C interop reproducer ===");

  // Test 1: Create object
  writeln("\n--- Test 1: Create ---");
  var result1 = testCreateCallback(c_ptrTo(myCreateCallback): MyCallback);
  writeln("Create result: ", result1);

  // Test 2: Create then modify
  writeln("\n--- Test 2: Create then Modify ---");
  var handle: MyHandle = nil;

  // First create an object
  var createResult = myCreateCallback(handle);
  writeln("Create result: ", createResult);

  // Then modify it
  var modifyResult = testModifyCallback(c_ptrTo(myModifyCallback): MyCallback, handle);
  writeln("Modify result: ", modifyResult);
}

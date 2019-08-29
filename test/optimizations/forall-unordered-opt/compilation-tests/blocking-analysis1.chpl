config const N = 100;
var globalSync: sync int;
var globalSingle: single int;
var globalAtomicBool: atomic bool;
var globalAtomicInt: atomic int;
var globalInt: int;

// Explicit main included to reduce test maintenance
proc main() { }

// Varying the atomic method called
proc blocking_loop_int_read() {
  while globalAtomicInt.read() == 0 { }
}
blocking_loop_int_read();

proc blocking_loop_int_compare_and_swap() {
  while globalAtomicInt.compareAndSwap(0, 1) == false { }
}
blocking_loop_int_compare_and_swap();

proc blocking_loop_int_waitfor() {
  globalAtomicInt.waitFor(false);
}
blocking_loop_int_waitfor();

proc blocking_loop_bool_read() {
  while globalAtomicBool.read() == false { }
}
blocking_loop_bool_read();

proc blocking_loop_bool_compare_and_swap() {
  while globalAtomicBool.compareAndSwap(false, true) == false { }
}
blocking_loop_bool_compare_and_swap();

proc blocking_loop_bool_test_and_set() {
  while globalAtomicBool.testAndSet() == true { }
}
blocking_loop_bool_test_and_set();

proc blocking_loop_bool_waitfor() {
  globalAtomicBool.waitFor(false);
}
blocking_loop_bool_waitfor();

// sync and single
proc blocking_sync_write() {
  globalSync = 1;
}
blocking_sync_write();

proc blocking_sync_read() {
  var x = globalSync;
  return x;
}
blocking_sync_read();

proc blocking_single_write() {
  globalSingle = 1;
}
blocking_single_write();

proc blocking_single_read() {
  var x = globalSingle;
  return x;
}
blocking_single_read();

// tests of conditionals
proc blocking_in_if() {
  if N == 1 {
    while globalAtomicInt.read() == 0 { }
  }
}
blocking_in_if();

proc blocking_in_else() {
  if N == 1 {
  } else {
    while globalAtomicInt.read() == 0 { }
  }
}
blocking_in_else();

// nested loop
proc blocking_nested_loop() {
  while N == 1 {
    while globalAtomicInt.read() == 0 { }
  }
}
blocking_nested_loop();

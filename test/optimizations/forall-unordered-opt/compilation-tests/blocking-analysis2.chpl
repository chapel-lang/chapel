config const N = 100;
var globalAtomicInt: atomic int;

// Explicit main included to reduce test maintenance
proc main() { }

proc one_atomic_op() {
  globalAtomicInt.add(1);
}
one_atomic_op();

proc blocking_loop_int_read() {
  while globalAtomicInt.read() == 0 { }
}

// varying the loop type containing an atomic method
proc blocking_atomic_while() {
  while globalAtomicInt.read() == 0 { }
}
blocking_atomic_while();

proc blocking_atomic_do_while() {
  do { } while globalAtomicInt.read() == 0;
}
blocking_atomic_do_while();

proc blocking_atomic_for_range() {
  for i in 1..N {
    globalAtomicInt.read();
  }
}
blocking_atomic_for_range();

iter myiter() {
  for i in 1..N {
    yield i;
  }
}

proc blocking_atomic_for_iterator() {
  for x in myiter() {
    globalAtomicInt.read();
  }
}
blocking_atomic_for_iterator();

proc blocking_atomic_in_forall() {
  forall i in 1..N {
    globalAtomicInt.read();
  }
}
blocking_atomic_in_forall();

proc blocking_recursive(arg:int):int {
  if arg <=0 then
    return 0;

  globalAtomicInt.read();

  return 1+blocking_recursive(arg-1);
}
blocking_recursive(N);

// varying ways of calling a blocking method
proc calls_blocking() {
  blocking_loop_int_read();
}
calls_blocking();

class Parent {
  proc mayblock() { }
}
class Child : Parent {
  proc mayblock() {
  }
}

proc blocking_virtual_method(arg: Parent) {
  calls_blocking();
}

proc calls_an_extern() {
  extern proc myexternfn();
  myexternfn();
}
calls_an_extern();

proc calls_one_op_in_loop() {
  while true {
    one_atomic_op();
  }
}
calls_one_op_in_loop();

proc calls_one_op_recursive(arg:int):int {
  if arg <=0 then
    return 0;

  one_atomic_op();

  return 1+calls_one_op_recursive(arg-1);
}
calls_one_op_recursive(N);

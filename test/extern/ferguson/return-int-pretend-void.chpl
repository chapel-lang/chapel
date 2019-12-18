require "return-int-pretend-void.h";

use SysCTypes;

extern proc foo():void;  // actually, foo() returns c_int
extern proc bar():c_int; // actually, bar() returns void

// The intent of this test is to lock in behavior that
// tolerates some inconsistency in the return type,
// provided that the returned value is never used.
proc test() {
  foo();
  bar();
}

test();

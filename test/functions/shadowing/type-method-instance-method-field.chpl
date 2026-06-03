// Test for issue #28599 - edge case
// Instance parenless method should still conflict with field
// even when there's also a type method with the same name
record R {
  var x: int = 17;
}
proc type R.x param do return 1;  // type method - OK
proc R.x do return 99;             // instance method - should error

var r = new R();
writeln(r.x);

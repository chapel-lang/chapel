// Test for issue #28599
// Type methods should not conflict with fields of the same name
record R {
  var x: int = 17;
}
proc type R.x param do return 1;

var r = new R();
writeln(r.x); // should print 17 (field access)
writeln(R.x); // should print 1 (type method call)

// Test for issue #28599 - param field case
// Type methods should conflict with param fields since
// param fields generate type-method accessors
record R {
  param x: int = 17;
}
proc type R.x param do return 1;

var r = new R();
writeln(r.x);
writeln(R.x);

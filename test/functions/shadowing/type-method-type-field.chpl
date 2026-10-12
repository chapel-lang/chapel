// Test for issue #28599 - type field case
// Type methods should conflict with type fields since
// type fields generate type-method accessors
record R {
  type x = int;
}
proc type R.x type do return real;

var r = new R();
writeln(r.x:string);
writeln(R.x:string);

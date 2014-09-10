// Test a begin block with a reference to a record variable

record R {
  var x: int;
}

var r = new R();
r.x = 7;
writeln( r);

begin with (ref r) {
  r.x = 14;
  writeln( r);
}

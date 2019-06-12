use UnitTest;
var sep ="=="*40;
try {
  UnitTest.assertFalse(true);
}
catch e {
  writeln(e);
  writeln(sep);
}
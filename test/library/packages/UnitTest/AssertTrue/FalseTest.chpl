use UnitTest;
var sep = "=="*40;
try {
  UnitTest.assertTrue(false);
}
catch e {
  writeln(e);
  writeln(sep);
}
try {
  UnitTest.assertTrue(1==2);
}
catch e {
  writeln(e);
  writeln(sep);
}
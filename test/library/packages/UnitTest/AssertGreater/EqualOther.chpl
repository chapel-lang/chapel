use UnitTest;
var sep = "=="*40;
var x1 = 1.0;
var y1 = 1;
try {
  UnitTest.assertGreaterThan(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x2 = "equal string";
try {
  UnitTest.assertGreaterThan(x2,x2);
}
catch e {
  writeln(e);
  writeln(sep);
}
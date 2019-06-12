use UnitTest;
var sep = "=="*40; 
var x1 = 5;
var y1 = 7i;
try {
  UnitTest.assertGreaterThan(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}
var x2 = 1 + 5i;
var y2 = 5i;
try {
  UnitTest.assertGreaterThan(x2,y2);
}
catch e {
  writeln(e);
  writeln(sep);
}
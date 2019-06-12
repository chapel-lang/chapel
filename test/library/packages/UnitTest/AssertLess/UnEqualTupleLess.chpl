use UnitTest;
var x = (1,2,5);
var y = (1,2,3);
var sep ="=="*40;
try {
  UnitTest.assertLessThan(x,y);
}
catch e {
  writeln(e);
  writeln(sep);
}
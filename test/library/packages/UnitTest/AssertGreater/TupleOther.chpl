use UnitTest;
var sep = "=="*40;
var x1 = (1,2,3);
var y1 = (1,2,5);
try{
  UnitTest.assertGreaterThan(x1,y1);
}
catch e {
  writeln(e);
  writeln(sep);
}

var x2 = [1,2,3];
var y2 = (1,2);
try{
  UnitTest.assertGreaterThan(x2,y2);
}
catch e {
  writeln(e);
  writeln(sep);
}
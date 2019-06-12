use UnitTest;
var sep = "=="*40;
var x1D = [1,2,5];
var y1D = [1,2,5];
var xND : [1..5,1..7] real;
var yND : [{1..5,1..7}] real;
try {
  UnitTest.assertNotEqual(x1D,y1D);
}
catch e {
  writeln(e);
  writeln(sep);
}
try{
  UnitTest.assertNotEqual(xND,yND);
}
catch e {
  writeln(e);
  writeln(sep);
}
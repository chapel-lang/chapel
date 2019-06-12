use UnitTest;
var sep = "=="*40;
var x = "unequalstring";
var y = "unequalString";
try {
  UnitTest.assertLessThan(x,y);
}
catch e {
  writeln(e);
  writeln(sep);
}
x = "Just a string";
y = "Just a string with space";
try {
  UnitTest.assertLessThan(x,y);
}
catch e {
  writeln(e);
  writeln(sep);
}
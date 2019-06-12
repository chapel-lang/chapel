use UnitTest;
var sep = "=="*40;
var x = "Just a string";
var y = "Just a string with space";
try {
  UnitTest.assertGreaterThan(x,y);
}
catch e {
  writeln(e);
  writeln(sep);
}
x = "unequalString";
y = "unequalstring";
try {
  UnitTest.assertGreaterThan(x,y);
}
catch e {
  writeln(e);
  writeln(sep);
}
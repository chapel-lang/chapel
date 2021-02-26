config const option = true;
var A:[1..10] int;

proc makeDomain() {
  const D = {1..2};
  writeln("in makeDomain, D.definedConst=", D._value.definedConst);
  return D;
}

proc test1() {
  writeln("test1");
  var D = makeDomain();
  writeln("in test, D.definedConst=", D._value.definedConst);
  writeln("D. is ", D);
  D = {2..2};
  writeln("D is now ", D);
}
test1();

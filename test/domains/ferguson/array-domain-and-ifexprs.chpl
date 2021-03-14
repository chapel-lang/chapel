config const option = true;
var A:[1..10] int;

proc makeDomain() {
  return {1..2};
}

proc test1() {
  writeln("test1");
  var D = if option then A.domain else makeDomain();
  writeln("D is ", D);
  D = {2..2};
  writeln("D is now ", D);
  writeln("A.domain is now ", A.domain);
}
test1();

proc test2() {
  writeln("test2");
  var D = if option then makeDomain() else A.domain;
  writeln("D is ", D);
  D = {2..2};
  writeln("D is now ", D);
  writeln("A.domain is now ", A.domain);
}
test2();

proc retTestA() {
  return if option then A.domain else makeDomain();
}
proc retTestB() {
  return if option then makeDomain() else A.domain;
}

proc test3() {
  writeln("test3");
  var D = retTestA();
  writeln("D is ", D);
  D = {2..2};
  writeln("D is now ", D);
  writeln("A.domain is now ", A.domain);
}
test3();

proc test4() {
  writeln("test4");
  var D = retTestB();
  writeln("D is ", D);
  D = {2..2};
  writeln("D is now ", D);
  writeln("A.domain is now ", A.domain);
}
test4();

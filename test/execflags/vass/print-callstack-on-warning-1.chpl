// generate some call stacks as contexts for warning messages

test1("test1");
test2("test2");
test3("test3");

proc test1(x) {
  test2(x);
}

proc test2(y) {
  test3(y);
}

proc test3(z) {
  compilerWarning("warning from test3, depth=0", 0);
  compilerWarning("warning from test3, default depth", 0);
  writeln("test3(z=", z, ")");
}

ptest1(1);
ptest2(10);
ptest3(100);

proc ptest1(param x) {
  ptest2(x+2);
}

proc ptest2(param y) {
  ptest3(y+3);
}

proc ptest3(param z) {
  compilerWarning("warning from ptest3, depth=0, z=", z:string, 0);
  compilerWarning("warning from ptest3, default depth, z=", z:string, 0);
  writeln("ptest3(z=", z, ")");
}

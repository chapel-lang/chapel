// generate some call stacks as contexts for warning messages

test1("test1");
test2("test2");
test3("test3");

def test1(x) {
  test2(x);
}

def test2(y) {
  test3(y);
}

def test3(z) {
  compilerWarning("warning from test3, depth=0", 0);
  compilerWarning("warning from test3, default depth", 0);
  writeln("test3(z=", z, ")");
}

ptest1(1);
ptest2(10);
ptest3(100);

def ptest1(param x) {
  ptest2(x+2);
}

def ptest2(param y) {
  ptest3(y+3);
}

def ptest3(param z) {
  compilerWarning("warning from ptest3, depth=0, z=", z:string, 0);
  compilerWarning("warning from ptest3, default depth, z=", z:string, 0);
  writeln("ptest3(z=", z, ")");
}

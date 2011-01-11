def z(param i) {
  param depth = 2;
  compilerWarning("param = ", i:string, "  depth = ", depth:string, depth);
  writeln("z("+i+")");
}

def y1(param i) {
  z(i);
}

def x1(param i) {
  y1(i);
}

def y2(param i) {
  z(i);
}

def x2(param i) {
  y2(i);
}

x1(1);
x2(1);

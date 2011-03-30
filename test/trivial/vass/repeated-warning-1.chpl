proc z(param i) {
  param depth = 2;
  compilerWarning("param = ", i:string, "  depth = ", depth:string, depth);
  writeln("z("+i+")");
}

proc y1(param i) {
  z(i);
}

proc x1(param i) {
  y1(i);
}

proc y2(param i) {
  z(i);
}

proc x2(param i) {
  y2(i);
}

x1(1);
x2(1);

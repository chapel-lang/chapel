def f(a, (b,(c, (d,e))), (f,g)) where a.type == int {
  writeln(a, " ", b, " ", c, " ", d, " ", e, " ", f, " ", g);
  return (a+b+c+d+e+f+g);
}

def main() {
  var p = f(1, (2,(3,(4,5))), (6,7)); 

  writeln(p);
}

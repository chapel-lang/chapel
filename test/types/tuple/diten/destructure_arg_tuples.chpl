proc f(a, (b,(c, (d,e))), (f,g)) where a.type == int {
  writeln(a, " ", b, " ", c, " ", d, " ", e, " ", f, " ", g);
  return (a+b+c+d+e+f+g);
}

proc main() {
  var p = f(1, (2,(3,(4,5))), (6,7)); 

  writeln(p);
}

const x = 10;

var y = 0;

proc f() ref {
  return y;
}
proc f() {
  return x;
}


f() = 11;

writeln(x, " " , y);

proc getstring(n:int) {
  return "a"*n;
}
proc gettup() {
  return (2, getstring(2), (4, getstring(4)));
}


proc run() {
  var (a, b, c) = gettup();

  writeln("a=", a, " b=", b, " c=", c);
}

run();

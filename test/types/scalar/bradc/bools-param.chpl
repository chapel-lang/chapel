param b  : bool     = true;

proc testAssignToAll(param x) {
  param locb: bool = x;

  writeln("locb = ", locb);
}
  
testAssignToAll(b);

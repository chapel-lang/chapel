param b  : bool     = true;
param b8 : bool(8)  = true;
param b16: bool(16) = true;
param b32: bool(32) = true;
param b64: bool(64) = true;

proc testAssignToAll(param x) {
  param locb: bool = x;
  param locb8: bool = x;
  param locb16: bool = x;
  param locb32: bool = x;
  param locb64: bool = x;

  writeln("locb = ", locb);
  writeln("locb8 = ", locb8);
  writeln("locb16 = ", locb16);
  writeln("locb32 = ", locb32);
  writeln("locb64 = ", locb64);
  writeln();
}
  
testAssignToAll(b);
testAssignToAll(b8);
testAssignToAll(b16);
testAssignToAll(b32);
testAssignToAll(b64);

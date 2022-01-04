operator >=(a: uint(64), param b: int(64)) where b >= 0 {
  // the >= in the where clause on this function should resolve to
  // >=(param int(64), param int(64)) param, not to the >= defined here.
  return a >= (b: uint(64));
}
var a: uint(64) = 2;
param b:int(64) = 1;

writeln(a >= b);

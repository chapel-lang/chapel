proc +(a: uint(64), b: int(64)) {
  writeln("Found my plus");
  return a + b:uint(64);
}

proc ==(a: int, b: uint) {
  if (a > 0) {
    var auint = a: uint;
    return auint == b;
  } else {
    return false;
  }
} 
var m: uint(64) = 1;
var n: int = 2;
var a: int = 1;
writeln(m + n);
writeln(a == m);

def +(a: uint(64), b: int(64)) {
  writeln("Found my plus");
  return a + b:uint(64);
}

def ==(a: int, b: int) {
  writeln("Found my ==, but it's going to give != instead");
  return a != b;
} 
var m: uint(64) = 1;
var n: int = 2;
var a: int = 1;
writeln(m + n);
writeln(a == n);

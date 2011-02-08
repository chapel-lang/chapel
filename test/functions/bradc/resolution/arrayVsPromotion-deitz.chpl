proc g(x: int, y: int, z: int) {
  writeln("In g(:int, :int, :int)");
}

proc g(x: [] int, y, z: int) {
  writeln("In g(:[] int, :int, :int)");
}

var A: [1..3] int;
var B: [1..3] int;

g(A, 2, B);

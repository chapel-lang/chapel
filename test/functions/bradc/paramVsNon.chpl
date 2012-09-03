proc foo(param x: int(32)) {
  writeln("In param 32-bit version");
}

proc foo(param x: int(64)) {
  writeln("In param version");
}

proc foo(x: int(32)) {
  writeln("In non-param 32-bit version");
}

proc foo(x: int(64)) {
  writeln("In non-param version");
}


param NEGNINE = -9;
var negnine = -9;

param NEGNINE32: int(32) = -9;
var negnine32: int(32) = -9;

foo(-9);
foo(NEGNINE);
foo(negnine);

foo(-9:int(32));
foo(NEGNINE32);
foo(negnine32);

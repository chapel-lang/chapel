proc plus(x: int, y: int) {
  return x + y;
}

proc plus(x: uint, y: uint) {
  return x + y;
}

//pragma "last resort"
proc plus(x: int, y: uint) {
  compilerError("No default support for plus(int, uint)");
}


proc foo(x: int) {
  writeln("In int foo!");
}

proc foo(x: real) {
  writeln("In real foo!");
}

//pragma "last resort"
proc foo(x) {
  compilerError("foo() can't be called with type: ", x.type:string);
}

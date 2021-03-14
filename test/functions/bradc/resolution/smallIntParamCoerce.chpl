proc foo(x:int(16)) {
  writeln("In foo()");
}

param i32: int(32) = 1;
param i64: int(64) = 1;

foo(i32);
foo(i64);

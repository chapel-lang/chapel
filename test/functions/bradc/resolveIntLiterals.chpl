proc foo(x: int(8)) {
  writeln("int8 version of foo called");
}


proc foo(x: int) {
  writeln("int version of foo called");
}


foo(8);
foo(8:int(8));
foo(8:int(16));
foo(8:int);

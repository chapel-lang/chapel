proc foo(type t:int) {
  writeln("int version");
}
proc foo(type t:int(8)) {
  writeln("int(8) version");
}

proc bar(x:int(8)) {
}

foo(int(8));
bar(1);

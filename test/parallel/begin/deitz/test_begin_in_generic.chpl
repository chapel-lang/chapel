proc bar(type t) {
  writeln("bar");
}

proc foo(type t) {
  begin bar(t);
}

foo(int);

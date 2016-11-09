var r:int;
proc foo(const ref t:int = 0) {
  r = t;
}
foo();
writeln(r);

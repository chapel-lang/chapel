@edition(last=2.0)
proc foo(x: int) {
  writeln("in old edition foo with arg x=", x);
}

proc main() {
  foo(12);
}

class C {
  var x: int;
}

proc main() {
  var c: C = new C(1);
  atomic c.x += 1;
  writeln("c.x = ", c.x);
  delete c;
}

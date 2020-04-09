class C {}
var c = new borrowed C();
proc foo(ref obj: object) {
  writeln("called foo(obj)");
}
foo(c);

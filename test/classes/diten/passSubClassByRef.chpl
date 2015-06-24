class C {}
var c = new C();
proc foo(ref obj: object) {
  writeln("called foo(obj)");
}
foo(c);

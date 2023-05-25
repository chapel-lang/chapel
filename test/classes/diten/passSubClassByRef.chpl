class C {}
var c = (new owned C()).borrow();
proc foo(ref obj: object) {
  writeln("called foo(obj)");
}
foo(c);

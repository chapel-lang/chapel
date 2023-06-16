class C {}
var c = (new owned C()).borrow();
proc foo(ref obj: RootClass) {
  writeln("called foo(obj)");
}
foo(c);

class C {}
var obj = new C(); var c = obj.borrow();
proc foo(ref obj: RootClass) {
  writeln("called foo(obj)");
}
foo(c);

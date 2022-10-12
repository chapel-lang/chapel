class C { var x = 0; }

proc foo(a: int, b: real, c: borrowed C) throws { writeln('foo'); }
proc bar(v1: int, v2: real, v3: borrowed C) throws { writeln('bar'); }

// All of these calls should be OK even though the formals have different
// names. This is because in legacy mode all formals are "anonymous".
// This mimicks a scenario that occurred in Arkouda very recently.
proc test() {
  const f1 = foo;
  type T1 = f1.type;
  proc useTheFn(cb: T1) { try! cb(0, 1.0, new owned C(2)); }

  useTheFn(foo);
  useTheFn(bar);
}
test();


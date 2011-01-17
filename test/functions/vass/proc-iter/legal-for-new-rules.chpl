// Code that is legal according to the new rules.
// By definition, it is also legal during transition.

proc p11a() {
  writeln("in p11a");
}
p11a();


proc p11b() {
  writeln("in p11b");
  return;
}
p11b();


proc p11c() {
  return 11;
}
writeln("p11c: ", p11c());


def i11a() {
  yield "11a";
}
ti(i11a());


iter i11b() {
  yield "11b";
  return;
}
ti(i11b());


// ensure it is treated as an iterator, not a procedure
iter i11g() {
  yield "11g-1.";
  yield "11g-2.";
}
writeln(i11g());


// just a declaration
_extern proc e1();


// helper: run an iterator
proc ti(it) {
  for i in it do write(i); writeln();
}

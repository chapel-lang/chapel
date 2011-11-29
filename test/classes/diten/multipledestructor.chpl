class C {
  proc ~C() { writeln("In ~C #1"); }
}
proc C.~C() { writeln("In ~C #2"); }

proc main {
  var c = new C();
  delete c;
}

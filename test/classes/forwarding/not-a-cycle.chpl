
enum testCase {
  OWNED, SHARED, UNMANAGED
}
config param test = testCase.OWNED;
proc typename type {
  if test == testCase.OWNED then return owned B;
  else if test == testCase.SHARED then return shared B;
  else if test == testCase.UNMANAGED then return unmanaged B;
  else compilerError("Unknown test case");
}

class B {
  iter fun():int { }
  iter fun(param tag: iterKind):int where tag == iterKind.standalone { }
}

record A {
  forwarding var impl: typename;

  proc init() {
    this.impl = new typename();
  }

  proc deinit() {
    if test == testCase.UNMANAGED then
      delete this.impl;
  }
}

proc main() {
  var aa = new A();
  {
    forall e in aa.fun() { e; }
    forall e in aa.fun() { e; }
  }
}

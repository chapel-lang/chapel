/* This class should not get the forwarded methods from B */
class A {
  var f;

  forwarding f;
}

class B {
  var b: int;

  proc someProc() { }

  proc anotherProc() { }
}

/* This class should get the forwarded methods from B */
class A {
  var f: B;
  
  forwarding f;
}

class B {
  var b: int;

  proc someProc() { }

  proc anotherProc() { }
}

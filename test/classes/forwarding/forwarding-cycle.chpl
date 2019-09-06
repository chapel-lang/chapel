// Test that forwarding cycle results in an error

class A {
  forwarding var data: unmanaged B?;
  proc fun(i:int) {}
}
class B {
  forwarding var data: unmanaged A?;
}

proc main() {
  var a = new unmanaged A();
  a.fun(1.1);
  delete a;
}

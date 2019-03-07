// Test that forwarding cycle results in an error

class A {
  forwarding var data: unmanaged B;
}
class B {
  forwarding var data: unmanaged A;
}

proc main() {
  var a = new unmanaged A();
  delete a;
}

// Test that forwarding cycle results in an error

class A {
  var data: unmanaged B?;
  forwarding data!;
  proc fun(i:int) {}
}
class B {
  var data: unmanaged A?;
  forwarding data!;
}

proc main() {
  var a = new unmanaged A();
  a.fun(1.1);
  delete a;
}

// Test that forwarding to self is detected as an error
class A {
  forwarding var data: unmanaged A?;
  proc fun(i:int) {}
}
proc main() {
  var a = new unmanaged A();
  a.fun(1.1);
  delete a;
}

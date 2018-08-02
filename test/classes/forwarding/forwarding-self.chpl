// Test that forwarding to self is detected as an error
class A {
  forwarding var data: unmanaged A;
}
proc main() {
  var a = new unmanaged A();
  delete a;
}

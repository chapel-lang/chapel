class MyClass {
  proc method() {
    writeln("in method");
  }
}
class Forwarder {
  forwarding var x;
}

proc test() {
  var ff = new unmanaged Forwarder(new unmanaged MyClass());

  ff.method();
  delete ff.x;
  delete ff;
}
test();

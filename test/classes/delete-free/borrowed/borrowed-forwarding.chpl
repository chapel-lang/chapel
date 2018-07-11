class MyClass {
  proc method() {
    writeln("in method");
  }
}
class Forwarder {
  forwarding var x;
}

proc test() {
  var x = new unmanaged MyClass();
  var b = _to_borrowed(x);
  var y = new unmanaged Forwarder(b);
  var bb = _to_borrowed(y);

  bb.method();

  delete x;
  delete y;
}
test();

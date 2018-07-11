
class C {
  var field:int;
}

class D {
  var field:real;
}


record WrapperOne {
  var instance; // e.g. some class
  proc f( a:instance.type, b=instance) {
    writeln("in f ", this, " a=", a, " b=", b);
  }
}

record WrapperTwo {
  var instance; // WrapperOne
  forwarding instance only f;
}


{
  var a = new WrapperTwo(new WrapperOne(new unmanaged C(1)));
  var aa = new unmanaged C(2);

  a.f(aa);

  delete aa;
  // Can't use deinit, otherwise there will be a double-free
  delete a.instance.instance;
}

{
  var b = new WrapperTwo(new WrapperOne(new unmanaged D(1.0)));
  var bb = new unmanaged D(2.0);

  b.f(bb);

  delete bb;
  delete b.instance.instance;
}

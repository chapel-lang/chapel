
class C {
  var field:int;
}

class D {
  var field:real;
}


record WrapperOne {
  var instance; // e.g. some class
  proc f( a:this.instance.type, b=this.instance) {
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

  // Can't use deinit, otherwise there will be a double-free
  delete aa, a.instance.instance;
}

{
  var b = new WrapperTwo(new WrapperOne(new unmanaged D(1.0)));
  var bb = new unmanaged D(2.0);

  b.f(bb);

  delete bb, b.instance.instance;
}


class C {
  var field:int;
}

class D {
  var field:real;
}


record WrapperOne {
  var instance; // e.g. some class
  proc f( a:this.type ) {
    writeln("in f ", this, " a=", a);
  }
}

record WrapperTwo {
  var instance; // WrapperOne
  forwarding instance;
}


{
  var a = new WrapperTwo(new WrapperOne(new unmanaged C(1)));
  var aa = new WrapperOne(new unmanaged C(2));

  a.f(aa);

  // Can't use deinit, otherwise there will be a double-free
  delete aa.instance, a.instance.instance;
}

{
  var b = new WrapperTwo(new WrapperOne(new unmanaged D(1.0)));
  var bb = new WrapperOne(new unmanaged D(2.0));

  b.f(bb);

  delete bb.instance, b.instance.instance;
}

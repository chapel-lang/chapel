class C {
  proc foo() {
    writeln("In C.foo()");
  }
}

class SubC: C {
  override proc foo() {
    writeln("In SubC.foo()");
  }
}


class D {
  proc foo() {
    return new unmanaged C();
  }

  proc bar() {
    return new owned C();
  }
}

class SubD: D {
  override proc foo() {
    return new unmanaged SubC();
  }

  override proc bar() {
    return new owned SubC();
  }
}

{
  var myD = new D();
  var res1 = myD.foo();
  var res2 = myD.bar();
  res1.foo();
  res2.foo();
}

{
  var myD: D = new D();
  var res1 = myD.foo();
  var res2 = myD.bar();
  res1.foo();
  res2.foo();
}

{
  var myD = new SubD();
  var res1 = myD.foo();
  var res2 = myD.bar();
  res1.foo();
  res2.foo();
}

{
  var myD: D = new SubD();
  var res1 = myD.foo();
  var res2 = myD.bar();
  res1.foo();
  res2.foo();
}

{
  var myD: SubD = new SubD();
  var res1 = myD.foo();
  var res2 = myD.bar();
  res1.foo();
  res2.foo();
}

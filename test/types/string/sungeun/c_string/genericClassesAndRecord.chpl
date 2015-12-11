use checkType;

{
  class C {
    var x;
  }

  var c0 = new C("blah");
  checkType(c0.x.type);

  param blah1 = "blah";
  var c1 = new C(blah1);
  checkType(c1.x.type);

  const blah2: c_string = "blah";
  var c2 = new C(blah2);
  checkType(c_string, c2.x.type);

  const blah3 = "blah";
  var c3 = new C(blah3);
  checkType(c3.x.type);
}

{
  class C {
    param x;
  }

  var c0 = new C("blah");
  checkType(string, c0.x.type);

  param blah1 = "blah";
  var c1 = new C(blah1);
  checkType(string, c1.x.type);
}

{
  record R {
    var x;
  }

  var r0 = new R("blah");
  checkType(r0.x.type);

  param blah1 = "blah";
  var r1 = new R(blah1);
  checkType(r1.x.type);

  const blah2: c_string = "blah";
  var r2 = new R(blah2);
  checkType(c_string, r2.x.type);

  const blah3 = "blah";
  var r3 = new R(blah3);
  checkType(r3.x.type);
}

{
  record R {
    param x;
  }

  var r0 = new R("blah");
  checkType(string, r0.x.type);

  param blah1 = "blah";
  var r1 = new R(blah1);
  checkType(string, r1.x.type);
}

use checkType;

{
  class C {
    var x;
  }

  var ownC0 = new owned C("blah");
  var c0 = ownC0.borrow();
  checkType(c0.x.type);

  param blah1 = "blah";
  var ownC1 = new owned C(blah1);
  var c1 = ownC1.borrow();
  checkType(c1.x.type);

  const blah2: c_string = "blah";
  var ownC2 = new owned C(blah2);
  var c2 = ownC2.borrow();
  checkType(c_string, c2.x.type);

  const blah3 = "blah";
  var ownC3 = new owned C(blah3);
  var c3 = ownC3.borrow();
  checkType(c3.x.type);
}

{
  class C {
    param x;
  }

  var ownC0 = new owned C("blah");
  var c0 = ownC0.borrow();
  checkType(string, c0.x.type);

  param blah1 = "blah";
  var ownC1 = new owned C(blah1);
  var c1 = ownC1.borrow();
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

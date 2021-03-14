use myrecord;

proc return_ref(ref r) ref
{
  return r;
}

proc myfunction() {

  var r:R;
  r.setup(x=1);
  r.verify();

  var r2:R;
  r2.setup(x=2);
  r2.verify();

  // Test use of return_ref as an lvalue.
  return_ref(r) = r2;
  r.verify();
}

myfunction();

verify();


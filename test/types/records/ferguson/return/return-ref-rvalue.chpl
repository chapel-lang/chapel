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
  r2 = return_ref(r);
  r2.verify();
}

myfunction();

verify();


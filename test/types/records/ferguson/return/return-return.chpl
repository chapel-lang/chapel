use myrecord;

proc return1()
{
  var r:R;
  r.setup(x=1);
  r.verify();
  return r;
}

proc return2()
{
  return return1();
}

proc myfunction() {

  var r = return2();
  r.verify();
}

myfunction();

verify();


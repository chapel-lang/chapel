record R {
  var x;

  proc const f()
  {
    x = 12;
  }
}

{
  var r = new R(1);

  r.f();
}

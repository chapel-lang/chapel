record R {
  var x;

  proc const ref f()
  {
    x = 24;
  }
}

{
  var r = new R(1);

  r.f();
}

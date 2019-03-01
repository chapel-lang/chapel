record history_real {
  param size : int;
  var h : size*real;
  var f : real;

  proc add(r : real) {
    for i in 2..size by -1 do
      h(i) = h(i-1);
    h(1) = f;
    f = r;
  }

  proc init=(type ThisType, r : real) {
    this.size = ThisType.size;
    this.complete();
    add(r);
  }
}

proc =(ref x : history_real, y : real) {
  x.add(y);
}

proc >(x : history_real, y : real) {
  return x.f > y;
}

proc +(x : history_real, y : real) {
  return x.f + y;
}

proc history_real.writeThis(ff) {
  ff.write(f, " (");
  for i in 1..size do
    ff.write(h(i), if i < size then ", " else ")");
}

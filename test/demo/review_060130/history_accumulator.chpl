record history_real {
  param size : int;
  var h : size*real;
  var f : real;

  proc add(r : real) {
    for i in 1..size-1 by -1 do
      h(i) = h(i-1);
    h(0) = f;
    f = r;
  }

  proc init=(r : real) {
    this.size = this.type.size;
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
  for i in 0..#size do
    ff.write(h(i), if i < size-1 then ", " else ")");
}

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

  proc init=(other : history_real) {
    this.size = other.size;
    this.h = other.h;
    this.f = other.f;
  }
}

proc =(ref x : history_real, y : real) {
  x.add(y);
}

proc =(ref x : history_real, y : x.type) {
  x.h = y.h;
  x.f = y.f;
}

proc >(x : history_real, y : real) {
  return x.f > y;
}

proc +(x : history_real, y : real) {
  return x.f + y;
}

proc history_real.writeThis(ff) throws {
  ff.write(f, " (");
  for i in 0..#size do
    ff.write(h(i), if i < size-1 then ", " else ")");
}

record history_real {
  param size : int;
  var h : size*real;
  var f : real;
}

proc =(ref x : history_real, y : real) {
  for i in 2..x.size by -1 do
    x.h(i) = x.h(i-1);
  x.h(1) = x.f;
  x.f = y;
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

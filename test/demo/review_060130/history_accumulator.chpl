record history_real {
  param size : int;
  var h : size*real;
  var f : real;
}

def =(x : history_real, y : real) {
  for i in 2..x.size by -1 do
    x.h(i) = x.h(i-1);
  x.h(1) = x.f;
  x.f = y;
  return x;
}

def >(x : history_real, y : real) {
  return x.f > y;
}

def +(x : history_real, y : real) {
  return x.f + y;
}

def history_real.write(ff : file) {
  ff.write(f, " (");
  for i in 1..size do
    ff.write(h(i), if i < size then ", " else ")");
}

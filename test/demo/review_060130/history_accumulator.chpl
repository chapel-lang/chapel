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

def fwrite(f : file, x : history_real) {
  fwrite(f, x.f, " (");
  for i in 1..x.size do
    fwrite(f, x.h(i), if i < x.size then ", " else ")");
}

use SysCTypes;

extern {
  typedef struct {
    double (* F) (double x);
  } S;
}

export proc f1(x : real) : real {
  return x;
}
export proc f2(x : real) : real {
  return x;
}

config const option = true;

proc main() {
  var ptr = c_ptrTo(f1);
  if option then
    ptr = c_ptrTo(f2);

  var s1 : S;
  s1.F = ptr;
}

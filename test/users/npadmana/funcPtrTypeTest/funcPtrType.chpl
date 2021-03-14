use SysCTypes;

extern {
  typedef struct {
    double (* F) (double x);
  } S;
}

export proc f1(x : real) : real {
  return x;
}

var s1 : S;
s1.F = c_ptrTo(f1);

class C{
  type t;
}

proc generic_fun(type t1, type t2, x:C(t1), y:C(t2)) : C(t1) checked {
  return y;
}

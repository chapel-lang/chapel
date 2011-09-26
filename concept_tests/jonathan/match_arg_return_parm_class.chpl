class C{
  type t;
}

proc not_generic(x:int) : int {
  return x;
}

proc generic_fun(type t, y:C(t)) : C(t) checked {
  return y;
}

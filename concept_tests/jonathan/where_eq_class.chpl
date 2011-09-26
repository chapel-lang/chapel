class C {
  type t;
}

proc generic_fun(type a, type b, x : C(a)) : C(b) where C(a)==C(b) checked {
  return x;
}

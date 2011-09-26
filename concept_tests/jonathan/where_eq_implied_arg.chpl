class C {
  type t;
}

proc generic_fun(type a, type b, x : a) : b where C(a)==C(b) checked {
  return x;
}

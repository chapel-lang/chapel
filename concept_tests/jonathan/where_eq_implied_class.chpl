class C {
  type t;
}

proc generic_fun(type a, type b, x : C(a)) : C(b) where a==b checked {
  return x;
}

class C {
  type t;
}

proc generic_fn(type a, type b, x:C(a)) : b where C(a) == b checked {
  return x;
}

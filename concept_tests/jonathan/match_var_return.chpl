class C{
  type t;
}

proc generic_fun(type t, y:C(t)) : C(t) checked {
  var x : C(t);
  return x;
}

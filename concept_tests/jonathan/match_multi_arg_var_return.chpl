class C{
  type t;
}

proc generic_fun(type t1, type t2, y:C(t1)) : C(t2) checked {
  var x : C(t2);
  return x;
}

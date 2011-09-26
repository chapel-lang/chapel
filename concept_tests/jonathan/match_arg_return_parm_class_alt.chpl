class C{
  type t;
}

proc not_generic(x:int) : int {
  var y = 4;
  return y;
}

proc generic_fun(type a, type b, y:C(a)) : C(a) checked {
  var za:C(a);
  var zb:C(b);
  return za;
}

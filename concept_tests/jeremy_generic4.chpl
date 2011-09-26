class C {
  type T;
  type U;
}

proc generic_fun (x: C(?T, ?U)) : C(T, U) {
  var a: T;
  var b: U;
  return x;
}

proc main {
  var c = new C(int, real);
  var d = generic_fun(c);
}


class C {
  type T;
}

proc generic_fun (x: C(?T), y: C(?U)) : C(T) {
  var a: T;
  var b: U;
  return y;
}

proc main {
  var c = new C(int);
  var d = generic_fun(c, c);
}


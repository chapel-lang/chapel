class C {
  type T;
}

proc generic_fun (x: C(?T)) : C(T) {
  return x;
}

proc main {
  var c = new C(int);
  var d = generic_fun(c);
}


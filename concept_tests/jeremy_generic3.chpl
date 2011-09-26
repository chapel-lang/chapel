class C {
  type T;
}

proc generic_fun (x: C(?T)) : T {
  var y : T;
  return y;
}

proc main {
  var c = new C(int);
  var d = generic_fun(c);
}


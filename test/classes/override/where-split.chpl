class Parent {
  proc f(param p) where p == 1 { }
  proc g(type t) where t == int { }
  proc h(x) where x.type == int { }
}

class Child : Parent {
  proc f(param p) where p == 2 { }
  proc g(type t) where t == string { }
  proc h(x) where x.type == string { }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow():borrowed Parent;
  y.f(1);
  x.f(2);
  y.g(int);
  x.g(string);
  y.h(1);
  x.h("hi");
}
test();

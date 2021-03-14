class Parent {
  proc f(param p) where p == 1 || p == 2 { }
  proc g(type t) where t == int || t == uint { }
  proc h(x) where x.type == int || x.type == uint { }
}

class Child : Parent {
  proc f(param p) where p == 2 || p == 3 { }
  proc g(type t) where t == uint || t == string { }
  proc h(x) where x.type == uint || x.type == string { }
}

proc test() {
  var x = new owned Child();
  var y = x.borrow():borrowed Parent;
  y.f(1);
  y.f(2);
  x.f(2);
  x.f(3);

  y.g(int);
  y.g(uint);
  x.g(uint);
  x.g(string);
  
  y.h(1:int);
  y.h(1:uint);
  x.h(1:uint);
  x.h("hi");
}
test();

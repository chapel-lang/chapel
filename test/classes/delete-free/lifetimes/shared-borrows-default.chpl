pragma "safe"
module l16 {



class MyClass {
  var x:int;
}

record R {
  var c:shared MyClass;
}

proc f(c:shared MyClass) {
  var r = new R(c);
  return r;
}

proc g(c:shared MyClass) {
  return new R(c);
}

proc h() {
  return new R(new shared MyClass(10));
}

proc i() {
  var r = new R(new shared MyClass(10));
  return r;
}

proc j() {
  return h();
}

proc k() {
  var r = h();
  return r;
}

proc l(c:shared MyClass) {
  var r = f(c);
  return r;
}

proc m(c:shared MyClass) {
  return f(c);
}

proc test() {
  var myshared = new shared MyClass(1);

  var v1 = f(myshared);
  var v2 = g(myshared);
  var v3 = h();
  var v4 = i();
  var v5 = j();
  var v6 = k();
  var v7 = l(myshared);
  var v8 = m(myshared);

  writeln(v1.c.borrow());
  writeln(v2.c.borrow());
  writeln(v3.c.borrow());
  writeln(v4.c.borrow());
  writeln(v5.c.borrow());
  writeln(v6.c.borrow());
  writeln(v7.c.borrow());
  writeln(v8.c.borrow());
}

test();
}


proc foo() {
  return 1;
}

class X {
  var x : int;
}

class A {
  var x = new owned X(foo());
}

var a = new owned A();

class B {
  var a = new owned A(new owned X(foo()));
}

var b = new owned B();

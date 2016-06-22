class A {
  type eltType;
}

class B : A {
}

var a = new A(int);
var b = new B(int);

delete b;
delete a;

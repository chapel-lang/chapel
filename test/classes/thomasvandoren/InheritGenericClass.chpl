class A {
  type eltType;
}

class B : A {
}

var a = new owned A(int);
var b = new owned B(int);

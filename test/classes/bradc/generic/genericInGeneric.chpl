class C {
  type t;
}

class D {
  type t2;
  
  var x = new C(t2);
}

class E {
  type t3;

  var x = new D();
}

var myE: E(int) = new E(int);

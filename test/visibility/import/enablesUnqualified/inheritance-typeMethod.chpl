module M {
  class C {
    var x: int;
    proc type foo() { writeln("In C.foo()"); }
    operator +(c1: C, c2: C) { return new C(c1.x + c2.x); }
  }
}

module N {
  import M.C;
  class D: C { }

  var myD = new D();
  D.foo();
}

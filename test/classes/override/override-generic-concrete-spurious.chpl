class A {
}

class C {
  proc foo(x: int) {
    writeln("In parent foo()");
  }
}

class D : C {
  type t;
  
  override proc foo(x: shared A) {
    writeln("In child foo()");
  }
}

var myD2 = new owned D(int);
myD2.foo(new shared A());

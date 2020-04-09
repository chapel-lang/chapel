class A {
}

class C {
  proc foo(x: shared A) {
    writeln("In parent foo()");
  }
}

class D : C {
  type t;
  
  proc foo(x: shared A) {
    writeln("In child foo()");
  }
}

var myD2: C = new owned D(int);
myD2.foo(new shared A());

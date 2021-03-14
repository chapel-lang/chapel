class C {
  var X: [?D] real;

  proc foo() {
    writeln(D);
  }
}

var A: [1..1000] real;

var myC = new C(A);

myC.foo();

class A {
  proc X.fooA() { writeln("X.fooA()"); }  // error: X is undefined
}

var myA = new A();
myA.fooA();  // error: fooA() is not defined on A
